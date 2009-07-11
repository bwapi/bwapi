#include "AI.h"

#include <algorithm>
#include <fstream>
#include <math.h>

#include <Util/Exceptions.h>
#include <Util/FileLogger.h>
#include <Util/Dictionary.h>
#include <Util/Strings.h>
#include <Util/RectangleArray.h>

#include <BWAPI.h>

#include <BuildOrder/Root.h>
#include <BuildOrder/Branch.h>
#include <BuildOrder/Command.h>
#include <BuildOrder/BuildWeights.h>
#include <BuildOrder/Executor.h>

#include <Pathfinding/Utilities.h>

#include "Globals.h"
#include "Task.h"
#include "TaskGatherGas.h"
#include "TaskGather.h"
#include "TaskBuild.h"
#include "TaskInvent.h"
#include "TaskUpgrade.h"
#include "TaskTrain.h"
#include "TaskFight.h"
#include "Player.h"
#include "Unit.h"
#include "Expansion.h"
#include "Map.h"
#include "MapInfo.h"
#include "MapExpansion.h"
#include "MapStartingPosition.h"
#include "BuildingPositionSet.h"
#include "Formation.h"
#include "LuaAI.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  AI::AI(void)
  :mapInfo(NULL)
  ,startingPosition(NULL)
  ,root(NULL)
  ,temp(NULL)
  ,pathFinding(NULL)
  ,mineralGatherers(0)
  ,buildOrderExecutor(NULL)
  {

    try
    {
     config = new Util::Dictionary("bwapi-data\\bwapi.ini");
     log = new Util::FileLogger(config->get("log_path") + "\\ai",   Util::LogLevel::Normal);
     deadLog =new Util::FileLogger(config->get("log_path") + "\\dead", Util::LogLevel::MicroDetailed);
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error","wt");
      fprintf_s(f, "Couldn't load configuration file bwapi.ini because: %s", exception.getMessage().c_str());
      fclose(f);
    }

    BWAI::ai = this;
    BWAPI::BWAPI_init();
    drawColor=BWAPI::Color(0,0,255);
    try
    {
      Expansion::maximumMineralDistance = Util::Strings::stringToInt(config->get("max_mineral_distance"));
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Used default value for max_mineral_distance as it couldn't be loaded exception: %s", exception.getMessage().c_str());
    }
    
    for (int i = 0; i < 228; i++)
      this->buildTaskUnitsPlanned[i] = 0;

    try
    {
      this->root = new BuildOrder::Root(config->get("build_order_path"));    
      Util::Logger::globalLog->log("Build order loaded");
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Error when loading build order: %s", exception.getMessage().c_str());
    }
    this->savedUnitAddress=NULL;
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  AI::~AI(void)
  {
    delete root;
  
    for each (TaskBuild* i in this->plannedBuildings)
      delete i;
      
    for each (Expansion* i in this->expansions)
      delete i;
      
    for(std::set<Unit*>::iterator u=this->units.begin();u!=this->units.end();u++)
      delete (*u);
    this->units.clear();
    this->unit_mapping.clear();

    delete this->log;
    delete deadLog;
  }
  //---------------------------------------- BETTER MINERAL PATCH FOR ----------------------------------------
  bool AI::betterMinralPatch(BWAI::TaskGather* task1, BWAI::TaskGather* task2, Unit* optimiseFor)
  {
    if (task1->executors.size() < task2->executors.size())
      return true;
    if (task1->executors.size() > task2->executors.size())
      return false;
    if (optimiseFor != NULL &&
        task1->getExpansion()->gatherCenter != task2->getExpansion()->gatherCenter)
    {
      u16 distance1 = optimiseFor->getCenterDistance(task1->getExpansion()->gatherCenter);
      u16 distance2 = optimiseFor->getCenterDistance(task2->getExpansion()->gatherCenter);
      if (distance1 < distance2)
        return true;
      if (distance1 > distance2)
       return false;
    }
    u16 distance1 = task1->getMineral()->getDistance(task1->getExpansion()->gatherCenter);
    u16 distance2 = task2->getMineral()->getDistance(task1->getExpansion()->gatherCenter);
    if (distance1 < distance2)
      return true;
    if (distance1 > distance2)
      return false;
    return false;
  }
  //----------------------------------------- BETTER WORKER TO FREE ------------------------------------------
  bool AI::betterWorkerToFree(Unit* worker1, Unit* worker2, const BWAPI::Position& buildingPosition)
  {
    if (worker2 == NULL)
      return true;
   
    if ((worker1->getTask() == NULL || worker1->getTask()->getType() == BWAI::TaskType::Gather) && 
        (worker2->getTask() != NULL && worker2->getTask()->getType() != BWAI::TaskType::Gather))
      return true;
        if ((worker2->getTask() == NULL || worker2->getTask()->getType() == BWAI::TaskType::Gather) && 
            (worker1->getTask() != NULL && worker1->getTask()->getType() != BWAI::TaskType::Gather))
      return false;
    
    if (worker1->getTask() != NULL && worker2->getTask() != NULL)
    {
      if (worker1->getTask()->getType() == BWAI::TaskType::Gather &&
          worker2->getTask()->getType() != BWAI::TaskType::Gather)
        return true;

      if (worker1->getTask()->getType() != BWAI::TaskType::Gather &&
          worker2->getTask()->getType() == BWAI::TaskType::Gather)
        return false;
      if (worker1->getTask()->getType() == BWAI::TaskType::Gather)
      {
        if (((TaskGather*)worker1->getTask())->getExpansion() == 
            ((TaskGather*)worker1->getTask())->getExpansion() &&
            ((TaskGather*)worker1->getTask())->getExpansion() != NULL)
        { /* This part prioritise scv's near the command center when they gather, so scv's in mineral line
             Wont be taken, it causes them to be stucked there sometimes. */
          if (worker2->getOrder() == BWAPI::Orders::ReturnMinerals &&
              worker1->getOrder() == BWAPI::Orders::MoveToMinerals)
            return true;
          if (worker1->getOrder() == BWAPI::Orders::ReturnMinerals &&
              worker2->getOrder() == BWAPI::Orders::MoveToMinerals)
            return false;
          u16 distance1 = ((TaskGather*)worker1->getTask())->getExpansion()->gatherCenter->getDistance(worker1);
          u16 distance2 = ((TaskGather*)worker2->getTask())->getExpansion()->gatherCenter->getDistance(worker2);
          if (distance1 < distance2)
            return true;
          if (distance1 > distance2)
            return false;
        }
        else
        {
          u16 distance1 = ((TaskGather*)worker1->getTask())->getExpansion()->gatherCenter->getDistance(buildingPosition);
          u16 distance2 = ((TaskGather*)worker2->getTask())->getExpansion()->gatherCenter->getDistance(buildingPosition);
          if (distance1 < distance2)
            return true;
          if (distance1 > distance2)
            return false;
        }
      }
    }
 
    double distance1 = buildingPosition.getDistance(worker1->getPosition());
    double distance2 = buildingPosition.getDistance(worker2->getPosition());
 
    if (distance1 < distance2)
      return true;
    if (distance1 > distance2)
      return false;
    return false;
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void AI::update(void)
  {

    try
    {
      std::set<BWAPI::Unit*> allUnits=BWAPI::Broodwar->getAllUnits();
      for(std::set<Unit*>::iterator u=this->units.begin();u!=this->units.end();)
      {
        /** && false because Game::getUnits does not yet include loaded units (i.e. in dropship/refinery/bunker/etc */
        if (allUnits.find((*u)->getUnit())==allUnits.end() && false)
        {
          this->unit_mapping.erase((*u)->getUnit());
          delete (*u);
          u=this->units.erase(u);
        }
        else
        {
          u++;
        }
      }
      for(std::set<BWAPI::Unit*>::iterator u=allUnits.begin();u!=allUnits.end();u++)
      {
        this->getUnit(*u);
      }

      this->reserved.clear();
      for each (TaskBuild* i in this->plannedBuildings)
      {
        if (i->getBuilding() ==NULL)
        {
          this->reserved += i->getReserved();
        }
      }

    }
    catch (GeneralException& exception)
    {
      this->root->log->log("Exception in AI::update: %s", exception.getMessage().c_str());
    }
  }
  //------------------------------------------------ ON START ------------------------------------------------
  void AI::onStart()
  {
    try
    {
      this->log->logImportant("Ai::onStart start");
      root = new BuildOrder::Root(config->get("build_order_path"));
    }
    catch (GeneralException& exception)
    {
      this->log->logCritical("Error when loading build order: %s", exception.getMessage().c_str());
      return;
    }
    try
    {
      this->player = new Player(BWAPI::Broodwar->self());
      this->opponent = new Player(BWAPI::Broodwar->enemy());
      this->player_mapping.insert(std::make_pair(BWAPI::Broodwar->self(),this->player));
      this->player_mapping.insert(std::make_pair(BWAPI::Broodwar->enemy(),this->opponent));
      BWAPI::Broodwar->enableFlag(BWAPI::Flag::CompleteMapInformation);
      BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

      char mapPath[256] = "\0";
      sprintf_s(mapPath, 256, "%s\\%08x.xml", config->get("maps_path").c_str(), BWAPI::Broodwar->getMapHash());
      mapInfo = new MapInfo(mapPath);
      this->checkNewExpansions();
      this->root->log->log("Help pre-prepared information found for the curent map");
      if (this->expansions.size())
      {
        for each (MapStartingPosition* i in mapInfo->startingPositions)
          if (this->expansions.front()->gatherCenter->getDistance(i->expansion->getPosition()) < 100)
          {
            this->startingPosition = i;
            break;
          }
        if (this->startingPosition)
        {
         this->root->log->log("Starting position is (%s) at (%d, %d)", 
                        this->startingPosition->expansion->getID().c_str(), 
                        this->startingPosition->expansion->getPosition().x(), 
                        this->startingPosition->expansion->getPosition().y());
        }
      }
      this->pathFinding = new PathFinding::Utilities();
    }
    catch (GeneralException& exception)
    {
      this->root->log->log("Exception in AI::onStart: %s", exception.getMessage().c_str());
      delete this->mapInfo;
      this->mapInfo = NULL;
    }
    if (this->root != NULL)
    {
      this->buildOrderExecutor = this->root->getStart();
      if (this->buildOrderExecutor == NULL)
        this->root->log->log("Didn't find build order to play with %s against %s", 
                             this->player->getRace().getName().c_str(), 
                             this->opponent->getRace().getName().c_str());
      else
        this->root->log->log("Chose root branch : %s", 
                             this->buildOrderExecutor->actualBranch()->getName().c_str());
    }
    this->mineralGatherers = 0;
    this->log->logImportant("Ai::onStart end\n\n");
  }
  //------------------------------------------------- ON END -------------------------------------------------
  void AI::onEnd()
  {
    this->log->logImportant("Ai::onEnd start");
    for each (Expansion* i in this->expansions)
      delete i;
    this->expansions.clear();
    this->activeMinerals.clear();
    this->expansionsSaturated = false;
    
    delete this->mapInfo;
    this->mapInfo = NULL;
    
    for each (TaskBuild* i in this->plannedBuildings)
      delete i;
    this->plannedBuildings.clear();
    
    for each (TaskInvent* i in this->plannedInvents)
      delete i;
    this->plannedInvents.clear();
    
    for each (TaskUpgrade* i in this->plannedUpgrades)
      delete i;
    this->plannedUpgrades.clear();
    
    for each (TaskGatherGas* i in this->activeRefineries)
      delete i;
    this->activeRefineries.clear();
    
    for each (TaskFight* i in this->fightGroups)
      delete i;
    this->fightGroups.clear();
    
    for each (TaskTrain* i in this->plannedUnits)
      delete i;
    this->plannedUnits.clear();
    
    for(std::set<Unit*>::iterator u=this->units.begin();u!=this->units.end();u++)
    {
      (*u)->clearTask();
      (*u)->expansion = NULL;
    }
      
    this->startingPosition = NULL;  
    
    delete this->pathFinding;
    this->pathFinding = NULL;
        
    delete this->buildOrderExecutor;
    this->buildOrderExecutor = NULL;
    
    this->log->logImportant("Ai::onEnd end\n\n");
  }
  //-----------------------------------------------  ON FRAME ------------------------------------------------
  void AI::onFrame(void)
  {
    BWAPI::Broodwar->drawCircle(BWAPI::CoordinateType::Map,BWAPI::Broodwar->self()->getStartLocation().x()*32+2*32,BWAPI::Broodwar->self()->getStartLocation().y()*32+3*16,100,BWAPI::Colors::Red,false);
    std::set<BWAPI::Unit*> myUnits=BWAPI::Broodwar->self()->getUnits();
    for(std::set<BWAPI::Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++)
    {
      if ((*i)->isMoving())
      {
        BWAPI::Broodwar->drawLine(BWAPI::CoordinateType::Map,(*i)->getPosition().x(),(*i)->getPosition().y(),(*i)->getTargetPosition().x(),(*i)->getTargetPosition().y(),BWAPI::Colors::Green);
      }
    }
    this->update();
    try
    {
      if (BWAPI::Broodwar->getFrameCount() < 2)
        return;
      if (!this->player)
        return;

      if (this->buildOrderExecutor)
        this->buildOrderExecutor->execute();

       
      //this->checkSupplyNeed();
      this->checkNewExpansions();

      std::list<Unit*> idleWorkers;
      this->getIdleWorkers(idleWorkers);
      #pragma region DisabledPathFindingPerformanceTest
      /*
      if (!idleWorkers.empty())
        temp = idleWorkers.front();
      if (temp)
      {
        PathFinding::UnitModel source(temp);
        this->pathFinding->generatePath(source, PathFinding::WalkabilityPosition(20, 20));
      }
      */
      #pragma endregion

      if (activeMinerals.size() > 0)
      {
        this->assignIdleWorkersToMinerals(idleWorkers);
        this->rebalanceMiners();
        this->checkAssignedWorkers();
      }
      //this->performAutoBuild();
      this->executeTasks();
      
      if (this->cycle && !this->fightGroups.empty())
      {
        TaskFight* task = this->fightGroups.front();
        Formation* formation = new Formation(task->executors);
        formation->generatePositions(cyclePosition, cycleAngle);
        formation->execute();
        delete formation;
        cycleAngle += (float)0.01;
      } 
    }
    catch (GeneralException& e)
    {
      this->log->logCritical("Exception caught in AI::onFrame: %s", e.getMessage().c_str());
    }
  }
  //----------------------------------------------- GET PLAYER -----------------------------------------------
  Player* AI::getPlayer(BWAPI::Player* player)
  {
    if (this->player_mapping.find(player)==this->player_mapping.end())
      return NULL;
    return this->player_mapping.find(player)->second;
  }
  //------------------------------------------------ GET UNIT ------------------------------------------------
  Unit* AI::getUnit(BWAPI::Unit* unit)
  {
    if (unit==NULL) return NULL;
    if (this->unit_mapping.find(unit)==this->unit_mapping.end())
    {
      this->unit_mapping.insert(std::make_pair(unit,new Unit(unit)));
    }
    Unit* u=this->unit_mapping.find(unit)->second;
    this->units.insert(u);
    return u;
  }
  //-------------------------------------------- ON CANCEL TRAIN ---------------------------------------------
/*  void AI::onCancelTrain()
  {
    this->log->log("Cancelled unit caught");
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
  }*/
  //------------------------------------------ START NEW EXPANSION -------------------------------------------
   void AI::startNewExpansion(Unit *gatherCenter)
   {
     this->expansions.push_back(new Expansion(gatherCenter));
     this->rebalanceMiners();
   }
  u16 pos = 0;
  //-------------------------------------------- REBALANCE MINERS --------------------------------------------
   void AI::rebalanceMiners(void)
   {
     if (activeMinerals.size() < 2)
       return;
     Unit* gatherer;
     anotherStep:
     for each (TaskGatherGas* i in this->activeRefineries)
       if (i->executors.size() < 3)
       {
         Unit* newUnitToGatherGas = this->freeBuilder(i->getRefinery()->getPosition());
         if (newUnitToGatherGas != NULL)
           i->addExecutor(newUnitToGatherGas);
       }
     TaskGather* best = activeMinerals.front();
     TaskGather* worst = activeMinerals.front();
     for each (TaskGather* i in this->activeMinerals)
     {
       if (best->executors.size() > i->executors.size())
         best = i;
       else 
         if (worst->executors.size() < i->executors.size())
           worst = i;
     }

     if (best->executors.size() + 1 < worst->executors.size())
     {
       gatherer = worst->executors.front();
       gatherer->freeFromTask();
       AI::optimizeMineralFor = gatherer;
       
       best = bestFor(gatherer);
       
       this->log->log("Gatherer [%d] reabalanced from [%d] to [%d]", 
                       (int)gatherer, 
                       (int)(worst->getMineral()),
                       (int)(best->getMineral()));

       best->addExecutor(gatherer);
       goto anotherStep;
     }
   }
  //----------------------------------------- CHECK ASSIGNED WORKERS -----------------------------------------
  void AI::checkAssignedWorkers(void)
  {
    for each (TaskGather* i in this->activeMinerals)
      i->execute();
  }
  //----------------------------------------------------------------------------------------------------------
  Unit* AI::optimizeMineralFor = NULL;
  
  //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
  void AI::onRemoveUnit(BWAPI::Unit* unit)
  {
    Unit* dead = BWAI::Unit::BWAPIUnitToBWAIUnit(unit);
    this->deadLog->log("AI::onRemove Unit %s just died", dead->getName().c_str());
    if (dead->getType().isBuilding())
      for each (TaskBuild* i in this->plannedBuildings)
        if (dead == i->getBuilding())
          i->buildingDied();
    if (dead->isMineral())
    {
      if (dead->expansion != NULL)
        dead->expansion->removeMineral(dead);
    }
    else if (dead->getType() == BWAPI::UnitTypes::Terran_Refinery ||
             dead->getType() == BWAPI::UnitTypes::Zerg_Extractor ||
             dead->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
    {
      if (dead->expansion != NULL)
        this->removeGatherGasTask(dead);
    }
    else if (
              (
               dead->getType() == BWAPI::UnitTypes::Terran_Command_Center ||
               dead->getType() == BWAPI::UnitTypes::Protoss_Nexus ||
               dead->getType() == BWAPI::UnitTypes::Zerg_Hatchery ||
               dead->getType() == BWAPI::UnitTypes::Zerg_Lair ||
               dead->getType() == BWAPI::UnitTypes::Zerg_Hive
              ) &&
              dead->expansion != NULL
            )
      this->removeExpansion(dead->expansion);
    if (dead->getTask() != NULL)
      dead->freeFromTask();
    dead->expansion = NULL;
    this->deadLog->log("AI::onRemoveUnit end", dead->getName().c_str());
    if (this->unit_mapping.find(unit)!=this->unit_mapping.end())
    {
      Unit* u=this->unit_mapping.find(unit)->second;
      this->unit_mapping.erase(unit);
      units.erase(u);
      delete u;
    }
   }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  bool AI::onSendText(std::string text)
  {
    std::string message = text;
    std::vector<std::string> parsed = Util::Strings::splitString(message, " ");
    parsed.push_back("");parsed.push_back("");parsed.push_back(""); // to avoid range checks everywhere
    if (parsed[0] == "/setColor")
    {
      int red=Util::Strings::stringToInt(std::string(parsed[1]));
      int green=Util::Strings::stringToInt(std::string(parsed[2]));
      int blue=Util::Strings::stringToInt(std::string(parsed[3]));
      this->drawColor=BWAPI::Color(red,green,blue);
    }
    else if (parsed[0] == "/getColor")
    {
      BWAPI::Broodwar->print("%d (%d,%d,%d)",this->drawColor.getID(),this->drawColor.red(),this->drawColor.green(),this->drawColor.blue());
    }
    else if (parsed[0] == "/setScreenPosition")
    {
      int x=Util::Strings::stringToInt(std::string(parsed[1]));
      int y=Util::Strings::stringToInt(std::string(parsed[2]));
      BWAPI::Broodwar->setScreenPosition(x,y);
    }
    else if (parsed[0] == "/getStartLocation")
    {
      BWAPI::Broodwar->print("(%d,%d)",BWAPI::Broodwar->self()->getStartLocation().x(),BWAPI::Broodwar->self()->getStartLocation().y());
    }
    else if (parsed[0] == "/save")
    {
      if (parsed[1] == "fog")
      {
        std::string result = Map::saveFogOfWarMap(config->get("data_path") + "\\fog-of-war.txt");
        BWAPI::Broodwar->print(result.c_str());
      }
      else if (parsed[1] == "creep")
      {
        std::string result = Map::saveZergCreepMap(config->get("data_path") + "\\zerg-creep.txt");
        BWAPI::Broodwar->print(result.c_str());
      }
      else if (parsed[1] == "techs")
      {
        std::string fileName = config->get("data_path") + "\\techs";
        Util::FileLogger techsLog(fileName, Util::LogLevel::MicroDetailed, false);
        for(std::set<BWAPI::TechType>::iterator i=BWAPI::TechTypes::allTechTypes().begin();i!=BWAPI::TechTypes::allTechTypes().end();i++)
        {
          techsLog.log("%s = 0x%02X",i->getName().c_str(),i->getID());
        }
        BWAPI::Broodwar->print("Techs saved to %s .ini", fileName.c_str());
      }
      else if (parsed[1] == "upgrades")
      {
        std::string fileName = config->get("data_path") + "\\upgrades";
        Util::FileLogger upgradesLog(fileName, Util::LogLevel::MicroDetailed, false);
        for(std::set<BWAPI::UpgradeType>::iterator i=BWAPI::UpgradeTypes::allUpgradeTypes().begin();i!=BWAPI::UpgradeTypes::allUpgradeTypes().end();i++)
        {
          upgradesLog.log("%s = 0x%02X",i->getName().c_str(),i->getID());
        }
        BWAPI::Broodwar->print("Upgrades saved to %s .ini", fileName.c_str());
      }
      else if (parsed[1] == "units")
      {
        std::string fileName = config->get("data_path") + "\\units";
        Util::FileLogger unitsLog(fileName, Util::LogLevel::MicroDetailed, false);
        for(std::set<BWAPI::UnitType>::iterator i=BWAPI::UnitTypes::allUnitTypes().begin();i!=BWAPI::UnitTypes::allUnitTypes().end();i++)
        {
          unitsLog.log("%s = 0x%02X",i->getName().c_str(),i->getID());
        }
        BWAPI::Broodwar->print("Units saved to %s .ini", fileName.c_str());        
      }      
      else if (parsed[1] == "buildability")
      {
        std::string result = Map::saveBuildabilityMap(config->get("data_path") + "\\buildability.txt");
        BWAPI::Broodwar->print(result.c_str());
      }
      else if (parsed[1] == "walkability")
      {
        std::string result = Map::saveWalkabilityMap(config->get("data_path") + "\\walkability.txt");
        BWAPI::Broodwar->print(result.c_str());
      }
      else if (parsed[1] == "height")
      {
        std::string result = Map::saveHeightMap(config->get("data_path") + "\\height.txt");
        BWAPI::Broodwar->print(result.c_str());
      }
      else if (parsed[1] == "startlocations")
      {
        std::string fileName = config->get("data_path") + "\\startlocations";
        Util::FileLogger startlocationsLog(fileName, Util::LogLevel::MicroDetailed, false);
        startlocationsLog.log("%s has %d start locations:",BWAPI::Broodwar->mapName().c_str(),BWAPI::Broodwar->getStartLocations().size());
        BWAPI::Broodwar->print("%s has %d start locations:",BWAPI::Broodwar->mapName().c_str(),BWAPI::Broodwar->getStartLocations().size());
        for(std::set<BWAPI::TilePosition>::const_iterator i=BWAPI::Broodwar->getStartLocations().begin();
          i!=BWAPI::Broodwar->getStartLocations().end();i++)
        {
          double angle=atan2((*i).y()-BWAPI::Broodwar->mapHeight()/2.0,(*i).x()-BWAPI::Broodwar->mapWidth()/2.0);
          double clock_deg=angle*180.0/3.14159265+90.0;
          int clock_hour=((int)floor(clock_deg*12.0/360.0+0.5)+11)%12+1;
          startlocationsLog.log("%d o clock: %d, %d",clock_hour, (*i).x(), (*i).y());
          BWAPI::Broodwar->print("%d o clock: %d, %d",clock_hour, (*i).x(), (*i).y());
        }
        BWAPI::Broodwar->print("Saved start locations");

      }
      else if (parsed[1] == "defined" && parsed[2] == "buildings")
      {
        if (this->mapInfo != NULL)
        {
          std::string fileName = config->get("data_path") + "\\pre-defined-buildings.txt";
          mapInfo->saveDefinedBuildingsMap(fileName);
          BWAPI::Broodwar->print("Defined buildings saved to %s", fileName.c_str());
        }
        else
          BWAPI::Broodwar->print("Map info for the current map is not available.");
      }
      else
        BWAPI::Broodwar->print("Unknown command '%s' - possible commands are: fog, techs, upgrades, units, "
                              "buildability, walkability, height, startlocations, defined buildings", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/count")
    {
      BWAPI::Broodwar->print("Count Pylon: %d; %d", BWAI::ai->buildTaskUnitsPlanned[(u16)BWAPI::UnitTypes::Protoss_Pylon.getID()], BWAPI::Broodwar->self()->getAllUnits(BWAPI::UnitTypes::Protoss_Pylon));
      return true;
    }
    else if (parsed[0] == "/tech")
    {
      if (parsed[1] == "add")
      {
        std::string techName = message.substr(strlen("/tech add "), message.size() - strlen("/tech add "));
        BWAPI::TechType tech = BWAPI::TechTypes::getTechType(techName);
        if (tech == BWAPI::TechTypes::None)
          BWAPI::Broodwar->print("Unknown upgrade name '%s'", techName);
        else
        {
          if (this->player->canAfford(tech))
          {
            this->plannedInvents.push_back(new BWAI::TaskInvent(tech, 0));
            BWAPI::Broodwar->print("Added tech '%s'", techName.c_str());
          }
          else
            BWAPI::Broodwar->print("Cant afford the tech right now -> Try again later");
        }
        return true;
      }
      else if (parsed[1] == "list")
      {
        for each (TaskInvent* i in this->plannedInvents)
          BWAPI::Broodwar->print(i->getTechType().getName().c_str());
      }
      else 
        BWAPI::Broodwar->print("Unknown command '%s' - possible commands are: add, list", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/upgrade")
    {
      if (parsed[1] == "add")
      {
        std::string upgradeName = message.substr(strlen("/upgrade add "), message.size() - strlen("/upgrade add "));
        BWAPI::UpgradeType upgrade = BWAPI::UpgradeTypes::getUpgradeType(upgradeName);
        if (upgrade == BWAPI::UpgradeTypes::None)
          BWAPI::Broodwar->print("Unknown upgrade name '%s'", upgradeName);
        else
        {
          if (this->player->canAfford(upgrade, this->player->upgradeLevel(upgrade) + 1))
          {
            this->plannedUpgrades.push_back(new BWAI::TaskUpgrade(upgrade, this->player->upgradeLevel(upgrade) + 1, 0));
            BWAPI::Broodwar->print("Added upgrade '%s' level %d", upgradeName, this->player->upgradeLevel(upgrade) + 1);
          }
          else
            BWAPI::Broodwar->print("Cant afford the upgrade right now -> Try again later");
        }
        return true;
      }
      else if (parsed[1] == "list")
      {
        for each (TaskUpgrade* i in this->plannedUpgrades)
          BWAPI::Broodwar->print(i->getUpgradeType().getName().c_str());
      }
      else 
        BWAPI::Broodwar->print("Unknown command '%s' - possible commands are: add, list", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/fight")
    {
      if (parsed[1] == "add")
      {
        if (parsed[2] == "all")
        {
          if (this->fightGroups.empty())
            this->fightGroups.push_back(new TaskFight());
          TaskFight* task = this->fightGroups.front();
          u16 addedCount = 0;
          for each (Unit* i in this->units)
            if (!i->getType().isBuilding() &&
                !i->getType().isWorker() &&
                i->getType().canMove() &&
                i->getOwner() == this->player &&
                i->getTask() == NULL)
            {
              addedCount++;
              task->addExecutor(i);
            }
          BWAPI::Broodwar->print("%u units added to the fight group", addedCount);
        }
        else
        {
          if (this->fightGroups.empty())
            this->fightGroups.push_back(new TaskFight());
          TaskFight* task = this->fightGroups.front();
          for(std::set<BWAPI::Unit*>::const_iterator i=BWAPI::Broodwar->getSelectedUnits().begin();
            i!=BWAPI::Broodwar->getSelectedUnits().end();i++)
          {
            Unit* unit=BWAI::Unit::BWAPIUnitToBWAIUnit(*i);
            unit->freeFromTask();
            task->addExecutor(unit);
          }
        }
      }
      else if (parsed[1] == "remove")
      {
        if (this->fightGroups.empty())
          return true;
        if (parsed[2] == "all")
        {
          TaskFight* task = this->fightGroups.front();
          delete task;
          this->fightGroups.erase(this->fightGroups.begin());
        }
        else
        {
          for(std::set<BWAPI::Unit*>::const_iterator i=BWAPI::Broodwar->getSelectedUnits().begin();
            i!=BWAPI::Broodwar->getSelectedUnits().end();i++)
          {
            Unit* unit=BWAI::Unit::BWAPIUnitToBWAIUnit(*i);
            unit->freeFromTask();
          }
        }
      }
      else 
        BWAPI::Broodwar->print("Unknown add command '%s' - possible values are: add, add all, remove, remove all", parsed[1].c_str());
      return true;
    } 
    else if (parsed[0] == "/formation")
    {
      BWAPI::Position position = BWAPI::Position(BWAPI::Broodwar->getMouseX() + BWAPI::Broodwar->getScreenX(),
                                                 BWAPI::Broodwar->getMouseY() + BWAPI::Broodwar->getScreenY());
      if (parsed[1] == "cycle")
      {
        this->cyclePosition = position;
        this->cycleAngle = 0;
        cycle = true;
      }
      else if (parsed[1] == "uncycle")
        cycle = false;
      else
      {
        u16 angle = atoi(parsed[1].c_str());
        if (angle == 0 && parsed[1] != "0")
        {
          BWAPI::Broodwar->print("Invalid angle '%s'", parsed[1]);
        }
        if (this->fightGroups.empty())
          return true;        
        TaskFight* task = this->fightGroups.front();
        Formation* formation = new Formation(task->executors);
        formation->generatePositions(position, (float)(angle*(3.141)/180));
        formation->execute();
        delete formation;
      }
      return true;
    } 
    else if (parsed[0] == "/attack" || parsed[0] == "/patrol")
    {
      BWAPI::Position position(BWAPI::Broodwar->getMouseX() + BWAPI::Broodwar->getScreenX(),
                               BWAPI::Broodwar->getMouseY() + BWAPI::Broodwar->getScreenY());

      if (parsed[1] == "location")
      {
        if (this->fightGroups.empty())
          return true;        
        TaskFight* task = this->fightGroups.front();

        for each (Unit* i in task->executors)
        {
          if (parsed[0] == "/attack")
            i->orderAttackMove(position);
          else
            i->orderPatrol(position);
        }
      }
      else
      {
		  BWAPI::Broodwar->print("Unknown %s command '%s' - possible values are: location", (parsed[0] == "/attack") ? "attack" : "patrol" , parsed[1].c_str());
      }
      return true;
    }
    else if (parsed[0] == "/res")
    {
      BWAPI::Broodwar->print("Actual: Min: %d Gas: %d",this->player->getMinerals(),this->player->getGas());
      BWAPI::Broodwar->print("Reserved: Min: %d Gas: %d",this->reserved.minerals,this->reserved.gas);
      BWAPI::Broodwar->print("Available: Min: %d Gas: %d",this->player->getMinerals()-this->reserved.minerals,this->player->getGas()-this->reserved.gas);
      return true;
    }
    else if (parsed[0] == "/hash")
    {
      this->root->log->log("%08x", BWAPI::Broodwar->getMapHash());
      return true;
    }
    else if (parsed[0] == "/luatest")
    {
      if(lua.executeFile("bwapi-data\\test.lua") == false)
        this->root->log->log("Could not load test.lua.");
      else
        this->root->log->log("Load test.lua successfully.");
      return true;
    }
    else if (parsed[0] == "/reload")
    {
      if (parsed[1] == "map")
      {
        try
        {
          char mapPath[256] = "\0";
          sprintf_s(mapPath, 256, "%s\\%08x.xml", config->get("maps_path").c_str(), BWAPI::Broodwar->getMapHash());
          mapInfo = new MapInfo(mapPath);
          BWAPI::Broodwar->print("Map data reloaded successfully.");
        }
        catch (GeneralException& exception)
        {
          Util::Logger::globalLog->log("Error when loading map: %s", exception.getMessage().c_str());
          return true;
        }
      }
      else if (parsed[1] == "bo")
      {
        try
        {
          root = new BuildOrder::Root(config->get("build_order_path"));
          BWAPI::Broodwar->print("Build order reloaded successfully.");
        }
        catch (GeneralException& exception)
        {
          Util::Logger::globalLog->log("Error when loading build order: %s", exception.getMessage().c_str());
          return true;
        }
      }
      else
        BWAPI::Broodwar->print("Unknown reload command '%s' - possible values are: map, bo", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/type")
    {
      std::set<BWAPI::Unit*> selectedUnits=BWAPI::Broodwar->getSelectedUnits();
      if (selectedUnits.size()>0)
      {
        BWAPI::Unit* unit=*selectedUnits.begin();
        BWAPI::UnitType type=unit->getType();
        if (parsed[1] == "supplyRequired")
        {
          BWAPI::Broodwar->printPublic("%d",type.supplyRequired());
        }
        else if (parsed[1] == "supplyProvided")
        {
          BWAPI::Broodwar->printPublic("%d",type.supplyProvided());
        }
        else if (parsed[1] == "canProduce")
        {
          BWAPI::Broodwar->printPublic("%s",type.canProduce() ? "true" : "false");
        }
        else if (parsed[1] == "canAttack")
        {
          BWAPI::Broodwar->printPublic("%s",type.canAttack() ? "true" : "false");
        }
        else if (parsed[1] == "canMove")
        {
          BWAPI::Broodwar->printPublic("%s",type.canMove() ? "true" : "false");
        }
        else if (parsed[1] == "isFlyer")
        {
          BWAPI::Broodwar->printPublic("%s",type.isFlyer() ? "true" : "false");
        }
        else if (parsed[1] == "regeneratesHP")
        {
          BWAPI::Broodwar->printPublic("%s",type.regeneratesHP() ? "true" : "false");
        }
        else if (parsed[1] == "isSpellcaster")
        {
          BWAPI::Broodwar->printPublic("%s",type.isSpellcaster() ? "true" : "false");
        }
        else if (parsed[1] == "hasPermanentCloak")
        {
          BWAPI::Broodwar->printPublic("%s",type.hasPermanentCloak() ? "true" : "false");
        }
        else if (parsed[1] == "isInvincible")
        {
          BWAPI::Broodwar->printPublic("%s",type.isInvincible() ? "true" : "false");
        }
        else if (parsed[1] == "isOrganic")
        {
          BWAPI::Broodwar->printPublic("%s",type.isOrganic() ? "true" : "false");
        }
        else if (parsed[1] == "isMechanical")
        {
          BWAPI::Broodwar->printPublic("%s",type.isMechanical() ? "true" : "false");
        }
        else if (parsed[1] == "isRobotic")
        {
          BWAPI::Broodwar->printPublic("%s",type.isRobotic() ? "true" : "false");
        }
        else if (parsed[1] == "isDetector")
        {
          BWAPI::Broodwar->printPublic("%s",type.isDetector() ? "true" : "false");
        }
        else if (parsed[1] == "isResourceContainer")
        {
          BWAPI::Broodwar->printPublic("%s",type.isResourceContainer() ? "true" : "false");
        }
        else if (parsed[1] == "isResourceDepot")
        {
          BWAPI::Broodwar->printPublic("%s",type.isResourceDepot() ? "true" : "false");
        }
        else if (parsed[1] == "isWorker")
        {
          BWAPI::Broodwar->printPublic("%s",type.isWorker() ? "true" : "false");
        }
        else if (parsed[1] == "requiresPsi")
        {
          BWAPI::Broodwar->printPublic("%s",type.requiresPsi() ? "true" : "false");
        }
        else if (parsed[1] == "requiresCreep")
        {
          BWAPI::Broodwar->printPublic("%s",type.requiresCreep() ? "true" : "false");
        }
        else if (parsed[1] == "isTwoUnitsInOneEgg")
        {
          BWAPI::Broodwar->printPublic("%s",type.isTwoUnitsInOneEgg() ? "true" : "false");
        }
        else if (parsed[1] == "isBurrowable")
        {
          BWAPI::Broodwar->printPublic("%s",type.isBurrowable() ? "true" : "false");
        }
        else if (parsed[1] == "isCloakable")
        {
          BWAPI::Broodwar->printPublic("%s",type.isCloakable() ? "true" : "false");
        }
        else if (parsed[1] == "isBuilding")
        {
          BWAPI::Broodwar->printPublic("%s",type.isBuilding() ? "true" : "false");
        }
        else if (parsed[1] == "isAddon")
        {
          BWAPI::Broodwar->printPublic("%s",type.isAddon() ? "true" : "false");
        }
        else if (parsed[1] == "isFlyingBuilding")
        {
          BWAPI::Broodwar->printPublic("%s",type.isFlyingBuilding() ? "true" : "false");
        }
        else if (parsed[1] == "isNeutral")
        {
          BWAPI::Broodwar->printPublic("%s",type.isNeutral() ? "true" : "false");
        }
        else
        {
          BWAPI::Broodwar->printPublic("Unrecognized method call: %s",parsed[1].c_str());
        }
      }
      return true;
    }
    else if (parsed[0] == "/call")
    {
      std::set<BWAPI::Unit*> selectedUnits=BWAPI::Broodwar->getSelectedUnits();
      if (selectedUnits.size()>0)
      {
        BWAPI::Unit* unit=*selectedUnits.begin();
        if (this->savedUnitAddress!=NULL)
        {
          unit=this->savedUnitAddress;
          BWAPI::Broodwar->printPublic("Using unit %d",unit);
        }

        if (parsed[1] == "saveAddress")
        {
          this->savedUnitAddress=unit;
        }
        else if (parsed[1] == "clearAddress")
        {
          this->savedUnitAddress=NULL;
        }
        else if (parsed[1] == "getAddress")
        {
          BWAPI::Broodwar->printPublic("%d",unit);
        }
        else if (parsed[1] == "getOwner")
        {
          BWAPI::Broodwar->printPublic("%s [%s]",unit->getOwner()->getName().c_str(),unit->getOwner()->getRace().getName().c_str());
        }
        else if (parsed[1] == "getType")
        {
          BWAPI::Broodwar->printPublic("%s",unit->getType().getName().c_str());
        }
        else if (parsed[1] == "health")
        {
          BWAPI::Broodwar->printPublic("%d",unit->health());
        }
        else if (parsed[1] == "shield")
        {
          BWAPI::Broodwar->printPublic("%d",unit->shield());
        }
        else if (parsed[1] == "energy")
        {
          BWAPI::Broodwar->printPublic("%d",unit->energy());
        }
        else if (parsed[1] == "resources")
        {
          BWAPI::Broodwar->printPublic("%d",unit->resources());
        }
        else if (parsed[1] == "getKillCount")
        {
          BWAPI::Broodwar->printPublic("%d",unit->getKillCount());
        }
        else if (parsed[1] == "groundWeaponCooldown")
        {
          BWAPI::Broodwar->printPublic("%d",unit->groundWeaponCooldown());
        }
        else if (parsed[1] == "airWeaponCooldown")
        {
          BWAPI::Broodwar->printPublic("%d",unit->airWeaponCooldown());
        }
        else if (parsed[1] == "spellCooldown")
        {
          BWAPI::Broodwar->printPublic("%d",unit->spellCooldown());
        }
        else if (parsed[1] == "defenseMatrixPoints")
        {
          BWAPI::Broodwar->printPublic("%d",unit->defenseMatrixPoints());
        }
        else if (parsed[1] == "defenseMatrixTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->defenseMatrixTimer());
        }
        else if (parsed[1] == "stimTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->stimTimer());
        }
        else if (parsed[1] == "ensnareTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->ensnareTimer());
        }
        else if (parsed[1] == "lockdownTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->lockdownTimer());
        }
        else if (parsed[1] == "stasisTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->stasisTimer());
        }
        else if (parsed[1] == "plagueTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->plagueTimer());
        }
        else if (parsed[1] == "maelstromTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->maelstromTimer());
        }
        else if (parsed[1] == "irradiateTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->irradiateTimer());
        }
        else if (parsed[1] == "getPosition")
        {
          BWAPI::Broodwar->printPublic("(%d,%d)",unit->getPosition().x(),unit->getPosition().y());
        }
        else if (parsed[1] == "getTilePosition")
        {
          BWAPI::Broodwar->printPublic("(%d,%d)",unit->getTilePosition().x(),unit->getTilePosition().y());
        }
        else if (parsed[1] == "getTarget")
        {
          if (unit->getTarget()==NULL)
          {
            BWAPI::Broodwar->printPublic("NULL");
          }
          else
          {
            BWAPI::Broodwar->printPublic("%d: %s",unit->getTarget(), BWAI::Unit::BWAPIUnitToBWAIUnit(unit)->getTarget()->getName().c_str());
          }
        }
        else if (parsed[1] == "getTargetPosition")
        {
          BWAPI::Broodwar->printPublic("(%d,%d)",unit->getPosition().x(),unit->getPosition().y());
        }
        else if (parsed[1] == "currentDirection")
        {
          BWAPI::Broodwar->printPublic("%d",unit->currentDirection());
        }
        else if (parsed[1] == "getOrder")
        {
          BWAPI::Broodwar->printPublic("%s",unit->getOrder().getName().c_str());
        }
        else if (parsed[1] == "getOrderTarget")
        {
          if (unit->getOrderTarget()==NULL)
          {
            BWAPI::Broodwar->printPublic("NULL");
          }
          else
          {
            BWAPI::Broodwar->printPublic("%s",BWAI::Unit::BWAPIUnitToBWAIUnit(unit)->getOrderTarget()->getName().c_str());
          }
        }
        else if (parsed[1] == "getOrderTimer")
        {
          BWAPI::Broodwar->printPublic("%d",unit->getOrderTimer());
        }
        else if (parsed[1] == "getSecondaryOrder")
        {
          BWAPI::Broodwar->printPublic("%s",unit->getSecondaryOrder().getName().c_str());
        }
        else if (parsed[1] == "getBuildUnit")
        {
          if (unit->getBuildUnit()==NULL)
          {
            BWAPI::Broodwar->printPublic("NULL");
          }
          else
          {
            BWAPI::Broodwar->printPublic("%d: %s",unit->getBuildUnit(), BWAI::Unit::BWAPIUnitToBWAIUnit(unit)->getBuildUnit()->getName().c_str());
          }
        }
        else if (parsed[1] == "getRemainingBuildTime")
        {
          BWAPI::Broodwar->printPublic("%d",unit->getRemainingBuildTime());
        }
        else if (parsed[1] == "getRallyPosition")
        {
          BWAPI::Broodwar->printPublic("(%d,%d)",unit->getRallyPosition().x(),unit->getRallyPosition().y());
        }
        else if (parsed[1] == "getRallyUnit")
        {
          if (unit->getRallyUnit()==NULL)
          {
            BWAPI::Broodwar->printPublic("NULL");
          }
          else
          {
            BWAPI::Broodwar->printPublic("%d: %s",unit->getRallyUnit(),BWAI::Unit::BWAPIUnitToBWAIUnit(unit->getRallyUnit())->getName().c_str());
          }
        }
        else if (parsed[1] == "getAddon")
        {
          if (unit->getAddon()==NULL)
          {
            BWAPI::Broodwar->printPublic("NULL");
          }
          else
          {
            BWAPI::Broodwar->printPublic("%d: %s",unit->getRallyUnit(),BWAI::Unit::BWAPIUnitToBWAIUnit(unit->getAddon())->getName().c_str());
          }
        }
        else if (parsed[1] == "getChild")
        {
          if (unit->getChild()==NULL)
          {
            BWAPI::Broodwar->printPublic("NULL");
          }
          else
          {
            BWAPI::Broodwar->printPublic("%d: %s",unit->getChild(),BWAI::Unit::BWAPIUnitToBWAIUnit(unit)->getChild()->getName().c_str());
          }
        }
        else if (parsed[1] == "getTrainingQueue")
        {
          std::list<BWAPI::UnitType> tqueue=unit->getTrainingQueue();
          if (tqueue.empty())
          {
            BWAPI::Broodwar->printPublic("<empty>");
          }
          else
          {
            for(std::list<BWAPI::UnitType>::iterator t=tqueue.begin();t!=tqueue.end();t++)
            {
              BWAPI::Broodwar->printPublic("%s",(*t).getName().c_str());
            }
          }
        }
        else if (parsed[1] == "getTransport")
        {
          if (unit->getTransport()==NULL)
          {
            BWAPI::Broodwar->printPublic("NULL");
          }
          else
          {
            BWAPI::Broodwar->printPublic("%d: %s",unit->getRallyUnit(),BWAI::Unit::BWAPIUnitToBWAIUnit(unit->getTransport())->getName().c_str());
          }
        }
        else if (parsed[1] == "getLoadedUnits")
        {
          std::list<BWAPI::Unit*> loaded=unit->getLoadedUnits();
          if (loaded.empty())
          {
            BWAPI::Broodwar->printPublic("<empty>");
          }
          else
          {
            for(std::list<BWAPI::Unit*>::iterator t=loaded.begin();t!=loaded.end();t++)
            {
              BWAPI::Broodwar->printPublic("%s",BWAI::Unit::BWAPIUnitToBWAIUnit(*t)->getName().c_str());
            }
          }
        }
        else if (parsed[1] == "getTech")
        {
          BWAPI::Broodwar->printPublic("%s",unit->getTech().getName().c_str());
        }
        else if (parsed[1] == "getUpgrade")
        {
          BWAPI::Broodwar->printPublic("%s",unit->getUpgrade().getName().c_str());
        }
        else if (parsed[1] == "getRemainingResearchTime")
        {
          BWAPI::Broodwar->printPublic("%d",unit->getRemainingResearchTime());
        }
        else if (parsed[1] == "getRemainingUpgradeTime")
        {
          BWAPI::Broodwar->printPublic("%d",unit->getRemainingUpgradeTime());
        }
        else if (parsed[1] == "isAccelerating")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isAccelerating() ? "true" : "false");
        }
        else if (parsed[1] == "isBeingConstructed")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isBeingConstructed() ? "true" : "false");
        }
        else if (parsed[1] == "isBeingHealed")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isBeingHealed() ? "true" : "false");
        }
        else if (parsed[1] == "isBlind")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isBlind() ? "true" : "false");
        }
        else if (parsed[1] == "isBraking")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isBraking() ? "true" : "false");
        }
        else if (parsed[1] == "isBurrowed")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isBurrowed() ? "true" : "false");
        }
        else if (parsed[1] == "isCloaked")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isCloaked() ? "true" : "false");
        }
        else if (parsed[1] == "isCompleted")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isCompleted() ? "true" : "false");
        }
        else if (parsed[1] == "isConstructing")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isConstructing() ? "true" : "false");
        }
        else if (parsed[1] == "isFollowing")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isFollowing() ? "true" : "false");
        }
        else if (parsed[1] == "isLockedDown")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isLockedDown() ? "true" : "false");
        }
        else if (parsed[1] == "isStasised")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isStasised() ? "true" : "false");
        }
        else if (parsed[1] == "isHallucination")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isHallucination() ? "true" : "false");
        }
        else if (parsed[1] == "isIdle")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isIdle() ? "true" : "false");
        }
        else if (parsed[1] == "isLifted")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isLifted() ? "true" : "false");
        }
        else if (parsed[1] == "isLoaded")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isLoaded() ? "true" : "false");
        }
        else if (parsed[1] == "isMorphing")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isMorphing() ? "true" : "false");
        }
        else if (parsed[1] == "isMoving")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isMoving() ? "true" : "false");
        }
        else if (parsed[1] == "isPatrolling")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isPatrolling() ? "true" : "false");
        }
        else if (parsed[1] == "isRepairing")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isRepairing() ? "true" : "false");
        }
        else if (parsed[1] == "isResearching")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isResearching() ? "true" : "false");
        }
        else if (parsed[1] == "isSelected")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isSelected() ? "true" : "false");
        }
        else if (parsed[1] == "isSieged")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isSieged() ? "true" : "false");
        }
        else if (parsed[1] == "isStartingAttack")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isStartingAttack() ? "true" : "false");
        }
        else if (parsed[1] == "isTraining")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isTraining() ? "true" : "false");
        }
        else if (parsed[1] == "isUnderStorm")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isUnderStorm() ? "true" : "false");
        }
        else if (parsed[1] == "isUnpowered")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isUnpowered() ? "true" : "false");
        }
        else if (parsed[1] == "isUpgrading")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isUpgrading() ? "true" : "false");
        }
        else if (parsed[1] == "isVisible")
        {
          BWAPI::Broodwar->printPublic("%s",unit->isVisible() ? "true" : "false");
        }
        else if (parsed[1] == "holdPosition")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->holdPosition();
          }
        }
        else if (parsed[1] == "stop")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->stop();
          }
        }
        else if (parsed[1] == "patrol")
        {
          int x=Util::Strings::stringToInt(std::string(parsed[2]));
          int y=Util::Strings::stringToInt(std::string(parsed[3]));
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->patrol(BWAPI::Position(x,y));
          }
        }
        else if (parsed[1] == "follow")
        {
          if (parsed.size()<3 || parsed[2].length()==0)
          {
            BWAPI::Broodwar->printPublic("Error: No unit specified.");
          }
          else
          {
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              BWAPI::Unit* unit=(BWAPI::Unit*)Util::Strings::stringToInt(std::string(parsed[2]));
              (*u)->follow(unit);
            }
          }
        }
        else if (parsed[1] == "setRallyPosition")
        {
          if (parsed.size()<4 || parsed[3].length()==0)
          {
            BWAPI::Broodwar->printPublic("Error: No position specified.");
          }
          else
          {
            int x=Util::Strings::stringToInt(std::string(parsed[2]));
            int y=Util::Strings::stringToInt(std::string(parsed[3]));
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->setRallyPosition(BWAPI::Position(x,y));
            }
          }
        }
        else if (parsed[1] == "setRallyUnit")
        {
          if (parsed.size()<3 || parsed[2].length()==0)
          {
            BWAPI::Broodwar->printPublic("Error: No unit specified.");
          }
          else
          {
            BWAPI::Unit* unit=(BWAPI::Unit*)Util::Strings::stringToInt(std::string(parsed[2]));
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->setRallyUnit(unit);
            }
          }
        }
        else if (parsed[1] == "repair")
        {
          if (parsed.size()<3 || parsed[2].length()==0)
          {
            BWAPI::Broodwar->printPublic("Error: No unit specified.");
          }
          else
          {
            BWAPI::Unit* unit=(BWAPI::Unit*)Util::Strings::stringToInt(std::string(parsed[2]));
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->repair(unit);
            }
          }
        }
        else if (parsed[1] == "morph")
        {
          if (parsed.size()<3 || parsed[2].length()==0)
          {
            BWAPI::Broodwar->printPublic("Error: No unit specified.");
          }
          else
          {
            BWAPI::UnitType type(Util::Strings::stringToInt(std::string(parsed[2])));
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->morph(type);
            }
          }
        }
        else if (parsed[1] == "burrow")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->burrow();
          }
        }
        else if (parsed[1] == "unburrow")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->unburrow();
          }
        }
        else if (parsed[1] == "siege")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->siege();
          }
        }
        else if (parsed[1] == "unsiege")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->unsiege();
          }
        }
        else if (parsed[1] == "cloak")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->cloak();
          }
        }
        else if (parsed[1] == "decloak")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->decloak();
          }
        }
        else if (parsed[1] == "lift")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->lift();
          }
        }
        else if (parsed[1] == "land")
        {
          int x=Util::Strings::stringToInt(std::string(parsed[2]));
          int y=Util::Strings::stringToInt(std::string(parsed[3]));
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->land(BWAPI::TilePosition(x,y));
          }
        }
        else if (parsed[1] == "load")
        {
          if (parsed.size()<3 || parsed[2].length()==0)
          {
            BWAPI::Broodwar->printPublic("Error: No unit specified.");
          }
          else
          {
            BWAPI::Unit* unit=(BWAPI::Unit*)Util::Strings::stringToInt(std::string(parsed[2]));
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->load(unit);
            }
          }
        }
        else if (parsed[1] == "unload")
        {
          if (parsed.size()<3 || parsed[2].length()==0)
          {
            BWAPI::Broodwar->printPublic("Error: No unit specified.");
          }
          else
          {
            BWAPI::Unit* unit=(BWAPI::Unit*)Util::Strings::stringToInt(std::string(parsed[2]));
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->unload(unit);
            }
          }
        }
        else if (parsed[1] == "unloadAll")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            if (parsed.size()<4 || parsed[2].length()==0)
            {
              (*u)->unloadAll();
            }
            else
            {
              int x=Util::Strings::stringToInt(parsed[2]);
              int y=Util::Strings::stringToInt(parsed[3]);
              (*u)->unloadAll(BWAPI::Position(x,y));
            }
          }
        }
        else if (parsed[1] == "cancelResearch")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->cancelResearch();
          }
        }
        else if (parsed[1] == "cancelUpgrade")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->cancelUpgrade();
          }
        }
        else if (parsed[1] == "cancelConstruction")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->cancelConstruction();
          }
        }
        else if (parsed[1] == "haltConstruction")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->haltConstruction();
          }
        }
        else if (parsed[1] == "cancelMorph")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->cancelMorph();
          }
        }
        else if (parsed[1] == "cancelAddon")
        {
          for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
          {
            (*u)->cancelAddon();
          }
        }
        else if (parsed[1] == "cancelTrain")
        {
          if (parsed.size()<3 || parsed[2].length()==0)
          {
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->cancelTrain();
            }
          }
          else
          {
            int slot=Util::Strings::stringToInt(parsed[2]);
            for(std::set<BWAPI::Unit*>::iterator u=selectedUnits.begin();u!=selectedUnits.end();u++)
            {
              (*u)->cancelTrain(slot);
            }
          }
        }
        else
        {
          BWAPI::Broodwar->printPublic("Unrecognized method call: %s",parsed[1].c_str());
        }
      }
      return true;
    }
    return false;
  }
  //------------------------------------------ CHECK NEW EXPANSION -------------------------------------------
  void AI::checkNewExpansions()
  {
    for each (Unit* i in this->units)
    {
      if (
           i->isCompleted() &&
           (
             i->getType() == BWAPI::UnitTypes::Terran_Command_Center ||
             i->getType() == BWAPI::UnitTypes::Protoss_Nexus ||
             i->getType() == BWAPI::UnitTypes::Zerg_Hatchery
           ) &&
           i->getOwner() == player)
        if (i->getOrder() != BWAPI::Orders::BuildingLiftoff &&
            i->expansion == NULL)
        {
          this->log->logImportant("Starting new expansion - %s", i->getName().c_str());
          this->startNewExpansion(i);
        }
        else if (i->getOrder() == BWAPI::Orders::BuildingLiftoff &&
                 i->expansion != NULL)
          this->removeExpansion(i->expansion);
    }
  }
  //-------------------------------------------- GET IDLE WORKERS --------------------------------------------
  void AI::getIdleWorkers(std::list<Unit*>& workers)
  {
    //if (!this->expansionsSaturated)
      for each (Unit* i in this->units)
      {
        if (i->isCompleted() &&
            i->getOwner() == player &&
            (
               i->getOrder() == BWAPI::Orders::PlayerGuard ||
               i->getOrder() == BWAPI::Orders::MoveToMinerals ||
               i->getOrder() == BWAPI::Orders::WaitForMinerals ||
               i->getOrder() == BWAPI::Orders::MiningMinerals ||
               i->getOrder() == BWAPI::Orders::ResetCollision2 ||
               i->getOrder() == BWAPI::Orders::ReturnMinerals
             ) &&
             !i->isSelected() &&
             i->getType().isWorker() &&
             i->getTask() == NULL)
          workers.push_back(i); 
      }
  }
  //------------------------------------ ASSIGN IDLE WORKERS TO MINERALS -------------------------------------
  void AI::assignIdleWorkersToMinerals(std::list<Unit*>& idleWorkers)
  {
    if (this->activeMinerals.size() > 0)
      for each (Unit* i in idleWorkers)
      {
        TaskGather* best = bestFor(i);
        if (best->executors.size() >= 2)
        {
          this->expansionsSaturated = true;
          break;
        }
        best->addExecutor(i);
     }
  }
  //------------------------------------------- PLANNED SUPPLY GAIN ------------------------------------------
  s32 AI::plannedSupplyGain(BWAPI::Race race)
  {
    s32 returnValue = 0;
    for each (TaskBuild* i in this->plannedBuildings)
      if (i->getBuildingType().getRace() == race)
        returnValue += i->getBuildingType().supplyProvided();
    return returnValue;
  }
  //---------------------------------------------- FREE BUILDER ----------------------------------------------
  Unit* AI::freeBuilder(BWAPI::Position position)
  {
    Unit* best = NULL;
    for each (Unit* i in this->units)
      if (i->getOwner() == player && i->getType().isWorker())
        if (this->betterWorkerToFree(i, best, position))
          best = i;

    if (best == NULL)
      return NULL;

    this->log->logCommon("%s was freed from it's task to do something else", best->getName().c_str());
    best->freeFromTask();
    return best;
  }
  //---------------------------------------------- EXECUTE TASK ----------------------------------------------
  void AI::executeTasks()
  {
    this->update();

    { // ---------- Planned buildings
      std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin();
      while (i != this->plannedBuildings.end())
        if ((*i)->execute())
        {
          if ((*i)->getBuilding() != NULL)
          {
            if ((*i)->getBuildingType() == BWAPI::UnitTypes::Terran_Refinery)
            {
              this->root->log->log("Finished refinery");
              Expansion *expansion = NULL;
              for each (Expansion* j in this->expansions)
                if (j->gatherCenter->getDistance((*i)->getBuilding()) < Expansion::maximumMineralDistance)
                  expansion = j;
              if (expansion != NULL)
                 this->activeRefineries.push_back(new TaskGatherGas((*i)->getBuilding(), expansion));
            }
            {
              for each (TaskTrain* j in this->plannedUnits)
              {
                if (j->getBuildingType() == (*i)->getBuildingType() && //normal building finished
                    (*i)->getBuilding()->getTask() == NULL)
                {
                  j->addExecutor((*i)->getBuilding());
                }
                if (!(*i)->executors.empty() && // bulding is building addon finished
                    (*i)->executors.front()->getType() == j->getBuildingType())
                  j->addExecutor((*i)->executors.front());
              }
            }
          }
          delete *i;
          i = this->plannedBuildings.erase(i);
        }
        else
          ++i;
    }
    { // ---------- Planned invents
      std::list<TaskInvent*>::iterator i = this->plannedInvents.begin();
      while (i != this->plannedInvents.end())
        if ((*i)->execute())
        {
          delete *i;
          i = this->plannedInvents.erase(i);
        }
        else
          ++i;
    }
    
    { // ---------- Planned upgrades
      std::list<TaskUpgrade*>::iterator i = this->plannedUpgrades.begin();
      while (i != this->plannedUpgrades.end())
        if ((*i)->execute())
        {
          delete *i;
          i = this->plannedUpgrades.erase(i);
        }
        else
          ++i;
    }

    { // ---------- Planned units
      std::list<TaskTrain*>::iterator i = this->plannedUnits.begin();
      while (i != this->plannedUnits.end())
      {
        if ((*i)->execute())
        {
          delete *i;
          i = this->plannedUnits.erase(i);
        }
        else
          ++i;
      }
    }
    if (this->activeMinerals.size() > 0)
      for each (TaskGather* i in this->activeMinerals)
        i->execute();
      for each (TaskGatherGas* i in this->activeRefineries)
        i->execute();

	  
    { // ---------- Fights
      std::list<TaskFight*>::iterator i = this->plannedFights.begin();
      while (i != this->plannedFights.end())
      {
        if ((*i)->execute())
        {
          delete *i;
          i = this->plannedFights.erase(i);
        }
        else
          ++i;
      }
    }
  }
  //----------------------------------------------------------------------------------------------------------
  TaskGather* AI::bestFor(Unit* gatherer)
  {
    if (this->activeMinerals.empty())
      return NULL;
    TaskGather* best = activeMinerals.front();
    for each (TaskGather* i in this->activeMinerals)
      if (this->betterMinralPatch(i, best, gatherer))
        best = i;
    return best;
  }
  //----------------------------------------------------------------------------------------------------------
  void AI::removeExpansion(Expansion* expansion)
  {
    for (std::list<Expansion*>::iterator i = this->expansions.begin();
           i != this->expansions.end();
           ++i)
        if (*i == expansion)
        {
          delete expansion;
          this->expansions.erase(i);
          return;
        }
  }

  //---------------------------------------- Get enemy main base's position ----------------------------
  //TODO correctly determine the position of the enemy's main base
  BWAPI::Position AI::getEnemyMain()
  {
    BWAPI::Position position(this->opponent->getStartLocation().x()*4+8,this->opponent->getStartLocation().y()*4+6);

	return position;
  }

  //----------------------------------------------------------------------------------------------------------
  BuildingPosition* AI::getFreeBuildingSpot(std::string spotName, Unit*& builderToUse)
  {
    BuildingPositionSet* position = this->startingPosition->positions[spotName];
    if (position == NULL)
      throw GeneralException("Position '" + spotName + "' not found in the current starting position");
    for each (BuildingPosition* i in position->positions)
      if (!i->reserved)
        {
          int occupiedCount = 0;
          Unit* occupied = NULL;
          for (int k = i->position.x(); 
                k < i->position.x() + position->tileWidth; 
                k++)
            for (int l = i->position.y(); 
                 l < i->position.y() + position->tileHeight; 
                 l++)
              if (!BWAPI::Broodwar->unitsOnTile(k,l).empty())
              {
                occupiedCount ++;
                if (BWAPI::Broodwar->unitsOnTile(k,l).size() == 1)
                  {
                    if (occupied != NULL &&
                      occupied->getUnit() == (*BWAPI::Broodwar->unitsOnTile(k,l).begin()))
                      occupiedCount--;
                    occupied =  BWAI::Unit::BWAPIUnitToBWAIUnit((*BWAPI::Broodwar->unitsOnTile(k,l).begin()));
                  }
                else
                  occupiedCount = 2;
              }
          if (
               occupiedCount == 0 || 
               (
                 occupiedCount == 1 &&
                 occupied->getType().isWorker() &&
                 occupied->getTask() == NULL &&
                 occupied->getOrder() == BWAPI::Orders::Guard
               ) ||
               (
                 occupiedCount == 1 &&
                 occupied->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser
               )
             )
          {
            this->root->log->log("Found free spot for %s at (%d,%d)", spotName.c_str(), i->position.x(), i->position.y());
            if (occupied != NULL &&
                occupied->getType() != BWAPI::UnitTypes::Resource_Vespene_Geyser)
              builderToUse = occupied;
            else
              builderToUse = NULL;
            return i;
          }
        } 
   return NULL;
  }
  //------------------------------------------ GET POSITIONS CALLED ------------------------------------------
  BuildingPositionSet* AI::getPositionsCalled(const std::string& place)
  {
    return this->startingPosition->positions[place];
  }
  //----------------------------------------------------------------------------------------------------------
  void AI::removeGatherGasTask(Unit* refinery)
  {
    for (std::list<TaskGatherGas*>::iterator i = this->activeRefineries.begin();
         i != this->activeRefineries.end();
         ++i)
      if ((*i)->getRefinery() == refinery)
      {
        delete *i;
        this->activeRefineries.erase(i);
        return;
      }
  }
  //----------------------------------------------------------------------------------------------------------
}

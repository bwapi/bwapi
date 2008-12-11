#include "AI.h"

#include <algorithm>
#include <fstream>

#include <Util/Exceptions.h>
#include <Util/FileLogger.h>
#include <Util/Dictionary.h>
#include <Util/Strings.h>
#include <Util/RectangleArray.h>

#include <BW/Unit.h>
#include <BW/UpgradeType.h>
#include <BW/TechType.h>

#include <BWAPI/Unit.h>
#include <BWAPI/Player.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Map.h>

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
#include "Unit.h"
#include "Expansion.h"
#include "MapInfo.h"
#include "MapExpansion.h"
#include "MapStartingPosition.h"
#include "BuildingPositionSet.h"
#include "Formation.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  AI::AI(void)
  :mapInfo(NULL)
  ,startingPosition(NULL)
  ,log    (new Util::FileLogger(config->get("log_path") + "\\ai",   Util::LogLevel::Normal))
  ,deadLog(new Util::FileLogger(config->get("log_path") + "\\dead", Util::LogLevel::MicroDetailed))
  ,root(NULL)
  ,map(NULL)
  ,temp(NULL)
  ,pathFinding(NULL)
  ,mineralGatherers(0)
  ,buildOrderExecutor(NULL)
  {
    this->units.reserve(BW::UNIT_ARRAY_MAX_LENGTH);
    BWAI::ai = this;
    try
    {
      Expansion::maximumMineralDistance = Util::Strings::stringToInt(config->get("max_mineral_distance"));
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Used default value for max_mineral_distance as it couldn't be loaded exception: %s", exception.getMessage().c_str());
    }
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->unitArray[i] = new Unit(BWAPI::Broodwar.getUnit(i));
    
    try
    {
      this->root = new BuildOrder::Root(config->get("build_order_path"));    
      Util::Logger::globalLog->log("Build order loaded");
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Error when loading build order: %s", exception.getMessage().c_str());
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  AI::~AI(void)
  {
    delete root;
  
    for each (TaskBuild* i in this->plannedBuildings)
      delete i;
      
    for each (Expansion* i in this->expansions)
      delete i;
      
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete unitArray[i];

    delete map;

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
  bool AI::betterWorkerToFree(Unit* worker1, Unit* worker2, const BW::Position& buildingPosition)
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
          if (worker2->getOrderID() == BW::OrderID::ReturnMinerals &&
              worker1->getOrderID() == BW::OrderID::MoveToMinerals)
            return true;
          if (worker1->getOrderID() == BW::OrderID::ReturnMinerals &&
              worker2->getOrderID() == BW::OrderID::MoveToMinerals)
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
 
    u16 distance1 = buildingPosition.getDistance(worker1->getPosition());
    u16 distance2 = buildingPosition.getDistance(worker2->getPosition());
 
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
      this->units.clear();
      for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
        this->units.push_back(i);
    
      this->reserved.clear();

      for each (TaskBuild* i in this->plannedBuildings)
        this->reserved += i->getReserved();
    }
    catch (GeneralException& exception)
    {
      this->root->log->log("Exception in AI::update: %s", exception.getMessage().c_str());
    }
  }
  //------------------------------------------------ ON START ------------------------------------------------
  void AI::onStart(BWAPI::Player *player, BWAPI::Player* opponent)
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
      this->map = new BWAPI::Map();
      this->player = player;
      this->opponent = opponent;
      mapInfo = new MapInfo(config->get("maps_path") + "\\" + BWAPI::Map::getName() + ".xml");
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
                        this->startingPosition->expansion->getPosition().x, 
                        this->startingPosition->expansion->getPosition().y);
        }
      }
      this->pathFinding = new PathFinding::Utilities();
    }
    catch (GeneralException& exception)
    {
      delete map;
      map = NULL;
      this->root->log->log("Exception in AI::onStart: %s", exception.getMessage().c_str());
      delete this->mapInfo;
      this->mapInfo = NULL;
    }
    if (this->root != NULL)
    {
      this->buildOrderExecutor = this->root->getStart();
      if (this->buildOrderExecutor == NULL)
        this->root->log->log("Didn't find build order to play with %s against %s", 
                             BW::Race::raceName(this->player->getRace()).c_str(), 
                             BW::Race::raceName(this->opponent->getRace()).c_str());
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
    
    for (unsigned int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      this->unitArray[i]->clearTask();
      this->unitArray[i]->expansion = NULL;
    }
      
    this->startingPosition = NULL;  
    
    delete this->pathFinding;
    this->pathFinding = NULL;
    
    delete this->map;
    this->map = NULL;
    
    delete this->buildOrderExecutor;
    this->buildOrderExecutor = NULL;
    
    this->log->logImportant("Ai::onEnd end\n\n");
  }
  //-----------------------------------------------  ON FRAME ------------------------------------------------
  void AI::onFrame(void)
  {
    try
    {
      if (!BWAPI::Broodwar.enabled)
        return;
      if (BWAPI::Broodwar.frameCount < 2)
        return;
      if (!this->player)
        return;
         
      if (this->buildOrderExecutor)
        this->buildOrderExecutor->execute();

      BW::Unit** selected = BWAPI::Broodwar.saveSelected();    
      this->refreshSelectionStates(selected);
       
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

      if (activeMinerals.size() >0)
      {
        this->assignIdleWorkersToMinerals(idleWorkers);
        this->rebalanceMiners();
        this->checkAssignedWorkers();
      }
      this->performAutoBuild();
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
      BWAPI::Broodwar.loadSelected(selected);
    }
    catch (GeneralException& e)
    {
      this->log->logCritical("Exception caught in AI::onFrame: %s", e.getMessage().c_str());
    }
  }
  //------------------------------------------------ GET UNIT ------------------------------------------------
  Unit* AI::getUnit(int index)
  {
    return unitArray[index];
  }
  //-------------------------------------------- ON CANCEL TRAIN ---------------------------------------------
  void AI::onCancelTrain()
  {
    /*
    this->log->log("Cancelled unit caught");
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    */
  }
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
                       gatherer->getIndex(), 
                       worst->getMineral()->getIndex(), 
                       best->getMineral()->getIndex());

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
  
  //----------------------------------------------- GET FIRST ------------------------------------------------
  Unit* AI::getFirst()
  {
    return Unit::BWUnitToBWAIUnit(*BW::BWDATA_UnitNodeTable_FirstElement);
  }
  //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
  void AI::onRemoveUnit(BW::Unit* unit)
  {
    Unit* dead = BWAI::Unit::BWUnitToBWAIUnit(unit);
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
    else if (dead->getType() == BW::UnitID::Terran_Refinery)
    {
      if (dead->expansion != NULL)
        this->removeGatherGasTask(dead);
    }
    else if (
              (
               dead->getType() == BW::UnitID::Terran_CommandCenter ||
               dead->getType() == BW::UnitID::Protoss_Nexus ||
               dead->getType() == BW::UnitID::Zerg_Hatchery
              ) &&
              dead->expansion != NULL
            )
      this->removeExpansion(dead->expansion);
    if (dead->getTask() != NULL)
      dead->freeFromTask();
    dead->expansion = NULL;
    this->deadLog->log("AI::onRemoveUnit end", dead->getName().c_str());
   }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  bool AI::onSendText(const char* text)
  {
    std::string message = text;
    std::vector<std::string> parsed = Util::Strings::splitString(message, " ");
    parsed.push_back("");parsed.push_back("");parsed.push_back(""); // to avoid range checks everywhere
    if (parsed[0] == "/save")
    {
      if (parsed[1] == "fog")
      {
        this->map->saveFogOfWarMap(config->get("data_path") + "\\fog-of-war.txt", 
                                   this->player->getID());
        BWAPI::Broodwar.print("fog of war saved to fo 'fog-of-war.txt'");
      }
      else if (parsed[1] == "techs")
      {
        std::string fileName = config->get("data_path") + "\\techs";
        Util::FileLogger techsLog(fileName, Util::LogLevel::MicroDetailed, false);
        for (int i = 0; i < BW::TECH_TYPE_COUNT; i++)
          if (BW::TechType((BW::TechID::Enum)i).isValid())
            techsLog.log("%s = 0x%02X",BW::TechType((BW::TechID::Enum)i).getName(), i);
        BWAPI::Broodwar.print("Techs saved to %s .ini", fileName.c_str());
      }
      else if (parsed[1] == "upgrades")
      {
        std::string fileName = config->get("data_path") + "\\upgrades";
        Util::FileLogger upgradesLog(fileName, Util::LogLevel::MicroDetailed, false);
        for (u8 i = 0; i < BW::UPGRADE_TYPE_COUNT; i++)
        {
          BW::UpgradeType upgrade = BW::UpgradeType((BW::UpgradeID::Enum)i);
          if (upgrade.isValid())
            upgradesLog.log("%s = 0x%02X",upgrade.getName(), i);
        }
        BWAPI::Broodwar.print("Upgrades saved to %s .ini", fileName.c_str());
      }
      else if (parsed[1] == "units")
      {
        std::string fileName = config->get("data_path") + "\\units";
        Util::FileLogger upgradesLog(fileName, Util::LogLevel::MicroDetailed, false);
        for (u8 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        {
          BW::UnitType unit = BW::UnitType((BW::UnitID::Enum)i);
            upgradesLog.log("%s = 0x%02X",unit.getName(), i);
        }
        BWAPI::Broodwar.print("Units saved to %s .ini", fileName.c_str());        
      }      
      else if (parsed[1] == "buildability")
        this->map->saveBuildabilityMap(config->get("data_path") + "\\buildability.txt");
      else if (parsed[1] == "walkability")
        this->map->saveWalkabilityMap(config->get("data_path") + "\\walkability.txt");
      else if (parsed[1] == "defined" && parsed[2] == "buildings")
      {
        if (this->mapInfo != NULL)
        {
          std::string fileName = config->get("data_path") + "\\pre-defined-buildings.txt";
          mapInfo->saveDefinedBuildingsMap(fileName);
          BWAPI::Broodwar.print("Defined buildings saved to %s", fileName.c_str());
        }
        else
          BWAPI::Broodwar.print("Map info for the current map is not available.");
      }
      else if (parsed[1] == "position")     // ------------------ Save Position
      {
        std::string path = config->get("maps_path") + "\\" + BWAPI::Map::getName() + "-autosave.xml";
        if (this->saveBuildings(path))
          BWAPI::Broodwar.print("Saved current build positions to %s.", path.c_str());
        else
          BWAPI::Broodwar.print("Failed to write %s.", path.c_str());
      }
      else
        BWAPI::Broodwar.print("Unknown command '%s' - possible commands are: fog, techs, upgrades, units, "
                              "buildability, walkability, defined buildings, position", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/tech")
    {
      if (parsed[1] == "add")
      {
        std::string techName = message.substr(strlen("/tech add "), message.size() - strlen("/tech add "));
        BW::TechType tech = BWAPI::Broodwar.techNameToType[techName];
        if (tech == BW::TechID::None)
          BWAPI::Broodwar.print("Unknown upgrade name '%s'", techName);
        else
        {
          if (this->player->canAfford(tech, this->reserved))
          {
            this->plannedInvents.push_back(new BWAI::TaskInvent(tech, 0));
            BWAPI::Broodwar.print("Added tech '%s'", techName.c_str());
          }
          else
            BWAPI::Broodwar.print("Cant afford the tech right now -> Try again later");
        }
        return true;
      }
      else if (parsed[1] == "list")
      {
        for each (TaskInvent* i in this->plannedInvents)
          BWAPI::Broodwar.print(i->getTechType().getName());
      }
      else 
        BWAPI::Broodwar.print("Unknown command '%s' - possible commands are: add, list", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/upgrade")
    {
      if (parsed[1] == "add")
      {
        std::string upgradeName = message.substr(strlen("/upgrade add "), message.size() - strlen("/upgrade add "));
        BW::UpgradeType upgrade = BWAPI::Broodwar.upgradeNameToType[upgradeName];
        if (upgrade == BW::UpgradeID::None)
          BWAPI::Broodwar.print("Unknown upgrade name '%s'", upgradeName);
        else
        {
          if (this->player->canAfford(upgrade, this->player->upgradeLevel(upgrade) + 1, this->reserved))
          {
            this->plannedUpgrades.push_back(new BWAI::TaskUpgrade(upgrade, this->player->upgradeLevel(upgrade) + 1, 0));
            BWAPI::Broodwar.print("Added upgrade '%s' level %d", upgradeName, this->player->upgradeLevel(upgrade) + 1);
          }
          else
            BWAPI::Broodwar.print("Cant afford the upgrade right now -> Try again later");
        }
        return true;
      }
      else if (parsed[1] == "list")
      {
        for each (TaskUpgrade* i in this->plannedUpgrades)
          BWAPI::Broodwar.print(i->getUpgradeType().getName());
      }
      else 
        BWAPI::Broodwar.print("Unknown command '%s' - possible commands are: add, list", parsed[1].c_str());
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
            if (i->getType() == BW::UnitID::Terran_Marine &&
                i->getTask() == NULL)
            {
              addedCount++;
              task->addExecutor(i);
            }
          BWAPI::Broodwar.print("%u units added to the fight group", addedCount);
        }
        else
        {
          if (this->fightGroups.empty())
            this->fightGroups.push_back(new TaskFight());
          TaskFight* task = this->fightGroups.front();
          BW::Unit ** selected = BWAPI::Broodwar.saveSelected();
          for (int i = 0; selected[i] != NULL; i++)
          {
            Unit* unit = BWAI::Unit::BWUnitToBWAIUnit(selected[i]);
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
          BW::Unit ** selected = BWAPI::Broodwar.saveSelected();
          for (int i = 0; selected[i] != NULL; i++)
          {
            Unit* unit = BWAI::Unit::BWUnitToBWAIUnit(selected[i]);
            unit->freeFromTask();
          }
        }
      }
      else 
        BWAPI::Broodwar.print("Unknown add command '%s' - possible values are: add, add all, remove, remove all", parsed[1].c_str());
      return true;
    } 
    else if (parsed[0] == "/formation")
    {
      BW::Position position = BW::Position(BWAPI::Broodwar.getMouseX() + BWAPI::Broodwar.getScreenX(),
                                           BWAPI::Broodwar.getMouseY() + BWAPI::Broodwar.getScreenY());
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
          BWAPI::Broodwar.print("Invalid angle '%s'", parsed[1]);
          true;
        }
        if (this->fightGroups.empty())
          return true;        
        TaskFight* task = this->fightGroups.front();
        Formation* formation = new Formation(task->executors);
        formation->generatePositions(position, (float)(angle*(3.141)/180));
        formation->execute();
        delete formation;
      }
    }
    else if (parsed[0] == "/reload")
    {
      if (parsed[1] == "map")
      {
        try
        {
          mapInfo = new MapInfo(config->get("maps_path") + "\\" + BWAPI::Map::getName() + ".xml");
          BWAPI::Broodwar.print("Map data reloaded successfully.");
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
          BWAPI::Broodwar.print("Build order reloaded successfully.");
        }
        catch (GeneralException& exception)
        {
          Util::Logger::globalLog->log("Error when loading build order: %s", exception.getMessage().c_str());
          return true;
        }
      }
      else
        BWAPI::Broodwar.print("Unknown reload command '%s' - possible values are: map, bo", parsed[1].c_str());
      return true;
    }
    return false;
  }
  //--------------------------------------------- SAVE BUILDINGS ---------------------------------------------
  // Re-writing to add positions if they are not found. Needs some thought.
  bool AI::saveBuildings(const std::string& path)
  {
    std::string learnBuildStream, learnBuildTest;
    char learnBuildTemp[256];
    u32 learnBuildPos, learnBuildLoc;

    char* bxmlHead = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
    char* bMapDescFull = "<map-description xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                         "	               xsi:noNamespaceSchemaLocation=\"map-info.xsd\">\n";
    char* bPosition = "position";
    char* bStartPositions = "starting-positions";
    char* bStartPosition = "starting-position";
    char* bStandardBuildingPlace = "standard-building-placement";

    learnBuildStream = " ";

    std::ifstream fin;
    std::ofstream fout;

    fin.open(path.c_str(), std::ios::in);
    if (fin.is_open())
    {
      fin.seekg(0);
      while(!fin.eof())
      {
        fin.getline(learnBuildTemp, 255);
        learnBuildStream.append(learnBuildTemp);
        learnBuildStream.append("\n");
      }
      fin.close();
    }
    // ######################## XML HEADER
    learnBuildPos = 0;
    learnBuildLoc = learnBuildStream.find("<?xml ");
    if (learnBuildLoc == std::string::npos)
    {
      learnBuildStream.insert(learnBuildPos, bxmlHead);
      learnBuildPos += strlen(bxmlHead);
    }
    else
      learnBuildPos = learnBuildStream.find(">", learnBuildLoc) + 1;

    // ######################## MAP-DESCRIPTION ELEMENT
    learnBuildLoc = learnBuildStream.find("map-description", learnBuildPos);
    if (learnBuildLoc == std::string::npos)
    {
      learnBuildStream.insert(learnBuildPos, bMapDescFull);
      learnBuildPos += strlen(bMapDescFull);
      learnBuildStream.insert(learnBuildPos, "</map-description>\n");
    }
    else
      learnBuildPos = learnBuildStream.find(">", learnBuildLoc) + 1;

    // ######################## EXPANSIONS ELEMENT
    learnBuildLoc = learnBuildStream.find("expansions>", learnBuildPos);
    if (learnBuildLoc == std::string::npos)
    {
      learnBuildStream.insert(learnBuildPos, "\n  <expansions>\n");
      learnBuildPos += 16;
      learnBuildStream.insert(learnBuildPos, "  </expansions>\n");
    }
    else
      learnBuildPos = learnBuildStream.find(">", learnBuildLoc) + 1;

    // ######################## EXPANSION ELEMENTS
    for (u8 i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
    {
      if (BW::startPositions[i].y != 0 && BW::startPositions[i].x != 0)
      {
         learnBuildTest = "";
         learnBuildTest.append("<expansion id=\"Player ");
         learnBuildTest.append(_itoa(i+1, learnBuildTemp, 10));
         learnBuildTest.append("\">");

         learnBuildLoc = learnBuildStream.find(learnBuildTest, learnBuildPos);
         if (learnBuildLoc == std::string::npos)
         {
           learnBuildStream.insert(learnBuildPos, "\n    ");
           learnBuildPos += 5;
           learnBuildStream.insert(learnBuildPos, learnBuildTest);
           learnBuildPos += learnBuildTest.size();
           learnBuildStream.insert(learnBuildPos, "\n");
           learnBuildPos += 1;
           learnBuildStream.insert(learnBuildPos, "      <position x=\"");
           learnBuildPos += 19;
           learnBuildStream.insert(learnBuildPos, _itoa(BW::startPositions[i].x, learnBuildTemp, 10));
           learnBuildPos += strlen(_itoa(BW::startPositions[i].x, learnBuildTemp, 10));
           learnBuildStream.insert(learnBuildPos, "\" y=\"");
           learnBuildPos += 5;
           learnBuildStream.insert(learnBuildPos, _itoa(BW::startPositions[i].y, learnBuildTemp, 10));
           learnBuildPos += strlen(_itoa(BW::startPositions[i].y, learnBuildTemp, 10));
           learnBuildStream.insert(learnBuildPos, "\"/>\n");
           learnBuildPos += 4;
           learnBuildStream.insert(learnBuildPos, "    </expansion>\n");
           learnBuildPos += 17;
         }
         else
         {
           learnBuildPos = learnBuildStream.find(">", learnBuildLoc) + 1;

           // ######################## POSITION ELEMENT
           learnBuildTest = "";
           learnBuildTest.append("<position x=\"");
           learnBuildTest.append(_itoa(BW::startPositions[i].x, learnBuildTemp, 10));
           learnBuildTest.append("\" y=\"");
           learnBuildTest.append(_itoa(BW::startPositions[i].y, learnBuildTemp, 10));
           learnBuildTest.append("\"/>");

           learnBuildLoc = learnBuildStream.find(learnBuildTest, learnBuildPos);
           if (learnBuildLoc == std::string::npos)
           {
             learnBuildStream.insert(learnBuildPos, "\n      ");
             learnBuildPos += 7;
             learnBuildStream.insert(learnBuildPos, learnBuildTest);
             learnBuildPos += learnBuildTest.size();
             learnBuildStream.insert(learnBuildPos, "\n");
             learnBuildPos += 1;
           }
           learnBuildPos = learnBuildStream.find("</expansion>", learnBuildLoc) + 13;
           
         }
       }
    }

    // @todo: <starting-positions> + <starting-position  + <standard-building-placement> + <build-position  + positions

    fout.open(path.c_str(), std::ios::out);
    if (!fout.is_open())
      return false;
    fout.seekp(0);
    fout << learnBuildStream;
    fout.close();
    return true;
                                                        /* Old shit
   FILE* f = fopen(path.c_str(), "wt");
    if (!f)
      throw FileException("Could not open " + BWAPI::Map::getName() + " for writing.");
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n"
               "<map-description xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
               "        xsi:noNamespaceSchemaLocation=\"map-info.xsd\">\n"
               "<expansions>\n");
    for (u8 i = 0; i < BW::PLAYABLE_PLAYER_COUNT ; i++)                   // get starting positions
          fprintf(f, "    <expansion id=\"Player  %d\">\n"
                     "      <position x=\"%d\" y=\"%d\"/>\n"
                     "    </expansion>\n", i, BW::startPositions[i].x, BW::startPositions[i].y);
    fprintf(f, "  </expansions>\n"
               "  <starting-positions>\n");
 
    for (u8 i2 = 0; i2 < BW::PLAYABLE_PLAYER_COUNT; i2++)
    {
      fprintf(f, "    <starting-position expansion-id=\"Player %d\">\n"
                 "      <standard-building-placement>\n"
                 "        <build-position name=\"expansion\" width=\"4\" height=\"3\">\n", i2);
      for each (Unit* i in this->units)                   // get expansion
        if ((i->getType() == BW::UnitID::Protoss_Nexus ||
             i->getType() == BW::UnitID::Zerg_Hatchery ||
             i->getType() == BW::UnitID::Terran_CommandCenter) &&
             i->getOwner() == BWAPI::Broodwar.players[i2] &&
             (i->getPosition().x != BW::startPositions[i2].x ||
              i->getPosition().y != BW::startPositions[i2].y))
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                 "        <build-position name=\"non-producting-3X2\" width=\"3\" height=\"2\">\n", i2);
      for each (Unit* i in this->units)                   // get non-producing 3x2
        if ((i->getType() == BW::UnitID::Terran_SupplyDepot ||
             i->getType() == BW::UnitID::Terran_Academy ||
             i->getType() == BW::UnitID::Terran_Armory ||
             i->getType() == BW::UnitID::Protoss_ArbiterTribunal ||
             i->getType() == BW::UnitID::Protoss_CitadelOfAdun ||
             i->getType() == BW::UnitID::Protoss_CyberneticsCore ||
             i->getType() == BW::UnitID::Protoss_FleetBeacon ||
             i->getType() == BW::UnitID::Protoss_Forge ||
             i->getType() == BW::UnitID::Protoss_Observatory ||
             i->getType() == BW::UnitID::Protoss_RoboticsSupportBay ||
             i->getType() == BW::UnitID::Protoss_TemplarArchives) &&
             i->getOwner() == BWAPI::Broodwar.players[i2])
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                 "        <build-position name=\"barracks\" width=\"4\" height=\"3\" shortcut=\"BB\">\n");
      for each (Unit* i in this->units)                   // get barracks
        if ((i->getType() == BW::UnitID::Terran_Barracks ||
             i->getType() == BW::UnitID::Protoss_Gateway) &&
             i->getOwner() == BWAPI::Broodwar.players[i2])
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                 "        <build-position name=\"engineering-bay\" width=\"4\" height=\"3\" shortcut=\"BE\">\n");
      for each (Unit* i in this->units)                   // get engineering-bay
        if (i->getType() == BW::UnitID::Terran_EngineeringBay &&
            i->getOwner() == BWAPI::Broodwar.players[i2])
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                  "        <build-position name=\"refinery\" width=\"4\" height=\"2\" shortcut=\"BR\">\n");
      for each (Unit* i in this->units)                   // get refinery
        if ((i->getType() == BW::UnitID::Terran_Refinery ||
             i->getType() == BW::UnitID::Protoss_Assimilator ||
             i->getType() == BW::UnitID::Zerg_Extractor) &&
             i->getOwner() == BWAPI::Broodwar.players[i2])
        {
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
          break;
        }
      fprintf(f, "        </build-position>\n"
                 "        <build-position name=\"turret\" width=\"2\" height=\"2\">\n");
      for each (Unit* i in this->units)                   // get turret
        if (i->getType().isBuilding() &&
            i->getType().canAttack() &&
            i->getOwner() == BWAPI::Broodwar.players[i2])
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                 "        <build-position name=\"bunker\" width=\"2\" height=\"2\">\n");
      for each (Unit* i in this->units)                   // get bunker
        if ((i->getType() == BW::UnitID::Terran_Bunker ||
             i->getType() == BW::UnitID::Protoss_ShieldBattery) &&
             i->getOwner() == BWAPI::Broodwar.players[i2])
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                 "        <build-position name=\"pylon\" width=\"2\" height=\"2\" shortcut=\"PP\">\n");
      for each (Unit* i in this->units)                   // get Pylon
        if (i->getType() == BW::UnitID::Protoss_Pylon &&
            i->getOwner() == BWAPI::Broodwar.players[i2])
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                 "        <build-position name=\"building-with-addon\" width=\"4\" height=\"3\" shortcut=\"A+\">\n");
      for each (Unit* i in this->units)                   // get building with addon
        if ((i->getType() == BW::UnitID::Terran_Factory ||
             i->getType() == BW::UnitID::Terran_Starport ||
             i->getType() == BW::UnitID::Terran_ScienceFacility) &&
             i->getOwner() == BWAPI::Broodwar.players[i2])
          fprintf(f, "          <position x=\"%d\" y=\"%d\"/>\n", i->getTilePosition().x, i->getTilePosition().y);
      fprintf(f, "        </build-position>\n"
                 "      </standard-building-placement>\n"
                 "    </starting-position>\n");
    }
    fprintf(f, "  </starting-positions>\n"
                 "</map-description>");
    fclose(f);*/
  } 
  //------------------------------------------ CHECK NEW EXPANSION -------------------------------------------
  void AI::checkNewExpansions()
  {
    for each (Unit* i in this->units)
    {
      if (
           i->isReady() &&
           (
             i->getType() == BW::UnitID::Terran_CommandCenter ||
             i->getType() == BW::UnitID::Protoss_Nexus ||
             i->getType() == BW::UnitID::Zerg_Hatchery
           ) &&
           i->getOwner() == player)
        if (i->getOrderID() != BW::OrderID::BuildingLiftoff &&
            i->expansion == NULL)
        {
          this->log->logImportant("Starting new expansion - %s", i->getName().c_str());
          this->startNewExpansion(i);
        }
        else if (i->getOrderID() == BW::OrderID::BuildingLiftoff &&
                 i->expansion != NULL)
          this->removeExpansion(i->expansion);
    }
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void AI::refreshSelectionStates(BW::Unit** selected)
  {
    for each (Unit* i in this->units)
      i->selected = false;
    for (int i = 0; selected[i] != NULL; i++)
      BWAI::Unit::BWUnitToBWAIUnit(selected[i])->selected = true;
  }
  //------------------------------------------- PERFRORM AUTOBUILD -------------------------------------------
  void AI::performAutoBuild()
  {
    /** 
     * Just workaround, all buildings started by user will register as Task Build
     * Reasons:
     * 1) Finished refinery will then register TaskGatherGas
     * 2) If the scv gets killed it will automaticaly send new one
     * 3) Will be counted in the check supply function into planned supplies
     */
    for each (Unit* i in this->units)
      if (i->isReady() &&
          i->getOwner() == player &&
          i->getType().isWorker() &&
          i->getTask() == NULL &&
          i->getOrderID() == BW::OrderID::ConstructingBuilding &&
          i->getOrderTarget() != NULL)
       {
         this->root->log->log("Custom building added buildTask");         
         this->plannedBuildings.push_back(new TaskBuild(i->getOrderTarget()->getType(), NULL, i, NULL, 0));
       }
  }
  //-------------------------------------------- GET IDLE WORKERS --------------------------------------------
  void AI::getIdleWorkers(std::list<Unit*>& workers)
  {
    //if (!this->expansionsSaturated)
      for each (Unit* i in this->units)
      {
        if (i->isReady() &&
            i->getOwner() == player &&
              (
               i->getOrderIDLocal() == BW::OrderID::PlayerGuard ||
               i->getOrderIDLocal() == BW::OrderID::MoveToMinerals ||
               i->getOrderIDLocal() == BW::OrderID::HarvestMinerals2 ||
               i->getOrderIDLocal() == BW::OrderID::MiningMinerals ||
               i->getOrderIDLocal() == BW::OrderID::ResetCollision2 ||
               i->getOrderIDLocal() == BW::OrderID::ReturnMinerals
             ) &&
             !i->selected &&
             (i->getType().isWorker()) &&
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
  s32 AI::plannedSupplyGain(BW::Race::Enum race)
  {
    s32 returnValue = 0;
    for each (TaskBuild* i in this->plannedBuildings)
      if (i->getBuildingType().getRace() == race)
        returnValue += i->getBuildingType().getSupplyProduced();
    return returnValue;
  }
  //---------------------------------------------- FREE BUILDER ----------------------------------------------
  Unit* AI::freeBuilder(BW::Position position)
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
    { // ---------- Planned buildings
      std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin();
      while (i != this->plannedBuildings.end())
        if ((*i)->execute())
        {
          if ((*i)->getBuilding() != NULL)
          {
            if ((*i)->getBuildingType() == BW::UnitID::Terran_Refinery)
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
                  j->addExecutor((*i)->getBuilding());
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
        if ((*i)->execute())
        {
          delete *i;
          i = this->plannedUnits.erase(i);
        }
        else
          ++i;
    }
    if (this->activeMinerals.size() > 0)
      for each (TaskGather* i in this->activeMinerals)
        i->execute();
      for each (TaskGatherGas* i in this->activeRefineries)
        i->execute();
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
          for (int k = i->position.x; 
                k < i->position.x + position->tileWidth; 
                k++)
            for (int l = i->position.y; 
                 l < i->position.y + position->tileHeight; 
                 l++)
              if (!BWAPI::Broodwar.unitsOnTile[k][l].empty())
              {
                occupiedCount ++;
                if (BWAPI::Broodwar.unitsOnTile[k][l].size() == 1)
                  {
                    if (occupied != NULL &&
                        occupied->getIndex() == BWAPI::Broodwar.unitsOnTile[k][l].front()->getIndex())
                      occupiedCount--;
                    occupied =  BWAI::Unit::BWAPIUnitToBWAIUnit(BWAPI::Broodwar.unitsOnTile[k][l].front());
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
                 occupied->getOrderID() == BW::OrderID::Guard
               ) ||
               (
                 occupiedCount == 1 &&
                 occupied->getType() == BW::UnitID::Resource_VespeneGeyser
               )
             )
          {
            this->root->log->log("Found free spot for %s at (%d,%d)", spotName.c_str(), i->position.x, i->position.y);
            if (occupied != NULL &&
                occupied->getType() != BW::UnitID::Resource_VespeneGeyser)
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

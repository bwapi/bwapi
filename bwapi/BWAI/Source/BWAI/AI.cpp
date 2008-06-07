#include "AI.h"

#include <algorithm>

#include <Util/Exceptions.h>
#include <Util/Logger.h>
#include <Util/Dictionary.h>
#include <Util/Strings.h>
#include <Util/RectangleArray.h>

#include <BW/Unit.h>

#include <BWAPI/Unit.h>
#include <BWAPI/Player.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Map.h>

#include <BuildOrder/Root.h>
#include <BuildOrder/Branch.h>
#include <BuildOrder/Command.h>
#include <BuildOrder/BuildWeights.h>

#include <Pathfinding/Utilities.h>

#include "Task.h"
#include "TaskGatherGas.h"
#include "TaskGather.h"
#include "TaskBuild.h"
#include "Unit.h"
#include "Expansion.h"
#include "MapInfo.h"
#include "MapExpansion.h"
#include "MapStartingPosition.h"
#include "BuildingPosition.h"

namespace BWAI
{
  //------------------------------- CONSTRUCTOR -------------------------------
  AI::AI(void)
  :mapInfo(NULL)
  ,startingPosition(NULL)
  ,log    (new Util::Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\ai",   Util::LogLevel::Normal))
  ,deadLog(new Util::Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\dead", Util::LogLevel::MicroDetailed))
  ,root(NULL)
  ,moneyToBeSpentOnBuildings(0)
  ,map(NULL)
  ,temp(NULL)
  ,pathFinding(NULL)
  {
    try
    {
      Expansion::maximumMineralDistance = Util::Strings::stringToInt(BWAPI::Broodwar.configuration->getValue("max_mineral_distance"));
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Used default value for max_mineral_distance as it couldn't be loaded exception: " + exception.getMessage());
    }
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->units[i] = new Unit(BWAPI::Broodwar.getUnit(i));
    this->first = NULL;
    
    try
    {
      this->root = new BuildOrder::Root(BWAPI::Broodwar.configuration->getValue("build_order_path"));    
      Util::Logger::globalLog->log("Build order loaded");
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Error when loading build order: " + exception.getMessage());
    }
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  AI::~AI(void)
  {
    delete root;
  
    for (std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin(); i != this->plannedBuildings.end(); ++i)
      delete *i;
      
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      delete *i;
      
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete units[i];

    delete map;

    delete this->log;
    delete deadLog;
  }
  //----------------------------- MINERAL VALUE -------------------------------
  bool mineralValue(BWAI::TaskGather*& task1, BWAI::TaskGather*& task2)
  {
    if (task1->executors.size() < task2->executors.size())
      return true;
    if (task1->executors.size() > task2->executors.size())
      return false;
    if (AI::optimizeMineralFor != NULL &&
        task1->getExpansion()->gatherCenter != task2->getExpansion()->gatherCenter)
    {
      u16 distance1 = AI::optimizeMineralFor->getCenterDistance(task1->getExpansion()->gatherCenter);
      u16 distance2 = AI::optimizeMineralFor->getCenterDistance(task2->getExpansion()->gatherCenter);
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
  //--------------------------- BETTER WORKER TO FREE -------------------------
  bool betterWorkerToFree(Unit*& worker1, Unit*& worker2, BW::Position buildingPosition)
  {
    if (worker2 == NULL)
      return true;
   
    if (worker1->getTask() == NULL && worker2->getTask() != NULL)
      return true;
    if (worker1->getTask() != NULL && worker2->getTask() == NULL)
      return false;
    
    if (worker1->getTask() != NULL)
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
            ((TaskGather*)worker1->getTask())->getExpansion())
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
  //------------------------------- UPDATE ------------------------------------
  void AI::update(void)
  {
    this->first = Unit::BWUnitToBWAIUnit(*BW::BWXFN_UnitNodeTable_FirstElement);
    if (this->first != NULL)
      this->first->updateNext();
    this->moneyToBeSpentOnBuildings = 0;

    for (std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin();
         i != this->plannedBuildings.end();
         ++i)
    {
      if ((*i)->getBuilding() == NULL)
        this->moneyToBeSpentOnBuildings += (*i)->getBuildingType().getMineralPrice();
    }
  }
  //------------------------------ ON START -----------------------------------
  void AI::onStart(BWAPI::Player *player)
  {
    try
    {
      this->log->log("Ai::onStart start", Util::LogLevel::Important);    
      this->map = new BWAPI::Map();
      if (this->unitNameToType.empty())
        for (int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
          this->unitNameToType.insert(std::pair<std::string, BW::UnitType>
                                       (
                                         BW::UnitType( (BW::UnitID::Enum) i ).getName(), 
                                         BW::UnitType( (BW::UnitID::Enum) i)
                                       )
                                     ); 
      this->player = player;
      this->map->saveBuildabilityMap(BWAPI::Broodwar.configuration->getValue("data_path") + "\\buildability.txt");
      this->map->saveWalkabilityMap(BWAPI::Broodwar.configuration->getValue("data_path") + "\\walkability.txt");
      std::string mapNameAbsolute = BWAPI::Map::getFileName();
      size_t lastDelimiterPos = mapNameAbsolute.rfind('\\');
      std::string mapName = mapNameAbsolute.substr(lastDelimiterPos + 1, mapNameAbsolute.size() - lastDelimiterPos - 1);
      mapInfo = new MapInfo(BWAPI::Broodwar.configuration->getValue("maps_path") + "\\" + mapName + ".xml");
      this->checkNewExpansions();
      
      this->log->log("Help pre-prepared information found for the curent map");
      
      if (this->expansions.size())
      {
        for (std::list<MapStartingPosition*>::iterator i = mapInfo->startingPositions.begin(); i != mapInfo->startingPositions.end(); ++i)
         if (this->expansions.front()->gatherCenter->getDistance((*i)->expansion->getPosition()) < 100)
          {
            this->startingPosition = *i;
            break;
          }
        if (this->startingPosition)
        {
         this->log->log("Starting position is (%s) at (%d, %d)", this->startingPosition->expansion->getID().c_str(), 
                                                                 this->startingPosition->expansion->getPosition().x, 
                                                                 this->startingPosition->expansion->getPosition().y);
        }
      }
      mapInfo->saveDefinedBuildingsMap();
      //this->pathFinding = new PathFinding::Utilities();
      this->pathFinding = NULL;

      /** For testing reasons, there is just one build order now*/
    }
    catch (GeneralException& exception)
    {
      delete map;
      map = NULL;
      this->log->log("Exception in AI::onStart: " + exception.getMessage());
      delete this->mapInfo;
      this->mapInfo = NULL;
    }

    if (this->root != NULL)
    {
      this->actualBranch = this->root->buildOrders.front();
      this->actualPosition = this->actualBranch->commands.begin();
    }
    this->log->log("Ai::onStart end", Util::LogLevel::Important);      
  }
  //--------------------------------- ON END ---------------------------------
  void AI::onEnd()
  {
    this->log->log("Ai::onEnd start", Util::LogLevel::Important);
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      delete *i;
    this->expansions.clear();
    this->activeMinerals.clear();
    this->expansionsSaturated = false;
    
    delete this->mapInfo;
    this->mapInfo = NULL;
    
    for (std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin(); i != this->plannedBuildings.end(); ++i)
      delete *i;
    this->plannedBuildings.clear();
    
    for (std::list<TaskGatherGas*>::iterator i = this->activeRefineries.begin(); i != this->activeRefineries.end(); ++i)
      delete *i;
    this->activeRefineries.clear();
    
    for (unsigned int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      this->units[i]->clearTask();
      this->units[i]->expansion = NULL;
      this->units[i]->lastTrainedUnit = BW::UnitID::None;
    }
      
    this->startingPosition = NULL;  
    
    delete this->pathFinding;
    this->pathFinding = NULL;
    
    delete this->map;
    this->map = NULL;
    
    this->log->log("Ai::onEnd end", Util::LogLevel::Detailed);      
  }
  //-------------------------------  ON FRAME ---------------------------------
  void AI::onFrame(void)
  {
    if (BWAPI::Broodwar.frameCount < 2)
      return;
    if (!this->player)
      return;
       
    if (root != NULL &&
        this->actualPosition != this->actualBranch->commands.end())
      if ((*this->actualPosition)->execute())
        ++this->actualPosition;

    BW::Unit** selected = BWAPI::Broodwar.saveSelected();    
    this->refreshSelectionStates(selected);
     
    this->checkSupplyNeed();
    this->checkNewExpansions();

    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (!i->hasEmptyBuildQueueLocal())
        i->lastTrainedUnit = i->getBuildQueueLocal()[i->getBuildQueueSlotLocal()];

    std::list<Unit*> idleWorkers;
    this->getIdleWorkers(idleWorkers);
    #pragma region DisabledPathFindingPerformanceTest
    if (!this->expansions.empty())
      if (this->expansions.front()->gatherCenter->getRawData()->currentBuildUnit != NULL)
        this->log->log("Currently built progress = %u unit = %s", this->expansions.front()->gatherCenter->getRawData()->currentBuildUnit->remainingBuildTime, BWAI::Unit::BWUnitToBWAIUnit(this->expansions.front()->gatherCenter->getRawData()->currentBuildUnit)->getName().c_str());
    /*
    if (!idleWorkers.empty())
      temp = idleWorkers.front();
    if (temp)
    {
      PathFinding::UnitModel source(temp);
      this->pathFinding->generatePath(source, PathFinding::WalkabilityPosition(20, 20));
    }
    */
    #pragma endregion DisabledPathFindingPerformanceTest
    this->assignIdleWorkersToMinerals(idleWorkers);

    this->checkWorkersNeed();
    this->performAutoBuild();
    this->rebalanceMiners();
    this->checkAssignedWorkers();
    this->executeTasks();
    BWAPI::Broodwar.loadSelected(selected);
    
  }
  //-------------------------------- GET UNIT ---------------------------------
  Unit* AI::getUnit(int index)
  {
    return units[index];
  }
  //------------------------------- ON CANCEL TRAIN ---------------------------
  void AI::onCancelTrain()
  {
    this->log->log("Cancelled unit caught");
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    
    if (selected[0] != NULL)
    {
      Unit::BWUnitToBWAIUnit(selected[0])->lastTrainedUnit = BW::UnitID::None;
      this->log->log("Cancelled production caught - %s", Unit::BWUnitToBWAIUnit(selected[0])->getType().getName(), Util::LogLevel::Detailed);
    }
  }
  //---------------------------- START NEW EXPANSION -------------------------
   void AI::startNewExpansion(Unit *gatherCenter)
   {
     this->expansions.push_back(new Expansion(gatherCenter));
     this->rebalanceMiners();
   }
  u16 pos = 0;
  //---------------------------- REBALANCE MINERS -----------------------------
   void AI::rebalanceMiners(void)
   {
     if (activeMinerals.size() < 2)
       return;
     Unit* gatherer;
     anotherStep:
     for (std::list<TaskGatherGas*>::iterator i = this->activeRefineries.begin(); i != this->activeRefineries.end(); ++i)
       if ((*i)->executors.size() < 3)
       {
         Unit* newUnitToGatherGas = this->freeBuilder((*i)->getRefinery()->getPosition());
         if (newUnitToGatherGas != NULL)
           (*i)->addExecutor(newUnitToGatherGas);
       }
     TaskGather* best = activeMinerals.front();
     TaskGather* worst = activeMinerals.front();
     for (std::list<TaskGather*>::iterator i = this->activeMinerals.begin(); i != this->activeMinerals.end(); ++i)
       if (best->executors.size() > (*i)->executors.size())
         best = (*i);
       else 
         if (worst->executors.size() < (*i)->executors.size())
           worst = (*i);

     if (best->executors.size() + 1 < worst->executors.size())
     {
       gatherer = worst->executors.front();
       gatherer->freeFromTask();
       AI::optimizeMineralFor = gatherer;
       
       best = bestFor(gatherer);
       
       this->log->log("Gatherer [%d] reabalanced from [%d] to [%d]", gatherer->getIndex(), 
                                                                     worst->getMineral()->getIndex(), 
                                                                     best->getMineral()->getIndex());
       best->addExecutor(gatherer);
       goto anotherStep;
     }
   }
  //---------------------------- CHECK ASSIGNED WORKERS -----------------------
 void AI::checkAssignedWorkers(void)
  {
    for (std::list<TaskGather*>::iterator i = this->activeMinerals.begin(); i != this->activeMinerals.end(); ++i)
      (*i)->execute();
  }
  //---------------------------------------------------------------------------
  Unit* AI::optimizeMineralFor = NULL;
  
  //------------------------------- GET FIRST ---------------------------------
  Unit* AI::getFirst()
  {
    return this->first;
  }
  //----------------------------- ON REMOVE UNIT ------------------------------
  void AI::onRemoveUnit(BW::Unit* unit)
  {
    Unit* dead = BWAI::Unit::BWUnitToBWAIUnit(unit);
    this->deadLog->log("AI::onRemove Unit %s just died", dead->getName().c_str());

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
    else if (dead->getType().isWorker())
      if (dead->getTask())
        dead->getTask()->freeExecutor(dead);
    else if (
              (
               dead->getType() == BW::UnitID::Terran_CommandCenter ||
               dead->getType() == BW::UnitID::Protoss_Nexus ||
               dead->getType() == BW::UnitID::Zerg_Hatchery
              ) &&
              dead->expansion != NULL
            )
      this->removeExpansion(dead->expansion);
    dead->lastTrainedUnit = BW::UnitID::None;
    dead->expansion = NULL;
    this->deadLog->log("AI::onRemoveUnit end", dead->getName().c_str());
   }
  //------------------------------ CHECK NEW EXPANSION ------------------------
  void AI::checkNewExpansions()
  {
    for (BWAI::Unit* i = this->getFirst(); i != NULL; i = i->getNext())
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
          this->log->log("Starting new expansion - %s", i->getName().c_str(), Util::LogLevel::Important);
          this->startNewExpansion(i);
        }
        else if (i->getOrderID() == BW::OrderID::BuildingLiftoff &&
                 i->expansion != NULL)
          this->removeExpansion(i->expansion);
    }
  }
  //----------------------------- REFRESH SELECTION STATES --------------------
  void AI::refreshSelectionStates(BW::Unit** selected)
  {
    for (BWAI::Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      i->selected = false;
    for (int i = 0; selected[i] != NULL; i++)
      BWAI::Unit::BWUnitToBWAIUnit(selected[i])->selected = true;
  }
  //---------------------------- PERFRORM AUTOBUILD ---------------------------
  bool AI::performAutoBuild()
  {
    /** 
     * Just workaround, all buildings started by user will registrate as Task Build
     * Reasons:
     * 1) Finished refinery will then register TaskGatherGas
     * 2) If the scv gets killed it will automaticaly send new one
     * 3) Will be counted in the check supply function into planned supplies
     */
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isReady() &&
          i->getOwner() == player &&
          i->getType().isWorker() &&
          i->getTask() == NULL &&
          i->getOrderID() == BW::OrderID::ConstructingBuilding &&
          i->getOrderTarget() != NULL)
       {
         this->log->log("Custom building added buildTask");         
         this->plannedBuildings.push_back(new TaskBuild(i->getOrderTarget()->getType(), BW::TilePosition::Invalid, i, NULL));
       }
          
    bool reselected = false;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isReady() &&
          (
            i->hasEmptyBuildQueueLocal() ||
            (
              i->getBuildQueueLocal()[(i->getBuildQueueSlotLocal() + 1) % 5] == BW::UnitID::None &&
              i->getRawDataLocal()->currentBuildUnit != NULL &&
              i->getRawData()->currentBuildUnit->remainingBuildTime <= BWAPI::Broodwar.getLatency()
            )
          ) &&
          i->getType().canProduce() &&
          i->getOwner() == player)
      { 
        BuildOrder::BuildWeights* weights = NULL;
        if (this->root)
          weights = this->root->weights[i->getType().getName()];
        if (weights) 
        {
          if (!weights->weights.empty())
          {
            std::pair<BW::UnitType, int> best = weights->weights.front();
            
            for (std::list<std::pair<BW::UnitType, int> >::iterator j = weights->weights.begin()++;
                 j != weights->weights.end();
                 ++j)
              if (this->player->canBuild((*j).first) &&
                  (float)this->player->allUnitTypeCount[best.first.getID()]/(float)best.second >
                  (float)this->player->allUnitTypeCount[(*j).first.getID()]/(float)(*j).second)
                best = *j;
            if (this->player->canAfford(best.first))
              i->trainUnit(best.first);
          }
        }
        else if (i->lastTrainedUnit != BW::UnitID::None &&
                 i->lastTrainedUnit.isValid())
        {
          BW::UnitType typeToBuild = BW::UnitType(i->lastTrainedUnit);
          if (this->player->canAfford(typeToBuild, this->moneyToBeSpentOnBuildings))
          {
            reselected = true;
            i->trainUnit(typeToBuild);
          }
        }
      }  
    return reselected;
  }
  //------------------------------ GET IDLE WORKERS ---------------------------
  void AI::getIdleWorkers(std::list<Unit*>& workers)
  {
    //if (!this->expansionsSaturated)
      for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
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
  //------------------------------- CHECK WORKERS NEED ------------------------
  void AI::checkWorkersNeed(void)
  {
    unsigned int workersTogether = 0;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->getOwner() == player &&
          i->getType().isWorker())
        workersTogether++;
    if (workersTogether >= this->activeMinerals.size()*2.4 ||
        workersTogether >= 90)
    {
      this->expansionsSaturated = true;
      for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
        (*i)->gatherCenter->lastTrainedUnit = BW::UnitID::None;
    }
    else
      if (workersTogether < this->activeMinerals.size()*2.4 &&
          this->expansionsSaturated == true)
        this->unsaturateGather();
  }
  //----------------------------- ASSIGN IDLE WORKERS TO MINERALS -------------
  void AI::assignIdleWorkersToMinerals(std::list<Unit*>& idleWorkers)
  {
    if (this->activeMinerals.size() > 0)
      for (std::list<Unit*>::iterator i = idleWorkers.begin(); i != idleWorkers.end(); ++i)
      {
        TaskGather* best = bestFor(*i);
        if (best->executors.size() >= 2)
        {
          this->expansionsSaturated = true;
          break;
        }
        best->addExecutor(*i);
     }
  }
  //------------------------COUNT OF PRODUCTION BUILDINGS ---------------------
  int AI::countOfTerranProductionBuildings()
  {
    int countOfTerranFactories = 0;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->getType().canProduce() &&
          i->getOwner() == this->player &&
          i->getType().isTerran())
         countOfTerranFactories++;
    return countOfTerranFactories;
  }
  //------------------------------ CHECK SUPPLY NEED --------------------------
  void AI::checkSupplyNeed()
  { 
    if (!this->startingPosition)
      return;
    int countOfTerranFactories = this->countOfTerranProductionBuildings();
    if (countOfTerranFactories != 0 &&
        countOfTerranFactories * 2 >= player->getSuppliesFreeLocal(BW::Race::Terran) + plannedTerranSupplyGain() &&
        player->getSuppliesFreeLocal(BW::Race::Terran) + plannedTerranSupplyGain() < 400)
    {
      this->log->log("Not enough supplies factories = %d freeSupplies = %d plannedToBuildSupplies = %d", countOfTerranFactories, player->getSuppliesFreeLocal(BW::Race::Terran), plannedTerranSupplyGain());
      Unit* builderToUse;
      BW::TilePosition spot = getFreeBuildingSpot("non-producting-3X2", builderToUse);
      if (spot != BW::TilePosition::Invalid)
      {
        this->log->log("Found free spot for supply depot at (%d,%d)", spot.x, spot.y);
        this->plannedBuildings.push_back(new TaskBuild(BW::UnitID::Terran_SupplyDepot, spot, builderToUse, this->startingPosition->positions["non-producting-3X2"]));
      }
    }        
  }
  //---------------------------------------------------------------------------
  s32 AI::plannedTerranSupplyGain()
  {
    s32 returnValue = 0;
    for (std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin(); i != this->plannedBuildings.end(); i++)
      if ((*i)->getBuildingType() == BW::UnitID::Terran_SupplyDepot)
        returnValue += 8;
      else if ((*i)->getType() == BW::UnitID::Terran_CommandCenter)
        returnValue += 10;
    return returnValue;
  }
  //----------------------------- FREE BUILDER --------------------------------
  Unit* AI::freeBuilder(BW::Position position)
  {
    Unit* best = NULL;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->getOwner() == player && i->getType().isWorker())
        if (betterWorkerToFree(i, best, position))
          best = i;

    if (best == NULL)
      return NULL;

    this->log->log("%s was freed from it's task to do something else", best->getName().c_str());
    best->freeFromTask();
    return best;
  }
  //--------------------------------- EXECUTE TASK ----------------------------
  void AI::executeTasks()
  {
    std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin();
    while (i != this->plannedBuildings.end())
      if ((*i)->execute())
      {
        if ((*i)->getBuildingType() == BW::UnitID::Terran_Refinery)
        {
          this->log->log("Finished refinery");
          Expansion *expansion = NULL;
          for (std::list<Expansion*>::iterator j = this->expansions.begin(); j != this->expansions.end(); ++j)
            if ((*j)->gatherCenter->getDistance((*i)->getBuilding()) < Expansion::maximumMineralDistance)
              expansion = *j;
          if (expansion != NULL)
             this->activeRefineries.push_back(new TaskGatherGas((*i)->getBuilding(), expansion));
        }
        delete *i;
        i = this->plannedBuildings.erase(i);
      }
      else
        ++i;
    for (std::list<TaskGather*>::iterator i = this->activeMinerals.begin(); i != this->activeMinerals.end(); ++i)
      (*i)->execute();
    for (std::list<TaskGatherGas*>::iterator i = this->activeRefineries.begin(); i != this->activeRefineries.end(); ++i)
      (*i)->execute();
  }
  //---------------------------------------------------------------------------
  TaskGather* AI::bestFor(Unit* gatherer)
  {
    this->optimizeMineralFor = gatherer;
    TaskGather* best = activeMinerals.front();
    for (std::list<TaskGather*>::iterator i = this->activeMinerals.begin(); i != this->activeMinerals.end(); ++i)
      if (mineralValue(*i, best))
        best = *i;
    return best;
  }
  //---------------------------------------------------------------------------
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
  //---------------------------------------------------------------------------
  BW::TilePosition AI::getFreeBuildingSpot(std::string spotName, Unit*& builderToUse)
  {
    BuildingPosition* position = this->startingPosition->positions[spotName];
    if (position == NULL)
      throw GeneralException("Position '" + spotName + "' not found in the current starting position");
    for (std::list<BW::TilePosition>::iterator i = position->positions.begin();
           i != position->positions.end();
           ++i)
      {
        int occupiedCount = 0;
        Unit* occupied = NULL;
        for (int k = (*i).x; 
              k < (*i).x + position->tileWidth; 
              k++)
          for (int l = (*i).y; 
               l < (*i).y + position->tileHeight; 
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
          this->log->log("Found free spot for " + spotName + " at (%d,%d)", (*i).x, (*i).y);
          if (occupied != NULL &&
              occupied->getType() != BW::UnitID::Resource_VespeneGeyser)
            builderToUse = occupied;
          else
            builderToUse = NULL;
          return *i;
        }
      } 
   return BW::TilePosition::Invalid;
  }
  //------------------------- GET POSITIONS CALLED ----------------------------
  BuildingPosition* AI::getPositionsCalled(const std::string& place)
  {
    return this->startingPosition->positions[place];
  }
  //---------------------------------------------------------------------------
  void AI::unsaturateGather()
  {
    this->expansionsSaturated = false;
    for (std::list<Expansion*>::iterator i = this->expansions.begin();
         i != this->expansions.end();
         ++i)
      switch ((*i)->gatherCenter->getType().getID())
      {
        case BW::UnitID::Terran_CommandCenter : (*i)->gatherCenter->lastTrainedUnit = BW::UnitID::Terran_SCV; break;
        case BW::UnitID::Protoss_Nexus        : (*i)->gatherCenter->lastTrainedUnit = BW::UnitID::Protoss_Probe; break;
        case BW::UnitID::Zerg_Hatchery        : (*i)->gatherCenter->lastTrainedUnit = BW::UnitID::Zerg_Drone; break;
      }
  }
  //---------------------------------------------------------------------------
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
  //---------------------------------------------------------------------------
}

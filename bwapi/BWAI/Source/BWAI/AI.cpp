#include "AI.h"

#include <algorithm>

#include <Exceptions.h>
#include <Logger.h>
#include <Dictionary.h>
#include <StringUtil.h>
#include <RectangleArray.h>

#include "Task.h"
#include "TaskGather.h"
#include "TaskBuild.h"
#include "Unit.h"
#include "Expansion.h"
#include "MapInfo.h"
#include "MapExpansion.h"
#include "MapStartingPosition.h"

#include "../../../BWAPI/Source/BWAPI/Unit.h"
#include "../../../BWAPI/Source/BWAPI/Player.h"
#include "../../../BWAPI/Source/BWAPI/Globals.h"
#include "../../../BWAPI/Source/BWAPI/Map.h"


namespace BWAI
{
  BWAPI::Unit *cc;
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
  //------------------------------- UPDATE ------------------------------------
  void AI::update(void)
  {
    this->first = Unit::BWUnitToBWAIUnit(*BW::BWXFN_UnitNodeTable_FirstElement);
    if (this->first != NULL)
      this->first->updateNext();
  }
  //------------------------------ ON START -----------------------------------
  void AI::onStart(BWAPI::Player *player)
  {
    try
    {
      this->log->log("Ai::onStart start", LogLevel::Important);
      this->player = player;
      BWAPI::Map::saveBuildabilityMap(BWAPI::Broodwar.configuration->getValue("data_path") + "\\buildability.txt");
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
         for (std::list<BW::TilePosition>::iterator j = this->startingPosition->nonProducing3X2BuildingPositions.begin(); 
              j != this->startingPosition->nonProducing3X2BuildingPositions.end(); 
              ++j)
           this->log->log("3X2 building at (%d, %d)", (*j).x, (*j).y);
        }
      }
      mapInfo->saveDefinedBuildingsMap();
     this->log->log("Ai::onStart end", LogLevel::Important);
    }
    catch (GeneralException& exception)
    {
      this->log->log("Exception in AI::onStart: " + exception.getMessage());
      delete this->mapInfo;
      this->mapInfo = NULL;
    }
  }
  //--------------------------------- ON END ---------------------------------
  void AI::onEnd()
  {
    this->log->log("Ai::onEnd start", LogLevel::Important);
    
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
    
    for (unsigned int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      this->units[i]->clearTask();
      this->units[i]->expansion = NULL;
      this->units[i]->lastTrainedUnit = BW::UnitID::None;
    }
      
    this->startingPosition = NULL;  
    
    this->log->log("Ai::onEnd end", LogLevel::Detailed);      
  }
  //------------------------------- CONSTRUCTOR -------------------------------
  AI::AI(void)
  :mapInfo(NULL)
  ,startingPosition(NULL)
  ,log    (new Util::Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\ai",   LogLevel::Normal))
  ,deadLog(new Util::Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\dead", LogLevel::MicroDetailed))
  {
    try
    {
      Expansion::maximumMineralDistance = StringUtil::stringToInt(BWAPI::Broodwar.configuration->getValue("max_mineral_distance"));
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Used default value for max_mineral_distance as it couldn't be loaded exception: " + exception.getMessage());
    }
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->units[i] = new Unit(BWAPI::Broodwar.getUnit(i));
    this->first = NULL;
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  AI::~AI(void)
  {
    for (std::list<TaskBuild*>::iterator i = this->plannedBuildings.begin(); i != this->plannedBuildings.end(); ++i)
      delete *i;
      
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      delete *i;
      
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete units[i];

    delete this->log;
    delete deadLog;
  }
  //-------------------------------  ON FRAME ---------------------------------
  void AI::onFrame(void)
  {
    if (BWAPI::Broodwar.frameCount < 2)
      return;
    
    /*BW::Unit** selected = BWAPI::Broodwar.saveSelected();    
    this->refreshSelectionStates(selected);
     
    this->checkSupplyNeed();
    this->checkNewExpansions();

    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (!i->hasEmptyBuildQueueLocal())
        i->lastTrainedUnit = i->getBuildQueueLocal()[i->getBuildQueueSlotLocal()];

    std::list<Unit*> idleWorkers;
    this->getIdleWorkers(idleWorkers);
    this->assignIdleWorkersToMinerals(idleWorkers);

    this->checkWorkersNeed();
    this->performAutoBuild();
    this->rebalanceMiners();
    this->checkAssignedWorkers();
    this->executeTasks();
    BWAPI::Broodwar.loadSelected(selected);*/
    
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
      this->log->log("Cancelled production caught - %s", Unit::BWUnitToBWAIUnit(selected[0])->getType().getName(), LogLevel::Detailed);
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
    else if (dead->getType().isWorker())
      if (dead->getTask())
        dead->getTask()->freeExecutor(dead);

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
           i->expansion == NULL &&
           i->getOwner() == player)
      {
        this->log->log("Starting new expansion - %s", i->getName().c_str(), LogLevel::Important);
        this->expansionsSaturated = false;
        this->startNewExpansion(i);
      }
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
    bool reselected = false;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isReady() &&
          i->getType() != BW::UnitID::None &&
          i->hasEmptyBuildQueueLocal() &&
          i->lastTrainedUnit != BW::UnitID::None &&
          i->getType().canProduce() &&
          i->getOwner() == player &&
          i->lastTrainedUnit.isValid())
      {
       BW::UnitType typeToBuild = BW::UnitType(i->lastTrainedUnit);
       if (
           typeToBuild.isValid() &&
            (
              (
                 typeToBuild.isTerran() &&
                 player->freeSuppliesTerranLocal() >= typeToBuild.getSupplies()
               ) ||
               (
                 typeToBuild.isProtoss() &&
                 player->freeSuppliesProtossLocal() >= typeToBuild.getSupplies()
               ) ||
               (
                 typeToBuild.isZerg() &&
                 player->freeSuppliesZergLocal() >= typeToBuild.getSupplies()
               )
             ) &&
             player->getMineralsLocal() >= typeToBuild.getMineralPrice() &&
             player->getGasLocal() >= typeToBuild.getGasPrice()
           )
        {
          reselected = true;
          i->trainUnit(typeToBuild);
        }
      }
    return reselected;
  }
  //------------------------------ GET IDLE WORKERS ---------------------------
  void AI::getIdleWorkers(std::list<Unit*>& workers)
  {
    if (!this->expansionsSaturated)
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
              i->expansion == NULL &&
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
    if (workersTogether >= this->activeMinerals.size()*2.5 ||
        workersTogether >= 90)
    {
      this->expansionsSaturated = true;
      for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
        (*i)->gatherCenter->lastTrainedUnit = BW::UnitID::None;
    }
  }
  //----------------------------- ASSIGN IDLE WORKERS TO MINERALS -------------
  void AI::assignIdleWorkersToMinerals(std::list<Unit*>& idleWorkers)
  {
    if (!this->expansionsSaturated &&
        this->activeMinerals.size() > 0)
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
  int AI::countOfProductionBuildings()
  {
    int countOfFactories = 0;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isReady() && 
          i->getType().canProduce() &&
          i->getOwner() == this->player)
         countOfFactories++;
    return countOfFactories;
  }
  //------------------------------ CHECK SUPPLY NEED --------------------------
  void AI::checkSupplyNeed()
  { 
    if (!this->startingPosition)
      return;
    int countOfFactories = this->countOfProductionBuildings();
    if (countOfFactories != 0 &&
        countOfFactories * 2 >= player->freeSuppliesTerranLocal() + plannedTerranSupplyGain() &&
        player->freeSuppliesTerranLocal() + plannedTerranSupplyGain() < 400)
    {
      this->log->log("Not enough supplies factories = %d freeSupplies = %d plannedToBuildSupplies = %d", countOfFactories , player->freeSuppliesTerranLocal(), plannedTerranSupplyGain());
      for (std::list<BW::TilePosition>::iterator i = this->startingPosition->nonProducing3X2BuildingPositions.begin();
          i != this->startingPosition->nonProducing3X2BuildingPositions.end();
          ++i)
      {
        int occupiedCount = 0;
        Unit* lastOccupied = NULL;
        for (int j = (*i).x; 
              j < (*i).x + BW::UnitType(BW::UnitID::Terran_SupplyDepot).getTileWidth(); 
              j++)
          for (int k = (*i).y; 
               k < (*i).y + BW::UnitType(BW::UnitID::Terran_SupplyDepot).getTileHeight(); 
               k++)
            if (BWAPI::Broodwar.unitsOnTile[j][k].size() > 0)
            {
              occupiedCount ++;
              lastOccupied = BWAI::Unit::BWAPIUnitToBWAIUnit(BWAPI::Broodwar.unitsOnTile[j][k].front());
            }
        if (
             occupiedCount == 0 || 
             (
               occupiedCount == 1 &&
               lastOccupied->getType().isWorker() &&
               lastOccupied->getTask() == NULL &&
               lastOccupied->getOrderID() == BW::OrderID::Guard
             )
           )
        {
          this->log->log("Found free spot for supply depot at (%d,%d)", (*i).x, (*i).y);
          this->plannedBuildings.push_back(new TaskBuild(BW::UnitID::Terran_SupplyDepot, (*i), lastOccupied));
          break;
        }
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
  //---------------------------------------------------------------------------
  Unit* AI::freeBuilder(BW::Position position)
  {
    Unit* best = NULL;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (
           i->getOwner() == player &&
           i->getType().isWorker() &&
           (
             i->getTask() == NULL ||
             i->getTask()->getType() == TaskType::Gather
           )
         )
      {
        if (best == NULL)
          best = i;
        else
          if (best->getDistance(position) > i->getDistance(position))
            best = i;
      }
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
        delete *i;
        i = this->plannedBuildings.erase(i);
      }
      else
        ++i;
    for (std::list<TaskGather*>::iterator i = this->activeMinerals.begin(); i != this->activeMinerals.end(); ++i)
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
}

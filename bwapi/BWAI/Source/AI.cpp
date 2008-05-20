#include "AI.h"
#include <algorithm>
#include "..//..//BWAPI//Source//BWAPI//Unit.h"
#include "..//..//BWAPI//Source//BWAPI//Player.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototypeDefinitions.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototype.h"
#include "..//..//BWAPI//Source//BWAPI//AbilityFlags.h"
#include "..//..//BWAPI//Source//BWAPI//Globals.h"
#include "Mineral.h"
#include "Unit.h"
#include "Expansion.h"
#include "MapInfo.h"
#include "MapExpansion.h"
#include "MapStartingPosition.h"
//#include "..//..//BWAPI//Source//BW//UnitPrototypeFlags.h"
//#include "..//..//BWAPI//Source//BW//BitMask.h"
#include "..//..//BWAPI//Source//BW//Unit.h" /**@todo remove */
#include "..//..//BWAPI//Source//BW//Bitmask.h" /**< @todo remove */
#include "..//..//BWAPI//Source//BW//MovementFlags.h" /**< @todo remove */
#include "..//..//BWAPI//Source//BW//OrderFlags.h" /**< @todo remove */
#include "..//..//BWAPI//Source//BWAPI//Map.h"

#include "../../Util/Exceptions.h"
#include "../../Util/Logger.h"
#include "../../Util/Dictionary.h"



namespace BWAI
{
  BWAPI::Unit *cc;
  //----------------------------- MINERAL VALUE -------------------------------
  bool mineralValue(BWAI::Mineral*& mineral1, BWAI::Mineral*& mineral2)
  {
    if (mineral1->gatherersAssigned.size() < mineral2->gatherersAssigned.size())
      return true;
    if (mineral1->gatherersAssigned.size() > mineral2->gatherersAssigned.size())
      return false;
    if (AI::optimizeMineralFor != NULL &&
        mineral1->expansion->gatherCenter != mineral2->expansion->gatherCenter)
    {
      u16 distance1 = AI::optimizeMineralFor->getDistance(mineral1->expansion->gatherCenter);
      u16 distance2 = AI::optimizeMineralFor->getDistance(mineral2->expansion->gatherCenter);
      if (distance1 < distance2)
        return true;
      if (distance1 > distance2)
       return false;
    }
    u16 distance1 = mineral1->mineral->getDistance(mineral1->expansion->gatherCenter);
    u16 distance2 = mineral2->mineral->getDistance(mineral1->expansion->gatherCenter);
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
      this->log->log("Help pre-prepared information found for the curent map");
      for (std::list<MapExpansion*>::iterator i = mapInfo->expansions.begin(); i != mapInfo->expansions.end(); ++i)
        this->log->log("Expansion (%s) at (%d, %d)", (*i)->getID().c_str(), (*i)->getPosition().x, (*i)->getPosition().y);
      
      for (std::list<MapStartingPosition*>::iterator i = mapInfo->startingPositions.begin(); i != mapInfo->startingPositions.end(); ++i)
      {
        this->log->log("StartingPosition (%s) at (%d, %d)", (*i)->expansion->getID().c_str(), (*i)->expansion->getPosition().x, (*i)->expansion->getPosition().y);
        for (std::list<BW::Position>::iterator j = (*i)->nonProducing3X2BuildingPositions.begin(); j != (*i)->nonProducing3X2BuildingPositions.end(); ++j)
          this->log->log("3X2 building at at (%d, %d)", (*j).x, (*j).y);
      }
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
    this->activeMinerals.clear();
    this->expansions.clear();
    this->expansionsSaturated = false;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      i->expansionAssingment = NULL;
    this->log->log("Ai::onEnd end", LogLevel::Detailed);
    delete mapInfo;
  }
  //------------------------------- CONSTRUCTOR -------------------------------
  AI::AI(void)
  :mapInfo(NULL)
  ,startingPosition(NULL)
  ,log(new Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\ai", LogLevel::MicroDetailed))
  ,deadLog(new Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\dead", LogLevel::MicroDetailed))
  {
    this->suppliesOrdered = 0;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->units[i] = new Unit(BWAPI::Broodwar.getUnit(i));
    this->first = NULL;
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  AI::~AI(void)
  {
    delete this->log;
    delete deadLog;

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete new Unit(BWAPI::Broodwar.getUnit(i));
  }
  bool mapDrawn = false;
  //-------------------------------  ON FRAME ---------------------------------
  void AI::onFrame(void)
  {
    if (BWAPI::Broodwar.frameCount < 2)
      return;
     
    this->checkSupplyNeed();
        
    bool reselected = false;
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    this->refreshSelectionStates(selected);

    this->checkNewExpansions();

    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (!i->hasEmptyBuildQueueLocal())
        i->lastTrainedUnitID = i->getBuildQueueLocal()[i->getBuildQueueSlotLocal()];

    std::list<Unit*> idleWorkers;
    this->getIdleWorkers(idleWorkers);
    this->assignIdleWorkersToMinerals(idleWorkers);

    this->checkWorkersNeed();
    reselected |= this->performAutoBuild();
    this->rebalanceMiners();
    reselected |= this->checkAssignedWorkers();
    if (reselected)
      BWAPI::Broodwar.loadSelected(selected);
    else
      delete [] selected;
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
      Unit::BWUnitToBWAIUnit(selected[0])->lastTrainedUnitID = BW::UnitType::None;
      this->log->log("Cancelled production caught - %s", Unit::BWUnitToBWAIUnit(selected[0])->getPrototype()->getName().c_str(), LogLevel::Detailed);
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
     Mineral* best = *activeMinerals.begin();
     Mineral* worst = *activeMinerals.begin();
     for (std::list<Mineral*>::iterator i = this->activeMinerals.begin(); i != this->activeMinerals.end(); ++i)
       if (best->gatherersAssigned.size() > (*i)->gatherersAssigned.size())
         best = (*i);
       else 
         if (worst->gatherersAssigned.size() < (*i)->gatherersAssigned.size())
           worst = (*i);

     if (best->gatherersAssigned.size() + 1 < worst->gatherersAssigned.size())
     {
       gatherer = worst->gatherersAssigned[0];
       worst->removeGatherer(gatherer);
       AI::optimizeMineralFor = gatherer;
       activeMinerals.sort(mineralValue);
       (*activeMinerals.begin())->assignGatherer(gatherer);
       goto anotherStep;
     }
   }
  //---------------------------- CHECK ASSIGNED WORKERS -----------------------
  bool AI::checkAssignedWorkers(void)
  {
    bool reselected = false;
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      reselected |= (*i)->checkAssignedWorkers();
    return reselected;
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
    this->deadLog->log("%s just died", dead->getName().c_str());

    if (dead->isMineral())
      if (dead->expansionAssingment != NULL)
        dead->expansionAssingment->removeMineral(dead);
    else if (dead->getPrototype()->getAbilityFlags() | BWAPI::AbilityFlags::Gather)
      if (dead->expansionAssingment != NULL)
        dead->expansionAssingment->removeWorker(dead);
    dead->lastTrainedUnitID = BW::UnitType::None;
    dead->expansionAssingment = NULL;
   }
  //------------------------------ CHECK NEW EXPANSION ------------------------
  void AI::checkNewExpansions()
  {
    for (BWAI::Unit* i = this->getFirst(); i != NULL; i = i->getNext())
    {
      if (
           i->isReady() &&
           (
             i->getType() == BW::UnitType::Terran_CommandCenter ||
             i->getType() == BW::UnitType::Protoss_Nexus ||
             i->getType() == BW::UnitType::Zerg_Hatchery
           ) &&
           i->expansionAssingment == NULL &&
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
          i->getPrototype() != NULL &&
          i->hasEmptyBuildQueueLocal() &&
          i->lastTrainedUnitID != BW::UnitType::None &&
          i->getPrototype()->canProduce() &&
          i->getOwner() == player &&
          i->lastTrainedUnitID < 228)
      {
        BWAPI::UnitPrototype* type = BWAPI::Prototypes::unitIDToPrototypeTable[i->lastTrainedUnitID];
        if (type != NULL &&
             (
               (
                  type->isTerran() &&
                  player->freeSuppliesTerranLocal() >= type->getSupplies()
                ) ||
                (
                  type->isProtoss() &&
                  player->freeSuppliesProtossLocal() >= type->getSupplies()
                ) ||
                (
                  type->isZerg() &&
                  player->freeSuppliesZergLocal() >= type->getSupplies()
                )
              ) &&
            player->getMineralsLocal() >= type->getMineralPrice() &&
            player->getGasLocal() >= type->getGasPrice())
        {
          reselected = true;
          i->trainUnit(type);
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
             (i->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Gather) &&
              i->expansionAssingment == NULL)
          workers.push_back(i); 
      }
  }
  //--------------------------------- CHECK WORKERS NEED ----------------------
  void AI::checkWorkersNeed(void)
  {
    unsigned int workersTogether = 0;
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      workersTogether += (*i)->asignedWorkers;
    if (workersTogether >= this->activeMinerals.size()*2.5 ||
        workersTogether >= 100)
    {
      this->expansionsSaturated = true;
      for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
        (*i)->gatherCenter->lastTrainedUnitID = BW::UnitType::None;
    }
  }
  //----------------------------- ASSIGN IDLE WORKERS TO MINERALS -------------
  void AI::assignIdleWorkersToMinerals(std::list<Unit*>& idleWorkers)
  {
    if (!this->expansionsSaturated)
      for (std::list<Unit*>::iterator i = idleWorkers.begin(); i != idleWorkers.end(); ++i)
      {
        AI::optimizeMineralFor = *i;
        /** @todo Just find smallest, no need to sort */
        activeMinerals.sort(mineralValue);
        if ((*activeMinerals.begin())->gatherersAssigned.size() >= 2)
        {
          this->expansionsSaturated = true;
          break;
        }
        (*activeMinerals.begin())->assignGatherer(*i);
      }
  }
  //---------------------------------------------------------------------------
  int AI::countOfProductionBuildings()
  {
    int countOfFactories = 0;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isReady() && i->getPrototype()->canProduce())
         countOfFactories++;
    return countOfFactories;
  }
  //---------------------------------------------------------------------------
  void AI::checkSupplyNeed()
  {
    int countOfFactories = this->countOfProductionBuildings();
    if (countOfFactories * 1.5 > player->freeSuppliesTerranLocal())
    {
        
    }
  }
  //---------------------------------------------------------------------------
}

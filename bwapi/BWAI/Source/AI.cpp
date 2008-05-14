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
//#include "..//..//BWAPI//Source//BW//UnitPrototypeFlags.h"
//#include "..//..//BWAPI//Source//BW//BitMask.h"
#include "..//..//BWAPI//Source//BW//Unit.h" /**@todo remove */
#include "..//..//BWAPI//Source//BW//Bitmask.h" /**< @todo remove */
#include "..//..//BWAPI//Source//BW//MovementFlags.h" /**< @todo remove */
#include "..//..//BWAPI//Source//BW//OrderFlags.h" /**< @todo remove */
#include "..//..//BWAPI//Source//Logger.h"

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
  #include <stdio.h>
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
    this->log->log("Ai::Game start", Logger::Important);
    this->player = player;
  }
  //--------------------------------- ON END ---------------------------------
  void AI::onEnd()
  {
    this->log->log("Ai::On end start",Logger::Important);
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      delete *i;
    this->expansions.clear();
    this->log->log("Ai::On end end",Logger::Detailed);
  }
  //------------------------------- CONSTRUCTOR -------------------------------
  AI::AI(void)
  {
    this->log = new Logger("ai", Logger::MicroDetailed);
    this->deadLog= new Logger("dead", Logger::MicroDetailed);

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
  //-------------------------------  ON FRAME ---------------------------------
  void AI::onFrame(void)
  {
    bool reselected = false;
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    this->refreshSelectionStates(selected);

    this->checkNewExpansions();


    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (!i->hasEmptyBuildQueueLocal() && 
           i->getType() != BW::UnitType::Terran_CommandCenter)
           i->lastTrainedUnitID = i->getBuildQueueLocal()[i->getBuildQueueSlotLocal()];

    std::list<Unit*> idleWorkers;
    if (!this->expansionsSaturated)
      for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
        {
         if (i->isReady() &&
             i->getOwner() == player &&
             i->getOrderIDLocal() == BW::OrderID::Idle &&
             !i->selected &&
             (i->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Gather) &&
              i->expansionAssingment == NULL)
         idleWorkers.push_back(i); 
       }
    
  /*   if (selectedUnit != NULL && 
        selectedUnit->getPrototype() == BWAPI::Prototypes::SCV &&
        marwin->getMineralsLocal() >= 150 &&
        this->suppliesOrdered < 20)
    {
      int x = (cc->getPosition().x - BWAPI::Prototypes::CommandCenter->dimensionLeft())/BW::TileSize;
      int y = (cc->getPosition().y - BWAPI::Prototypes::CommandCenter->dimensionUp())/BW::TileSize;
      selectedUnit->build(x + 5, y, BWAPI::Prototypes::SupplyDepot);
      suppliesOrdered ++;
      char message[50];
      sprintf(message, "Supply depot order given (%d, %d)", x, y);
      game.print(message);
    }*/
   
    /*for (unsigned int i = 0; i < expansions.size(); i++)
      if (expansions[i]->gatherCenter != NULL)
      {
        if (expansions[i]->gatherCenter->hasEmptyBuildQueueLocal() && 
            player->getMineralsLocal() >= this->worker->getMineralPrice()&&
            player->freeSuppliesTerranLocal() >= this->worker->getSupplies())
        {
          reselected = true;
          expansions[i]->gatherCenter->orderSelect();
          expansions[i]->gatherCenter->trainUnit(this->worker);
        }
    }*/
    unsigned int workersTogether = 0;
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      workersTogether += (*i)->asignedWorkers;
    if (workersTogether >= this->activeMinerals.size()*3 &&
        this->expansionsSaturated)
      for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
        (*i)->gatherCenter->lastTrainedUnitID = BW::UnitType::None;
    reselected |= this->performAutoBuild();
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
  //---------------------------------------------------------------------------
  void AI::onCancelTrain()
  {
    this->log->log("Cancelled unit caught");
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    
    if (selected[0] != NULL)
    {
      Unit::BWUnitToBWAIUnit(selected[0])->lastTrainedUnitID = BW::UnitType::None;
      this->log->log("Cancelled production caught - %s", Unit::BWUnitToBWAIUnit(selected[0])->getPrototype()->getName().c_str(), Logger::Detailed);
    }
  }
  //---------------------------- START NEW EXPANSION -------------------------
   void AI::startNewExpansion(Unit *gatherCenter)
   {
     this->expansions.push_back(new Expansion(gatherCenter));
     this->rebalanceMiners();
   }
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
      else if (worst->gatherersAssigned.size() < (*i)->gatherersAssigned.size())
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
  //---------------------------------------------------------------------------
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
        this->log->log("Starting new expansion - %s", i->getName().c_str(), Logger::Important);
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
      if (i->getPrototype() != NULL &&
          i->hasEmptyBuildQueueLocal() &&
          i->lastTrainedUnitID != BW::UnitType::None &&
          i->getPrototype()->canProduce() &&
          i->getOwner() == player &&
          i->lastTrainedUnitID < 228)
      {
        BWAPI::UnitPrototype* type = BWAPI::Prototypes::unitIDToPrototypeTable[i->lastTrainedUnitID];
        if (type != NULL &&
            player->freeSuppliesTerranLocal() >= type->getSupplies() &&
            player->getMineralsLocal() >= type->getMineralPrice() &&
            player->getGasLocal() >= type->getGasPrice())
        {
          reselected = true;
          i->trainUnit(type);
        }
      }
    return reselected;
  }
  //---------------------------------------------------------------------------
}

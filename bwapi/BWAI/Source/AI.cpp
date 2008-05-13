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
    FILE *f = fopen("bwai.log","at");
    fprintf(f,"Ai::On start call\n");
    fclose(f);
    this->player = player;

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      if (
          this->units[i]->isValid() &&
          this->units[i]->getOwner() == player &&
          (
           this->units[i]->getType() == BW::UnitType::Terran_CommandCenter ||
           this->units[i]->getType() == BW::UnitType::Protoss_Nexus ||
           this->units[i]->getType() == BW::UnitType::Zerg_Hatchery
          )
        )
      {
        this->startNewExpansion(this->units[i]);
        f = fopen("bwai.log","at");
        fprintf(f,"AI::rebalance miners call mineralCount = %d\n", this->activeMinerals.size());
        fclose(f);
        this->rebalanceMiners();
      }
    }
   if (this->expansions.size() > 0)
     switch (this->expansions[0]->gatherCenter->getType())
     {
       case BW::UnitType::Terran_CommandCenter : this->worker = BWAPI::Prototypes::SCV; break;
       case BW::UnitType::Protoss_Nexus        : this->worker = BWAPI::Prototypes::Probe; break;
       case BW::UnitType::Zerg_Hatchery        : this->worker = BWAPI::Prototypes::Drone; break;
     }
    f = fopen("bwai.log","at");
    fprintf(f,"Ai::On start end\n");
    fclose(f);
  }
  //--------------------------------- ON END ---------------------------------
  void AI::onEnd()
  {
    for (unsigned int i = 0; i < this->expansions.size(); i++)
       delete this->expansions[i];
    this->expansions.clear();
  }
  //------------------------------- CONSTRUCTOR -------------------------------
  AI::AI(void)
  {
    this->suppliesOrdered = 0;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->units[i] = new Unit(BWAPI::Broodwar.getUnit(i));
    this->first = NULL;
  }
  //-------------------------------  ON FRAME ---------------------------------
  void AI::onFrame(void)
  {
    BWAPI::Broodwar.logUnitList();
    /*std::vector<BWAI::Unit*> unitList;
    bool reselected = false;
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    bool firstSelectedFound = false;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      if (units[i]->isValid())
      {
        /*unit->selected = false;
        for (int j = 0; selected[j] != NULL; j++)
          if (selected[j] == unit->getOriginalRawData())
          {
            unit->selected = true;
            firstSelectedFound = true;
          }
        if (!unit->hasEmptyBuildQueueLocal() && 
            unit->getType() != BW::UnitType::Terran_CommandCenter)
          unit->lastTrainedUnitID = unit->getBuildQueueLocal()[unit->getBuildQueueSlotLocal()];
        if (unit->isReady() &&
            unit->getType() == BW::UnitType::Terran_CommandCenter &&
            unit->expansionAssingment == NULL &&
            unit->getOwner() == player)
        {
          FILE *f = fopen("bwai.log","at");
          fprintf(f,"Starting new expansion\n");
          fclose(f);
          this->expansionsSaturated = false;
          this->startNewExpansion(unit);
        }
      }
     if (!firstSelectedFound && selected[0] != NULL)
     {
       char message[50];
       sprintf(message, "Unknown selected addr = 0x%X\n", (int)selected[0]);
       sprintf(message, "Index = %d\n", ((int)selected[0] - (int)BW::BWXFN_UnitNodeTable)/336);
       BWAPI::Broodwar.print(message);
     }

    if (!this->expansionsSaturated)
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      Unit* unit = this->units[i];
      if (unit->isReady() &&
          unit->getOwner() == player &&
          unit->getOrderIDLocal() == BW::OrderID::Idle &&
          !unit->selected &&
          (unit->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Gather) &&
          unit->expansionAssingment == NULL)
         unitList.push_back(unit); 
    }
    /*if (selectedUnit != NULL && 
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
    /*unsigned int workersTogether = 0;
    for (unsigned int i = 0; i < this->expansions.size(); i++)
      workersTogether += this->expansions[i]->asignedWorkers;
    if (workersTogether >= this->activeMinerals.size()*3)
      for (unsigned int i = 0; i < this->expansions.size(); i++)
        this->expansions[i]->gatherCenter->lastTrainedUnitID = BW::UnitType::None;
    for (unsigned int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      if (units[i]->isValid() &&
          units[i]->hasEmptyBuildQueueLocal() &&
          units[i]->lastTrainedUnitID != BW::UnitType::None &&
          units[i]->getPrototype()->canProduce())
      {
        BWAPI::UnitPrototype* type = BWAPI::Prototypes::unitIDToPrototypeTable[units[i]->lastTrainedUnitID];
        if (type != NULL &&
            player->freeSuppliesTerranLocal() >= type->getSupplies() &&
            player->getMineralsLocal() >= type->getMineralPrice() &&
            player->getGasLocal() >= type->getGasPrice())
        {
          reselected = true;
          units[i]->trainUnit(type);
        }
    }

    for (unsigned int i = 0; i < unitList.size() && i < activeMinerals.size(); i++)
    {
      AI::optimizeMineralFor = unitList[i];
      std::sort(activeMinerals.begin(),activeMinerals.end(), mineralValue);
      if (activeMinerals[0]->gatherersAssigned.size() >= 2)
      {
        this->expansionsSaturated = true;
        break;
      }
      reselected = true;
      activeMinerals[0]->assignGatherer(unitList[i]);
      unitList[i]->orderRightClick(activeMinerals[0]->mineral);
      }
   this->rebalanceMiners();
   reselected |= this->checkAssignedWorkers();
   if (reselected)
     BWAPI::Broodwar.loadSelected(selected);
   else
     delete [] selected;*/
  }
  //-------------------------------- GET UNIT ---------------------------------
  Unit* AI::getUnit(int index)
  {
    return units[index];
  }
  //---------------------------------------------------------------------------
  void AI::onCancelTrain()
  {
    FILE* f = fopen("bwai.log","at");
    fprintf(f,"Cancelled\n");
    fclose(f);
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    if (selected[0] != NULL)
    {
      Unit::BWUnitToBWAIUnit(selected[0])->lastTrainedUnitID = BW::UnitType::None;
      FILE* f = fopen("bwai.log","at");
      fprintf(f,"%s will now not produce\n", Unit::BWUnitToBWAIUnit(selected[0])->getPrototype()->getName().c_str());
      fclose(f);
    }
  }
  //---------------------------- START NEW EXPANSION -------------------------
   void AI::startNewExpansion(Unit *gatherCenter)
   {
     this->expansions.push_back(new Expansion(gatherCenter));
   }
  //---------------------------- REBALANCE MINERS -----------------------------
   void AI::rebalanceMiners(void)
   {
     if (activeMinerals.size() < 2)
       return;
     Unit* gatherer;
     anotherStep:
     std::sort(activeMinerals.begin(),activeMinerals.end(), mineralValue);
     Mineral* first = activeMinerals[0];
     Mineral* last = activeMinerals[activeMinerals.size() - 1];
     if (first->gatherersAssigned.size() + 1 < last->gatherersAssigned.size())
     {
       gatherer = last->gatherersAssigned[0];
       last->removeGatherer(gatherer);
       AI::optimizeMineralFor = gatherer;
       std::sort(activeMinerals.begin(),activeMinerals.end(), mineralValue);
       first->assignGatherer(gatherer);
       goto anotherStep;
     }
   }
  //---------------------------------------------------------------------------
  bool AI::checkAssignedWorkers(void)
  {
    bool reselected = false;
    for (unsigned int i = 0; i < this->expansions.size(); i++)
      reselected |= this->expansions[i]->checkAssignedWorkers();
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

    FILE *f = fopen("death.log","at");
    fprintf(f, "Unit %s just died\n", dead->getName().c_str());
    fclose(f);

    if (dead->isMineral())
      if (dead->expansionAssingment != NULL)
        dead->expansionAssingment->removeMineral(dead);
    else if (dead->getPrototype()->getAbilityFlags() | BWAPI::AbilityFlags::Gather)
      if (dead->expansionAssingment != NULL)
        dead->expansionAssingment->removeWorker(dead);
   }
  //---------------------------------------------------------------------------
}

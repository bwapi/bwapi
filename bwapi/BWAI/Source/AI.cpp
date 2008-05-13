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
 template <class Type>
  std::string getBinary(Type value)
  {
   std::string result;
    for (int i = 0; i < sizeof(Type)*8; i++)
      if (value  & (1 << (sizeof(Type)*8-1-i)))
         result += "1";
      else
         result += "0";
    return result;
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
    logUnknownOrStrange();

    FILE *f = fopen("sums.log","at");
    int unitCount = 0;
    fprintf(f, "-------------------\n");
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
       fprintf(f, "%s (%s) \n", i->getName().c_str(), 
                                getBinary((u8) i->getOriginalRawData()->orderFlags.value).c_str());
    fclose(f);
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
  //---------------------------------------------------------------------------
  void AI::logUnknownOrStrange()
  {
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      if (units[i]->isValid())
      {
        Unit* unit = this->units[i];
        if (unit->getOrderID() != BW::OrderID::GameNotInitialized &&
            unit->getOrderID() != BW::OrderID::Finishing&&
            unit->getOrderID() != BW::OrderID::Idle &&
            unit->getOrderID() != BW::OrderID::Moving &&
            unit->getOrderID() != BW::OrderID::Attacking &&
            unit->getOrderID() != BW::OrderID::AttackMoving &&
            unit->getOrderID() != BW::OrderID::NotMovable &&
            unit->getOrderID() != BW::OrderID::JustToMutate &&
            unit->getOrderID() != BW::OrderID::Constructing &&
            unit->getOrderID() != BW::OrderID::Repair &&
            unit->getOrderID() != BW::OrderID::EggMutating &&
            unit->getOrderID() != BW::OrderID::GoingToBuild &&
            unit->getOrderID() != BW::OrderID::UnderConstruction &&
            unit->getOrderID() != BW::OrderID::NotControllable &&
            unit->getOrderID() != BW::OrderID::Following &&
            unit->getOrderID() != BW::OrderID::GoingToMutate &&
            unit->getOrderID() != BW::OrderID::Building_Landing &&
            unit->getOrderID() != BW::OrderID::Lifting &&
            unit->getOrderID() != BW::OrderID::ApproachingRafinery &&
            unit->getOrderID() != BW::OrderID::EnteringRafinery &&
            unit->getOrderID() != BW::OrderID::InRafinery &&
            unit->getOrderID() != BW::OrderID::ReturningGas &&
            unit->getOrderID() != BW::OrderID::ApproachingMinerals &&
            unit->getOrderID() != BW::OrderID::StartingMining  &&
            unit->getOrderID() != BW::OrderID::Mining &&
            unit->getOrderID() != BW::OrderID::ReturningMinerals &&
            unit->getOrderID() != BW::OrderID::OverlordIdle &&
            unit->getOrderID() != BW::OrderID::Burrowing &&
            unit->getOrderID() != BW::OrderID::Burrowed &&
            unit->getOrderID() != BW::OrderID::Unburrowing &&
            unit->getOrderID() != BW::OrderID::GettingMinedMinerals &&
            unit->getOrderID() != BW::OrderID::CritterWandering &&
            unit->getOrderID() != BW::OrderID::Stop &&
            unit->getOrderID() != BW::OrderID::BuildingMutating)
        {
         FILE *f = fopen("new_order_id.txt","at");
         fprintf(f, "%s\n", unit->getName().c_str());
         fclose(f);
        }
        if (unit->getOriginalRawData()->movementFlags.getBit(BW::MovementFlags::_alwaysZero1))
        {
          FILE *f = fopen("new_movementState.txt","at");
          fprintf(f, "%s  - Unknown  orderID = %d - movementstate _alwaysZero1 is not zero (%s)\n", unit->getName().c_str(), unit->getOrderID(), getBinary((u8)units[i]->getOriginalRawData()->movementFlags.value).c_str());
          fclose(f);
        }
        
       if (unit->getOriginalRawData()->resource &&
           !unit->isMineral() &&
           unit->getType() != BW::UnitType::Resource_VespeneGeyser)
        {
          FILE *f = fopen("new_movementState.txt","at");
          fprintf(f, "Unit %s orderID = %d - is resource and is not resource ^^\n", unit->getName().c_str(), unit->getOrderID());
          fclose(f);
        }         
     }
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    if (units[i]->getOriginalRawData()->playerID <= 11)
    {
      if (!units[i]->isMineral() &&
          units[i]->getType() != BW::UnitType::Zerg_Larva &&
          units[i]->getType() != BW::UnitType::Critter_Bengalaas &&
          units[i]->getType() != BW::UnitType::Zerg_Drone &&
          units[i]->getType() != BW::UnitType::Zerg_Egg &&
          units[i]->getOrderID() != BW::OrderID::BuildingMutating &&
          units[i]->getOriginalRawData()->orderFlags.getBit(BW::OrderFlags::willWanderAgain))
        {
          FILE *f = fopen("new_main_order_state.txt","at");
          fprintf(f, "Unit %s has orderstate.willWander again true\n", units[i]->getName().c_str());
          fclose(f);
        }
      /*if (units[i]->getType() != BW::UnitType::Zerg_Larva &&
          units[i]->getType() != BW::UnitType::Critter_Bengalaas &&
          units[i]->getType() != BW::UnitType::Zerg_Drone &&
          units[i]->getType() != BW::UnitType::Zerg_Overlord &&
          units[i]->getType() != BW::UnitType::Zerg_Egg &&
          units[i]->getOrderID() != BW::OrderID::BuildingMutating &&
          units[i]->getOriginalRawData()->orderFlags.getBit(BW::OrderFlags::autoWander))
        {
          FILE *f = fopen("new_main_order_state.txt","at");
          fprintf(f, "%s has auto wander state = true\n", units[i]->getName().c_str());
          fclose(f);
        }*/
     if (units[i]->getType() != BW::UnitType::Critter_Bengalaas &&
         units[i]->getType() != BW::UnitType::Critter_Ragnasaur &&
         units[i]->getType() != BW::UnitType::Critter_Kakaru &&
         units[i]->getOrderID() == BW::OrderID::CritterWandering)
        {
          FILE *f = fopen("new_main_order_state.txt","at");
          fprintf(f, "Unit %s is wandering around and is unknown critter\n", units[i]->getName().c_str());
          fclose(f);
        }
     if (
          units[i]->getType() != BW::UnitType::Zerg_Drone &&
          (
            units[i]->getOrderID() == BW::OrderID::JustToMutate ||
            units[i]->getOrderID() == BW::OrderID::GoingToMutate
          )
        )
     {
       FILE *f = fopen("new_order_id.txt","at");
       fprintf(f, "%s is going to mutate to building, but it is not drone\n", units[i]->getName().c_str());
       fclose(f);
     }

     if (
          units[i]->getType() != BW::UnitType::Zerg_Overlord &&
          units[i]->getOrderID() == BW::OrderID::OverlordIdle
        )
     {
       FILE *f = fopen("new_order_id.txt","at");
       fprintf(f, "%s is doing overlord idle (and is not overlord ^^)\n", units[i]->getName().c_str());
       fclose(f);
     }


    }
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
  //------------------------------- GET FIRST ---------------------------------
  Unit* AI::getFirst()
  {
    return this->first;
  }
  //---------------------------------------------------------------------------
}

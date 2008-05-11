#include "AI.h"
#include <algorithm>
#include "..//..//BWAPI//Source//BWAPI//Unit.h"
#include "..//..//BWAPI//Source//BWAPI//Player.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototypeDefinitions.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototype.h"
#include "..//..//BWAPI//Source//BWAPI//AbilityFlags.h"
#include "..//..//BWAPI//Source//BWAPI//Globals.h"
#include "..//..//BWAPI//Source//BW//Offsets.h"
#include "Mineral.h"
#include "Unit.h"
#include "Expansion.h"
//#include "..//..//BWAPI//Source//BW//UnitPrototypeFlags.h"
//#include "..//..//BWAPI//Source//BW//BitMask.h"

namespace BWAI
{
 BWAPI::Unit *cc;
  //----------------------------- MINERAL VALUE -------------------------------
 bool mineralValue(BWAI::Mineral*& mineral1, BWAI::Mineral*& mineral2)
  {
   if (mineral1->expansion->asignedWorkers < mineral2->expansion->asignedWorkers)
      return true;
   if (mineral1->expansion->asignedWorkers > mineral2->expansion->asignedWorkers)
      return false;
   if (mineral1->gatherersAssigned.size() < mineral2->gatherersAssigned.size())
     return true;
   if (mineral1->gatherersAssigned.size() > mineral2->gatherersAssigned.size())
     return false;
   if (mineral1->mineral->getDistance(mineral1->expansion->gatherCenter) < mineral2->mineral->getDistance(mineral2->expansion->gatherCenter))
     return true;
   if (mineral1->mineral->getDistance(mineral1->expansion->gatherCenter) > mineral2->mineral->getDistance(mineral2->expansion->gatherCenter))
     return false;
   return false;
 }

  
   #include <stdio.h>
  std::string getBinary(UNKNOWN_TYPE value)
  {
   std::string result;
    for (int i = 0; i < UNKNOWN_BIT_SIZE; i++)
      if (value  & (1 << (UNKNOWN_BIT_SIZE-1-i)))
         result += "1";
      else
         result += "0";
    return result;
  }
  //------------------------------ ON START -----------------------------------
  void AI::onStart(BWAPI::Game& game, BWAPI::Player *player)
  {
    FILE *f = fopen("bwai.log","at");
    fprintf(f,"Ai::On start call\n");
    fclose(f);
    this->player = player;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->units[i] = new Unit(game.getUnit(i));
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
  //------------------------------- CONSTRUCTOR -------------------------------
  AI::AI()
  {
    this->suppliesOrdered = 0;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->units[i] = NULL;
  }
  //-------------------------------  ON FRAME ---------------------------------
  void AI::onFrame(BWAPI::Game& game)
  {
    std::vector<BWAI::Unit*> unitList;
    bool reselected = false;
    
    BW::UnitData** selected = game.saveSelected();
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      if (units[i]->isValid())
      {
        Unit* unit = this->units[i];
        unit->selected = false;
        for (int j = 0; selected[j] != NULL; j++)
          if (selected[j] == unit->getOriginalRawData())
            unit->selected = true;
        if (!unit->hasEmptyBuildQueueLocal())
          unit->lastTrainedUnitID = unit->getBuildQueueLocal()[unit->getBuildQueueSlotLocal()];
        if (unit->isReady() &&
            unit->getType() == BW::UnitType::Terran_CommandCenter &&
            unit->expansionAssingment == NULL)
        {
          FILE *f = fopen("bwai.log","at");
          fprintf(f,"Starting new expansion\n");
          fclose(f);
          this->startNewExpansion(unit);
        }
      }


    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      Unit* unit = this->units[i];
      if (unit->isReady() &&
          unit->getOwner() == player)
      {
       if (!unit->selected &&
           (unit->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Gather) &&
           unit->expansionAssingment == NULL)
         unitList.push_back(unit);
      }
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
          FILE *f = fopen("bwai.log","at");
          fprintf(f,"We will build (%s)\n", type->getName().c_str());
          fclose(f);
          reselected = true;
          units[i]->trainUnit(type);
        }
    }

    if (unitList.size() != 0)
    {
      std::sort(activeMinerals.begin(),activeMinerals.end(), mineralValue);
      for (unsigned int i = 0; i < unitList.size(); i++)
      {
        reselected = true;
        activeMinerals[i]->assignGatherer(unitList[i]);
        unitList[i]->orderRightClick(activeMinerals[i]->mineral);
        FILE *f = fopen("bwai.log","at");
        fprintf(f,"(%s) -> (%s)\n", unitList[i]->getPrototype()->getName().c_str(), activeMinerals[i]->mineral->getPrototype()->getName().c_str());
        fclose(f);
      }
    }
   reselected |= this->checkAssignedWorkers();
   if (reselected)
     game.loadSelected(selected);
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
    FILE* f = fopen("bwai.log","at");
    fprintf(f,"Cancelled\n");
    fclose(f);
    BW::UnitData** selected = BWAPI::Broodwar.saveSelected();
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
     Unit* gatherer;
     anotherStep:
     std::sort(activeMinerals.begin(),activeMinerals.end(), mineralValue);
     Mineral* first = activeMinerals[0];
     Mineral* last = activeMinerals[activeMinerals.size() - 1];
     if (first->gatherersAssigned.size() + 1 < last->gatherersAssigned.size())
     {
       gatherer = first->gatherersAssigned[0];
       last->removeGatherer(gatherer);
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
}

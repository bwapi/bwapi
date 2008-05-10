#include "AI.h"
#include <algorithm>
#include "..//..//BWAPI//Source//BWAPI//Unit.h"
#include "..//..//BWAPI//Source//BWAPI//Player.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototypeDefinitions.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototype.h"
#include "..//..//BWAPI//Source//BWAPI//AbilityFlags.h"
#include "..//..//BWAPI//Source//BW//Offsets.h"
#include "Mineral.h"
#include "Unit.h"
#include "Expansion.h"

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
        Expansion* expansion = new Expansion(this->units[i]);
        this->expansions.push_back(expansion);
        for (unsigned int j = 0; j < expansion->minerals.size(); j++)
          this->activeMinerals.push_back(expansion->minerals[j]);
      }
    }
   if (this->expansions.size() > 0)
     switch (this->expansions[0]->gatherCenter->getType())
     {
       case BW::UnitType::Terran_CommandCenter : this->worker = BWAPI::Prototypes::SCV; break;
       case BW::UnitType::Protoss_Nexus        : this->worker = BWAPI::Prototypes::Probe; break;
       case BW::UnitType::Zerg_Hatchery        : this->worker = BWAPI::Prototypes::Drone; break;
     }
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
    {
      this->units[i]->selected = false;
      for (int j = 0; selected[j] != NULL; j++)
        if (selected[j] == this->units[i]->getOriginalRawData())
           this->units[i]->selected = true;
    }


    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      Unit* unit = this->units[i];
      if (unit->isValid() &&
          unit->getOwner() == player &&
          unit->getType() == BW::UnitType::Terran_SCV &&
          unit->expansionAssingment == 0)
      {
        FILE *f = fopen("bwai.log","at");
        fprintf(f,"(%s) - to build (%d) (%d - %d - %d - %d) (%s)\n", unit->getPrototype()->getName().c_str(), 
                                                      unit->getRawData()->remainingBuildTime,
                                                      unit->getRawData()->_17[1],
                                                      unit->getRawData()->_17[0],
                                                      unit->getRawData()->_18[0],
                                                      unit->getRawData()->_18[1],
                                                      BW::OrderID::orderName(unit->getOrderID()).c_str());
        fclose(f);
      }
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
   
    for (unsigned int i = 0; i < expansions.size(); i++)
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
    }
    
    if (unitList.size() != 0)
    {
      std::sort(activeMinerals.begin(),activeMinerals.end(), mineralValue);
      for (unsigned int i = 0; i < unitList.size(); i++)
      {
        FILE *f = fopen("bwai.log","at");
        fprintf(f,"Unit sent to gather\n");
        fclose(f);
        reselected = true;
        activeMinerals[i]->assignGatherer(unitList[i]);
        unitList[i]->orderRightClick(activeMinerals[i]->mineral);
        f = fopen("bwai.log","at");
        fprintf(f,"(%s) -> (%s)\n", unitList[i]->getPrototype()->getName().c_str(), activeMinerals[i]->mineral->getPrototype()->getName().c_str());
        fclose(f);
      }
    }
   
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
}
#include "Mineral.h"
#include "Unit.h"
#include "Expansion.h"
#include "Globals.h"
#include "../../BWAPI/Source/BW/UnitType.h"
#include "../../BWAPI/Source/BWAPI/Globals.h"
namespace BWAI
{
  //--------------------------------- CONSTRUCTOR -----------------------------
  Mineral::Mineral(BWAI::Unit* mineral, Expansion* expansion)
  :mineral(mineral)
  ,expansion(expansion)
  {
    ai->activeMinerals.push_back(this);
    mineral->expansionAssingment = expansion;
  }
  //--------------------------------- DESTRUCTOR ------------------------------
  Mineral::~Mineral()
  {
    for (unsigned int i = 0; i < this->gatherersAssigned.size(); i++)
    {
      this->gatherersAssigned[i]->expansionAssingment = NULL;
      this->expansion->asignedWorkers --;
    }
  }
  //-------------------------------- ASSIGNE GATHERER -------------------------
  void Mineral::assignGatherer(BWAI::Unit* gatherer)
  {
    //BWAI::ai->log->log("Gatherer assigned %s", gatherer->getName().c_str());
    this->gatherersAssigned.push_back(gatherer);
    gatherer->expansionAssingment = this->expansion;
    this->expansion->asignedWorkers ++;
  }
  //-------------------------------- REMOVE WORKER ----------------------------
  bool Mineral::removeGatherer(Unit* gatherer)
  {
    for (unsigned int i = 0; i < this->gatherersAssigned.size(); i++)
      if (this->gatherersAssigned[i] == gatherer)
      {
        this->gatherersAssigned.erase(this->gatherersAssigned.begin() + i);
        gatherer->expansionAssingment = NULL;
        this->expansion->asignedWorkers --;
        return true;
      }
    return false;
  }
  //---------------------------------------------------------------------------
  bool Mineral::checkAssignedWorkers()
  {
    bool reselected = false;
    for (unsigned int i = 0; i < this->gatherersAssigned.size(); i++)
    {
      Unit* gatherer = this->gatherersAssigned[i];
      if (gatherer->getOrderIDLocal() != BW::OrderID::PlayerGuard &&
          gatherer->getOrderIDLocal() != BW::OrderID::MoveToMinerals &&
          gatherer->getOrderIDLocal() != BW::OrderID::HarvestMinerals2 &&
          gatherer->getOrderIDLocal() != BW::OrderID::MiningMinerals &&
          gatherer->getOrderIDLocal() != BW::OrderID::ResetCollision2 &&
          gatherer->getOrderIDLocal() != BW::OrderID::ReturnMinerals)
      {
        this->removeGatherer(gatherer);
        ai->expansionsSaturated = false;
        i--;
      }
      else
      {
        Unit* miningBuddy = this->currentlyMining();
        /*if (gatherer->selected)
        {
          char message[100];
          sprintf(message, "Distance = %d", gatherer->getDistance(this->mineral));
          BWAPI::Game::print(message);
        }*/
        if (
             (
               gatherer->getOrderIDLocal() == BW::OrderID::MoveToMinerals || 
               gatherer->getOrderIDLocal() == BW::OrderID::HarvestMinerals2 || 
               gatherer->getOrderIDLocal() == BW::OrderID::PlayerGuard
             ) &&
             gatherer->getOrderID() != BW::OrderID::MiningMinerals &&
             gatherer->getTargetLocal() != this->expansion->gatherCenter &&
             (
               gatherer->getTargetLocal() != this->mineral
               ||
               (
                 gatherer->getDistance(this->mineral) <= BWAPI::Broodwar.getLatency()*3 &&
                 miningBuddy != NULL &&
                 miningBuddy->getOrderTimer() >= BWAPI::Broodwar.getLatency() &&
                 (
                   miningBuddy->getOrderTimer() == BWAPI::Broodwar.getLatency()-1 ||
                   BWAPI::Broodwar.frameCount - gatherer->lastFrameSpam > 4
                 )
               )
             )
           )
        {
          gatherer->lastFrameSpam = BWAPI::Broodwar.frameCount;
          gatherer->orderRightClick(mineral);
          reselected = true;
        }
      }
    }
    return reselected;
  }
  //---------------------------------------------------------------------------
  Unit* Mineral::currentlyMining(void)
  {
   for (unsigned int i = 0; i < this->gatherersAssigned.size(); i++)
    if (this->gatherersAssigned[i]->getOrderID() == BW::OrderID::MiningMinerals)
       return this->gatherersAssigned[i];
   return NULL;      
  }
  //---------------------------------------------------------------------------
}
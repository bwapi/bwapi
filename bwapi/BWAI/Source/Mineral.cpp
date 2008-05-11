#include "Mineral.h"
#include "Unit.h"
#include "Expansion.h"
#include "Globals.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototype.h" /** @todo remove, just for debug*/
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
  //-------------------------------- ASSIGNE GATHERER -------------------------
  void Mineral::assignGatherer(BWAI::Unit* gatherer)
  {
    this->gatherersAssigned.push_back(gatherer);
    gatherer->expansionAssingment = this->expansion;
    this->expansion->asignedWorkers ++;
    gatherer->orderRightClick(this->mineral);
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
      if (gatherer->getOrderIDLocal() != BW::OrderID::ApproachingMinerals &&
          gatherer->getOrderIDLocal() != BW::OrderID::StartingMining &&
          gatherer->getOrderIDLocal() != BW::OrderID::Mining &&
          gatherer->getOrderIDLocal() != BW::OrderID::ReturningMinerals &&
          gatherer->getOrderIDLocal() != BW::OrderID::GettingMinedMinerals)
      {
        this->removeGatherer(gatherer);
        ai->expansionsSaturated = false;
        i--;
      }
      else
        if (
             (
               gatherer->getOrderIDLocal() == BW::OrderID::ApproachingMinerals || 
               gatherer->getOrderIDLocal() == BW::OrderID::StartingMining || 
               gatherer->getOrderIDLocal() == BW::OrderID::Idle
              ) &&
               gatherer->getOrderID() != BW::OrderID::Mining &&
              (
                gatherer->getTargetLocal() != this->mineral ||
                (
                  gatherer->getDistance(this->mineral) <= 3 &&
                  this->SomeoneIsMining()
                )
              )
            )
        {
          gatherer->orderRightClick(mineral);
          reselected = true;
        }
    }
    return reselected;
  }
  //---------------------------------------------------------------------------
  bool Mineral::SomeoneIsMining(void)
  {
   for (unsigned int i = 0; i < this->gatherersAssigned.size(); i++)
    if (this->gatherersAssigned[i]->getOrderID() == BW::OrderID::Mining)
       return true;
   return false;      
  }
  //---------------------------------------------------------------------------
  void Mineral::checkDeadWorkers(void)
  {
    for (unsigned int i = 0; i < this->gatherersAssigned.size(); i++)
      if (!this->gatherersAssigned[i]->isReady())
      {
        this->gatherersAssigned.erase(this->gatherersAssigned.begin() + i);
        this->expansion->asignedWorkers --;
      }
  }
  //---------------------------------------------------------------------------
}
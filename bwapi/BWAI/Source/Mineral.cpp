#include "Mineral.h"
#include "Unit.h"
#include "Expansion.h"
#include "Globals.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototype.h" /** @todo remove, just for debug*/
#include "..//..//BWAPI//Source//BWAPI//Globals.h" /** @todo remove, just for debug*/
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
      if (gatherer->getOrderIDLocal() != BW::OrderID::ApproachingMinerals &&
          gatherer->getOrderIDLocal() != BW::OrderID::StartingMining &&
          gatherer->getOrderIDLocal() != BW::OrderID::Mining &&
          gatherer->getOrderIDLocal() != BW::OrderID::ReturningMinerals &&
          gatherer->getOrderIDLocal() != BW::OrderID::GettingMinedMinerals &&
          gatherer->getOrderIDLocal() != BW::OrderID::Idle)
      {
        this->reassignationReasons.log("%s removed from gather - other task", gatherer->getName().c_str());
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
                gatherer->getTargetLocal() != this->mineral &&
                gatherer->getTargetLocal() != gatherer->expansionAssingment->gatherCenter
                ||
                (
                  gatherer->getDistance(this->mineral) <= 3 &&
                  this->SomeoneIsMining()
                )
              )
            )
        {
          if (gatherer->getTargetLocal() != this->mineral)
          {
            
           this->reassignationReasons.log("(%4d) ---------- bad mineral target ------------------------------", BWAPI::Broodwar.frameCount);
           this->reassignationReasons.log("%s", gatherer->getName().c_str());
           if (gatherer->getTargetLocal() != NULL)
             this->reassignationReasons.log("%s", gatherer->getTargetLocal()->getName().c_str());
           else
             this->reassignationReasons.log("NULL target");
           this->reassignationReasons.log("%s", this->mineral->getName().c_str());
          }
         else
          {
            this->reassignationReasons.log("(%4d) ---------- Too close to mineral mined by other worker ----------------", BWAPI::Broodwar.frameCount);
            this->reassignationReasons.log("%s", gatherer->getName().c_str());
            this->reassignationReasons.log("%s\n", gatherer->getTargetLocal()->getName().c_str());
          }
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
  Logger Mineral::reassignationReasons = Logger("reasignation-reasons", LogLevel::MicroDetailed);
}
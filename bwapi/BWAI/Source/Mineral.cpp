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
            FILE* f = fopen("reasignation-reasons.log","at");
            fprintf(f, "(%4d) ---------- bad mineral target ------------------------------\n", BWAPI::Broodwar.frameCount);
            fprintf(f, "%s\n", gatherer->getName().c_str());
            if (gatherer->getTargetLocal() != NULL)
              fprintf(f, "%s\n%s\n", gatherer->getTargetLocal()->getName().c_str(), 
                                     this->mineral->getName().c_str());
            else
              fprintf(f, "NULL target\n%s\n", this->mineral->getName().c_str());
            fclose(f);
          }
         else
          {
            FILE* f = fopen("reasignation-reasons.log","at");
            fprintf(f, "(%4d) ---------- Too close to mineral mined by other worker ----------------\n", BWAPI::Broodwar.frameCount);
            fprintf(f, "%s\n", gatherer->getName().c_str());
            fprintf(f, "%s\n", gatherer->getTargetLocal()->getName().c_str());
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
}
#include "TaskBuild.h"

#include <Util/Logger.h>
#include <Util/Types.h>
#include <BW/UnitType.h>
#include <BW/OrderID.h>
#include <BWAPI/Player.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Game.h>

#include "Unit.h"
#include "AI.h"
#include "Globals.h"
#include "BuildingPositionSet.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskBuild::TaskBuild(BW::UnitType buildingType, 
                       BuildingPosition* position, 
                       Unit* builder, 
                       BuildingPositionSet* alternatives, 
                       u16 priority)
  :Task(builder, priority)
  ,buildingType(buildingType)
  ,position(position)
  ,building(NULL)
  ,alternatives(alternatives)
  ,spot(BW::TilePosition::Invalid)
  {
    if (position != NULL)
      position->reserved = true;
    BWAI::ai->prioritisedTasks.insert(this);
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskBuild::TaskBuild(BW::UnitType buildingType,
                       Unit* builder,
                       BW::TilePosition spot,
                       u16 priority)
  :Task(builder, priority)
  ,buildingType(buildingType)
  ,position(NULL)
  ,building(NULL)
  ,alternatives(NULL)
  ,spot(spot)
  {
    BWAI::ai->prioritisedTasks.insert(this);
  }  
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  TaskBuild::~TaskBuild()
  {
    if (position != NULL)
      position->reserved = false;
    BWAI::ai->prioritisedTasks.erase(this);
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  bool TaskBuild::execute()
  {
    if (BWAI::ai->buildTaskUnitsPlanned[(u16)this->buildingType.getID()] - BWAPI::Broodwar.BWAPIPlayer->getCompletedUnits(this->buildingType) > 0)
    {
      if (!this->executors.empty() &&
          this->building != NULL &&
          this->building->isCompleted())
      {
        BWAI::ai->log->logCritical("(%s) finished production of (%s)", this->executors.front()->getType().getName(), this->buildingType.getName());
        return true;
      }
      if (this->executors.empty() &&
          !this->buildingType.isAddon() &&
          this->position != NULL)
      {
        Unit* builder = ai->freeBuilder(this->position->position);
        if (builder)
          this->addExecutor(builder);
      }
      if (this->building == NULL && 
          !this->executors.empty())
        if (!this->buildingType.isAddon())
        {
          if (
            this->executors.front()->getOrderTarget() != NULL &&
            this->executors.front()->getOrderID() == BW::OrderID::ConstructingBuilding &&
            this->executors.front()->getOrderTarget()->getType() == buildingType)
          {
            this->building = executors.front()->getOrderTarget();
            BWAI::ai->log->logCritical("(%s) construction started", building->getName().c_str());
          }
        }
        else
          if (this->executors.front()->getBuildUnit() != NULL &&
              this->executors.front()->getBuildUnit()->getType() == buildingType)
          {
            this->building = BWAI::Unit::BWAPIUnitToBWAIUnit(executors.front()->getBuildUnit());
            BWAI::ai->log->logCritical("(%s) construction of addon started", building->getName().c_str());
          }

      if (!this->executors.empty() &&
          this->spot.isValid())
      {
        if (this->building == NULL &&
            this->executors.front()->getOrderIDLocal() == BW::OrderID::Nothing2 &&
            BWAI::ai->player->canAfford(this->buildingType, BWAPI::ReservedResources()))
          this->executors.front()->build(this->spot, this->getBuildingType());
        return false;
      }
          
          
      if (this->alternatives == NULL && 
          this->building == NULL &&
          this->spot == BW::TilePosition::Invalid)
        return true; // Special case of the custom building    

      if (this->building != NULL &&
          !this->executors.empty() &&
          this->executors.front()->getOrderTargetLocal() != this->building &&
          !this->building->isCompleted() &&
          !this->buildingType.isAddon())
      {
         this->executors.front()->orderRightClick(this->building);
         BWAI::ai->log->logCritical("(%s) Builder died - new builder sent to finish", building->getName().c_str());
      }
      
      if (!this->executors.empty() && this->building == NULL)
      {
        if ((this->position == NULL ||
            !this->canIBuild(this->position->position)) &&
            !this->buildingType.isAddon())
        {
          if (this->position != NULL)
          {
            this->position->reserved = false;
            this->position = NULL;
          }
          for each (BuildingPosition* i in alternatives->positions)
            if (i->reserved == false &&
                this->canIBuild(i->position))
            {
              this->position = i;
              this->position->reserved = true;
              break;
            }
          if (this->position == NULL)
            return false;
        }
        BW::Position center(this->position->position);
        center.x += (BW::TILE_SIZE*this->getBuildingType().getTileWidth())/2;
        center.y += (BW::TILE_SIZE*this->getBuildingType().getTileHeight())/2;
        if (this->position != NULL)
          // Note that the auto conversion constructor is used here, so it takes care of conversion between tile position and position
          if (this->executors.front()->getDistance(center) > 100 &&
              !this->buildingType.isAddon())
          {
            if (
                 (
                   this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildTerran &&
                   this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
                   this->executors.front()->getOrderIDLocal() != BW::OrderID::DroneStartBuild
                 ) &&
                 (
                    this->executors.front()->getOrderIDLocal() != BW::OrderID::Move ||
                    this->executors.front()->getTargetPositionLocal().getDistance(center) > 300
                 )
               )
            {
              this->executors.front()->orderRightClick(center);
              BWAI::ai->log->logCritical("(%s) sent to build (%s) at (%d,%d)", this->executors.front()->getName().c_str(), buildingType.getName(), center.x, center.y);
            }
          }
          else
            if (!this->buildingType.isAddon())
            {
              if (this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildTerran &&
                  this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
                  this->executors.front()->getOrderIDLocal() != BW::OrderID::DroneStartBuild &&
                  this->executors.front()->getOwner()->canAfford(buildingType, BWAPI::ReservedResources()))
              {
                BWAI::ai->log->logCritical("(%s) ordered to build (%s)", this->executors.front()->getName().c_str(), buildingType.getName());
                this->executors.front()->build(this->position->position, buildingType);
              }
            }
            else
              if (this->executors.front()->getSecondaryOrderIDLocal() != BW::OrderID::PlaceAddon &&
                  this->executors.front()->hasEmptyBuildQueueLocal() &&
                  this->executors.front()->getSecondaryOrderIDLocal() != BW::OrderID::BuildAddon)
              {
                BWAI::ai->log->logCritical("(%s) ordered to build addon (%s)", this->executors.front()->getName().c_str(), buildingType.getName());
                BWAI::ai->log->log("secondary order id local = %d", this->executors.front()->getSecondaryOrderIDLocal());
                this->executors.front()->build(this->position->position, buildingType);
              }
      }
      return false;
    }
    else
      return true;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  TaskType::Enum TaskBuild::getType()
  {
    return TaskType::Build;
  }
  //---------------------------------------------- GET MINERAL -----------------------------------------------
  BW::UnitType TaskBuild::getBuildingType()
  {
    return this->buildingType;
  }
  //----------------------------------------------------------------------------------------------------------
  bool TaskBuild::canIBuild(BW::TilePosition here)
  {
    for (int k = here.x; 
         k < here.x + this->alternatives->tileWidth; 
              k++)
      for (int l = here.y; 
           l < here.y + this->alternatives->tileHeight; 
           l++)
        if (BWAPI::Broodwar.unitsOnTile(k,l).empty() == false &&
             (
               BWAPI::Broodwar.unitsOnTile(k,l).front()->getIndex() != this->executors.front()->getIndex() &&
               BWAPI::Broodwar.unitsOnTile(k,l).front()->getType() != BW::UnitID::Resource_VespeneGeyser ||
               BWAPI::Broodwar.unitsOnTile(k,l).size() != 1
             )
           )
          return false;
    return true;
  }
  //----------------------------------------------------------------------------------------------------------
  Unit* TaskBuild::getBuilding()
  {
    return this->building;
  }
  //----------------------------------------------------------------------------------------------------------
  BWAPI::ReservedResources TaskBuild::getReserved()
  {
    if (this->building == NULL)
      return BWAPI::ReservedResources(this->buildingType.getMineralPrice(), this->buildingType.getGasPrice(), 0);
    return BWAPI::ReservedResources();
  }
  //----------------------------------------------------------------------------------------------------------
  void TaskBuild::buildingDied()
  {
    this->building = NULL;
  }
  //----------------------------------------------------------------------------------------------------------
}
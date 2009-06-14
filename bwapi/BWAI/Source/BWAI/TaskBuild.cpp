#include "TaskBuild.h"

#include <Util/Logger.h>
#include <Util/Types.h>
#include <BWAPI.h>
#include <BWAI/Globals.h>
#include <BWAI/Player.h>
#include <BWAI/ReservedResources.h>

#include "Unit.h"
#include "AI.h"
#include "Globals.h"
#include "BuildingPositionSet.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskBuild::TaskBuild(BWAPI::UnitType buildingType, 
                       BuildingPosition* position, 
                       Unit* builder, 
                       BuildingPositionSet* alternatives, 
                       u16 priority)
  :Task(builder, priority)
  ,buildingType(buildingType)
  ,position(position)
  ,building(NULL)
  ,alternatives(alternatives)
  ,spot(BWAPI::TilePositions::Invalid)
  {
    if (position != NULL)
      position->reserved = true;
    BWAI::ai->prioritisedTasks.insert(this);
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskBuild::TaskBuild(BWAPI::UnitType buildingType,
                       Unit* builder,
                       BWAPI::TilePosition spot,
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
    if (BWAI::ai->buildTaskUnitsPlanned[(u16)this->buildingType.getID()] - BWAI::ai->player->getCompletedUnits(this->buildingType) > 0)
    {
      if (!this->executors.empty() &&
          this->building != NULL &&
          this->building->isCompleted())
      {
        BWAI::ai->log->logCritical("(%s) finished production of (%s)", this->executors.front()->getType().getName().c_str(), this->buildingType.getName().c_str());
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
            this->executors.front()->getOrder() == BWAPI::Orders::ConstructingBuilding &&
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
            this->building = executors.front()->getBuildUnit();
            BWAI::ai->log->logCritical("(%s) construction of addon started", building->getName().c_str());
          }

      if (!this->executors.empty() &&
          this->spot.isValid())
      {
        if (this->building == NULL &&
            this->executors.front()->getOrder() == BWAPI::Orders::Nothing2 &&
            BWAI::ai->player->canAfford(this->buildingType))
          this->executors.front()->build(this->spot, this->getBuildingType());
        return false;
      }
          
          
      if (this->alternatives == NULL && 
          this->building == NULL &&
          this->spot == BWAPI::TilePositions::Invalid)
        return true; // Special case of the custom building    

      if (this->building != NULL &&
          !this->executors.empty() &&
          this->executors.front()->getOrderTarget() != this->building &&
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
        BWAPI::Position center(this->position->position);
        center.x += (BWAPI::TILE_SIZE*this->getBuildingType().tileWidth())/2;
        center.y += (BWAPI::TILE_SIZE*this->getBuildingType().tileHeight())/2;
        if (this->position != NULL)
          // Note that the auto conversion constructor is used here, so it takes care of conversion between tile position and position
          if (this->executors.front()->getDistance(center) > 100 &&
              !this->buildingType.isAddon())
          {
            if (
                 (
                   this->executors.front()->getOrder() != BWAPI::Orders::BuildTerran &&
                   this->executors.front()->getOrder() != BWAPI::Orders::BuildProtoss1 &&
                   this->executors.front()->getOrder() != BWAPI::Orders::DroneStartBuild
                 ) &&
                 (
                    this->executors.front()->getOrder() != BWAPI::Orders::Move ||
                    this->executors.front()->getTargetPosition().getDistance(center) > 300
                 )
               )
            {
              this->executors.front()->orderRightClick(center);
              BWAI::ai->log->logCritical("(%s) sent to build (%s) at (%d,%d)", this->executors.front()->getName().c_str(), buildingType.getName().c_str(), center.x, center.y);
            }
          }
          else
            if (!this->buildingType.isAddon())
            {
              if (this->executors.front()->getOrder() != BWAPI::Orders::BuildTerran &&
                  this->executors.front()->getOrder() != BWAPI::Orders::BuildProtoss1 &&
                  this->executors.front()->getOrder() != BWAPI::Orders::DroneStartBuild &&
                  this->executors.front()->getOwner()->canAffordNow(buildingType))
              {
                BWAI::ai->log->logCritical("(%s) ordered to build (%s)", this->executors.front()->getName().c_str(), buildingType.getName().c_str());
                this->executors.front()->build(this->position->position, buildingType);
              }
            }
            else
              if (this->executors.front()->getSecondaryOrder() != BWAPI::Orders::PlaceAddon &&
                  !this->executors.front()->isTraining() &&
                  this->executors.front()->getSecondaryOrder() != BWAPI::Orders::BuildAddon)
              {
                BWAI::ai->log->logCritical("(%s) ordered to build addon (%s)", this->executors.front()->getName().c_str(), buildingType.getName().c_str());
                BWAI::ai->log->log("secondary order id local = %d", this->executors.front()->getSecondaryOrder());
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
  BWAPI::UnitType TaskBuild::getBuildingType()
  {
    return this->buildingType;
  }
  //----------------------------------------------------------------------------------------------------------
  bool TaskBuild::canIBuild(BWAPI::TilePosition here)
  {
    for (int k = here.x; 
         k < here.x + this->alternatives->tileWidth; 
              k++)
      for (int l = here.y; 
           l < here.y + this->alternatives->tileHeight; 
           l++)
        if (BWAPI::Broodwar->unitsOnTile(k,l).empty() == false &&
             (
               (*BWAPI::Broodwar->unitsOnTile(k,l).begin()) != this->executors.front()->getUnit() &&
               (*BWAPI::Broodwar->unitsOnTile(k,l).begin())->getType() != BWAPI::UnitTypes::Resource_Vespene_Geyser ||
               BWAPI::Broodwar->unitsOnTile(k,l).size() != 1
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
  BWAI::ReservedResources TaskBuild::getReserved()
  {
    if (this->building == NULL)
      return BWAI::ReservedResources(this->buildingType.mineralPrice(), this->buildingType.gasPrice(), 0);
    return BWAI::ReservedResources();
  }
  //----------------------------------------------------------------------------------------------------------
  void TaskBuild::buildingDied()
  {
    this->building = NULL;
  }
  //----------------------------------------------------------------------------------------------------------
}
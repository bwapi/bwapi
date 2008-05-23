#include "TaskBuild.h"

#include <Logger.h>

#include "Unit.h"
#include "AI.h"
#include "Globals.h"

#include "../../BWAPI/Source/Types.h"
#include "../../BWAPI/Source/BW/UnitType.h"
#include "../../BWAPI/Source/BW/OrderID.h"
#include "../../BWAPI/Source/BW/Unit.h"

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskBuild::TaskBuild(BW::UnitType buildingType, BW::TilePosition position, Unit* builder)
  :Task(builder)
  ,buildingType(buildingType)
  ,position(position)
  ,building(NULL)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskBuild::~TaskBuild()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskBuild::execute()
  {
    if (!this->executors.empty() &&
        this->building != NULL &&
        this->building->isCompleted())
    {
      BWAI::ai->log->log("(%s) finished production of (%s)", this->executors.front()->getType().getName(), this->buildingType.getName());
      return true;
    }
    if (this->executors.empty())
    {
      Unit* builder = ai->freeBuilder(this->position);
      if (builder)
        this->addExecutor(builder);
    }
    if (!this->executors.empty())
    {
      BW::Position center = this->position;
      center.x += (u16)(BW::TileSize*1.5);
      center.y += BW::TileSize;
      // Note that the auto conversion constructor is used here, so it takes care of conversion between tile position and position
      if (this->executors.front()->getDistance(center) > 100)
      {
        if (this->executors.front()->getOrderIDLocal() != BW::OrderID::Move ||
            this->executors.front()->getDistance(center) > 300)
        {
          this->executors.front()->orderRightClick(center);
          BWAI::ai->log->log("(%s) sent to building position (%d,%d)", buildingType.getName(), center.x, center.y);
        }
      }
      else
        if (this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildTerran &&
            this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
            this->executors.front()->getOrderIDLocal() != BW::OrderID::DroneStartBuild &&
            this->executors.front()->getOwner()->getMinerals() >= buildingType.getMineralPrice() &&
            this->executors.front()->getOwner()->getGas() >= buildingType.getGasPrice())
          this->executors.front()->build(this->position, buildingType);
      if (this->building == NULL && 
          this->executors.front()->getOrderTarget() != NULL &&
          this->executors.front()->getOrderID() == BW::OrderID::ConstructingBuilding)
      {
        this->building = executors.front()->getOrderTarget();
        BWAI::ai->log->log("(%s) construction started", building->getName().c_str());
      }
    }
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskBuild::getType()
  {
    return TaskType::Build;
  }
  //------------------------------- GET MINERAL -------------------------------
  BW::UnitType TaskBuild::getBuildingType()
  {
    return this->buildingType;
  }
  //---------------------------------------------------------------------------
}
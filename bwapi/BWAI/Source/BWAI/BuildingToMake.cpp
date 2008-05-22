#include "BuildingToMake.h"

#include <Logger.h>

#include "Unit.h"
#include "AI.h"
#include "Globals.h"
#include "TaskBuild.h"

#include "../../BWAPI/Source/BW/UnitType.h"
#include "../../BWAPI/Source/Types.h"
#include "../../BWAPI/Source/BW/OrderID.h"
#include "../../BWAPI/Source/BW/Unit.h"


namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  BuildingToMake::BuildingToMake(Unit* builder, BW::UnitType buildingType, BW::TilePosition position)
  :builder(builder)
  ,buildingType(buildingType)
  ,position(position)
  ,building(0)
  {
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  BuildingToMake::~BuildingToMake()
  {
  }
  //--------------------------------- EXECUTE ---------------------------------
  bool BuildingToMake::execute()
  {
    if (this->builder!= NULL &&
        this->building != NULL &&
        this->building->isCompleted())
    {
      BWAI::ai->log->log("(%s) finished production of (%s)", this->builder->getType().getName(), this->buildingType.getName());
      if (builder->getTask() &&
          builder->getTask()->getType() == TaskType::Build)
        ((TaskBuild*)builder->getTask())->clearBuildingToMakePointer();
      return true;
    }
    if (this->builder == NULL)
      this->setBuilder(ai->freeBuilder(position));
    if (this->builder)
    {
      BW::Position center = this->position;
      center.x += (u16)(BW::TileSize*1.5);
      center.y += BW::TileSize;
      // Note that the auto conversion constructor is used here, so it takes care of conversion between tile position and position
      if (this->builder->getDistance(center) > 100)
      {
        if (this->builder->getOrderIDLocal() != BW::OrderID::Move ||
            this->builder->getDistance(center) > 300)
        {
          this->builder->orderRightClick(center);
          BWAI::ai->log->log("(%s) sent to building position (%d,%d)", buildingType.getName(), center.x, center.y);
        }
      }
      else
        if (this->builder->getOrderIDLocal() != BW::OrderID::BuildTerran &&
            this->builder->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
            this->builder->getOrderIDLocal() != BW::OrderID::DroneStartBuild)
          this->builder->build(this->position, buildingType);
      if (this->building == NULL && 
          this->builder->getOrderTarget() != NULL &&
          this->builder->getOrderID() == BW::OrderID::ConstructingBuilding)
      {
        this->building = builder->getOrderTarget();
        BWAI::ai->log->log("(%s) construction started", building->getName().c_str());
      }
    }
    return false;
  }
  //---------------------------------- GET TYPE -------------------------------
  BW::UnitType BuildingToMake::getType()
  {
     return this->buildingType;
  }
  //--------------------------------- SET BUILDER -----------------------------
  void BuildingToMake::setBuilder(Unit* builder)
  {
    this->builder = builder;
    if (this->builder != NULL)
      this->builder->setTask(new TaskBuild(this->builder, this));
  }
  //---------------------------------------------------------------------------
}

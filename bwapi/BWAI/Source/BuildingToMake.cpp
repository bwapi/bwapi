#include "BuildingToMake.h"

#include <Logger.h>

#include "Unit.h"
#include "AI.h"
#include "Globals.h"

#include "../../BWAPI/Source/BW/UnitType.h"
#include "../../BWAPI/Source/Types.h"
#include "../../BWAPI/Source/BW/OrderID.h"
#include "../../BWAPI/Source/BW/Unit.h"


namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  BuildingToMake::BuildingToMake(Unit* builder, BW::UnitType buildingType, BW::Position position)
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
/*    if (builder)
      BWAI::ai->log->log("BuildingToMake::execute (%s)", this->builder->getName().c_str());*/
    if (this->builder!= NULL &&
        this->building != NULL &&
        this->building->isCompleted())
    {
       BWAI::ai->log->log("(%s) finished production of (%s)", this->builder->getType().getName(), this->buildingType.getName());
      return true;
    }
    if (this->builder == NULL)
      this->builder = ai->freeBuilder();
    if (this->builder)
    {
      //BWAI::ai->log->log("Distance to building = (%d)", this->builder->getDistance(BW::Position(position.x*BW::TileSize, position.y*BW::TileSize)));
      if (this->builder->getDistance(BW::Position(position.x*BW::TileSize, position.y*BW::TileSize)) > 100)
      {
        if (this->builder->getOrderIDLocal() != BW::OrderID::Move ||
            this->builder->getTargetPositionLocal() != BW::Position(position.x*BW::TileSize, position.y*BW::TileSize))
        {
          this->builder->orderRightClick(BW::Position(position.x*BW::TileSize, position.y*BW::TileSize));
          BWAI::ai->log->log("(%s) sent to building position (%d,%d)", buildingType.getName(), position.x*BW::TileSize, position.y*BW::TileSize);
        }
      }
      else
        if (this->builder->getOrderIDLocal() != BW::OrderID::BuildTerran &&
            this->builder->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
            this->builder->getOrderIDLocal() != BW::OrderID::DroneStartBuild)
          this->builder->build(this->position.x, this->position.y, buildingType);
      /*BWAI::ai->log->log("one more before condition (%s)", this->builder->getName().c_str());
      BWAI::ai->log->log("Before condition this->building == NULL %d", this->building == NULL);
      BWAI::ai->log->log("Before condition this->builder->getOrderTarget() != NULL %d", this->builder->getOrderTarget() != NULL);
      BWAI::ai->log->log("Before condition this->builder->getOrderID() == BW::OrderID::BuildTerran %d", this->builder->getOrderID() == BW::OrderID::BuildTerran);*/
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
  //---------------------------------------------------------------------------
}

#include "BuildingToMake.h"
#include "Unit.h"
#include "AI.h"
#include "Globals.h"
#include "../../Util/Logger.h"
#include "../../BWAPI/Source/BWAPI/UnitPrototype.h"
#include "../../BWAPI/Source/Types.h"
#include "../../BWAPI/Source/BW/OrderID.h"
#include "../../BWAPI/Source/BW/Unit.h"


namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  BuildingToMake::BuildingToMake(Unit* builder, BWAPI::UnitPrototype* buildingType, BW::Position position)
  :builder(builder)
  ,buildingType(buildingType)
  ,position(position)
  ,building(0)
  {
  }
  //--------------------------------- EXECUTE ---------------------------------
  void BuildingToMake::execute()
  {
    if (this->builder)
    {
      if (this->builder->getOrderIDLocal() != BW::OrderID::BuildTerran &&
          this->builder->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
          this->builder->getOrderIDLocal() != BW::OrderID::DroneStartBuild)
        this->builder->build(this->position.x, this->position.y, buildingType);
      if (!this->building && builder->getRawData()->currentBuildUnit)
      {
        this->building = BWAI::Unit::BWUnitToBWAIUnit(builder->getRawData()->currentBuildUnit);
        BWAI::ai->log->log("(%s) construction started", building->getName().c_str());
      }
    }
  }
  //---------------------------------------------------------------------------
}

#include "CommandBuild.h"

#include <Exceptions.h>
#include <StringUtil.h>
#include <Logger.h>

#include "../BWAI/AI.h"
#include "../BWAI/TaskBuild.h"
#include "../BWAI/Globals.h"

#include "../../../BWAPI/Source/BWAPI/Player.h"
#include "../../../BWAPI/Source/BW/UnitType.h"

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  CommandBuild::CommandBuild(TiXmlElement* xmlElement)
  {
    const char * buildName = xmlElement->Attribute("name");
    if (buildName == NULL)
      throw XmlException("Expected attribute name in <build> element");
    this->name = buildName;
    
    const char * buildPlace = xmlElement->Attribute("place");
    if (buildPlace == NULL)
      throw XmlException("Expected attribute place in <build> element");
    this->place = buildPlace;
    
    const char * minimalPopulationAttribute = xmlElement->Attribute("minimal-population");
    if (minimalPopulationAttribute == NULL)
      throw XmlException("Expected attribute minimal-population in <build> element");
    this->minimalPopulation = StringUtil::stringToInt(minimalPopulationAttribute);
  }
  //---------------------------------------------------------------------------
  bool CommandBuild::execute()
  {
    //BWAI::ai->log->log("Checking what to build " + this->name);
    BW::UnitType toBuild = BWAI::ai->unitNameToType[this->name];
    //BWAI::ai->log->log("Need mineral price = %d, gasPrice =%d, suppliesCondition=%d", toBuild.getMineralPrice(), toBuild.getGasPrice(), this->minimalPopulation);
    //BWAI::ai->log->log("Have mineral price = %d, gasPrice =%d, suppliesCondition=%d", BWAI::ai->player->getMineralsLocal(), BWAI::ai->player->getGasLocal(), BWAI::ai->player->usedSuppliesLocal(toBuild.getRace()));
    if (BWAI::ai->player->getMineralsLocal() >= toBuild.getMineralPrice() &&
        BWAI::ai->player->getGasLocal() >= toBuild.getGasPrice() &&
        BWAI::ai->player->usedSuppliesLocal(toBuild.getRace()) >= 2*this->minimalPopulation)
    {
      BWAI::ai->log->log("Conditions to build " + this->name + "fullfiled starting to build");
      BWAI::Unit* scvToUse;
      BWAI::BuildingPosition* alternatives = BWAI::ai->getPositionsCalled(this->place); 
      BW::TilePosition position = BWAI::ai->getFreeBuildingSpot(this->place, scvToUse); 
      
      if (position.isValid())
        BWAI::ai->plannedBuildings.push_back(new BWAI::TaskBuild(toBuild, position, scvToUse, alternatives));
      else
        BWAI::ai->log->log("Didn't find spot to build " + this->name);
      return true;

    }
    return false;
  }
  //---------------------------------------------------------------------------
}
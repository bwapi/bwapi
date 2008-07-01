#include "ConditionMinimalPopulation.h"

#include <tinyxml.h>
#include <Util/Strings.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>

#include <BWAI/Globals.h>

#include <BWAPI/Player.h>

namespace BuildOrder
{
  //---------------------------------------- CONSTRUCTOR -----------------------------------------------------
  ConditionMinimalPopulation::ConditionMinimalPopulation(u16 minimalPopulation)
  :minimalPopulation(minimalPopulation)
  {
  }
  //---------------------------------------- CONSTRUCTOR -----------------------------------------------------
  ConditionMinimalPopulation::ConditionMinimalPopulation(TiXmlElement* element)
  {
    this->minimalPopulation = Util::Xml::getRequiredU16Attribute(element, "population");
  }
  //------------------------------------------ GET TYPE ------------------------------------------------------
  ConditionType::Enum ConditionMinimalPopulation::getType()
  {
    return ConditionType::MinimalPopulation;
  }
  //------------------------------------------ APPLIES -------------------------------------------------------
  bool ConditionMinimalPopulation::applies()
  {
    return BWAI::ai->player->getSuppliesUsedLocal(BWAI::ai->player->getRace()) >= 2*this->minimalPopulation;
  }
}
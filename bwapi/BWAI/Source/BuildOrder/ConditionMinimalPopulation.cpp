#include "ConditionMinimalPopulation.h"

#include <tinyxml.h>
#include <Util/Strings.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>
#include <BWAI/Globals.h>
#include <BWAI/Player.h>

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  ConditionMinimalPopulation::ConditionMinimalPopulation(u16 minimalPopulation)
  :minimalPopulation(minimalPopulation)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  ConditionMinimalPopulation::ConditionMinimalPopulation(TiXmlElement* element)
  {
    this->minimalPopulation = Util::Xml::getRequiredU16Attribute(element, "population");
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  ConditionType::Enum ConditionMinimalPopulation::getType()
  {
    return ConditionType::MinimalPopulation;
  }
  //------------------------------------------------ APPLIES -------------------------------------------------
  bool ConditionMinimalPopulation::applies()
  {
    return BWAI::ai->player->getSuppliesUsed(BWAI::ai->player->getRace()) >= 2*this->minimalPopulation;
  }
  //-------------------------------------------- DEBUG EVALUATE ----------------------------------------------
  void ConditionMinimalPopulation::debugEvaluate(std::string& view)
  {
    view += Util::Strings::ssprintf(" MinimalPopulation(%d)=%d ", 
                             2*this->minimalPopulation, 
                             BWAI::ai->player->getSuppliesUsed(BWAI::ai->player->getRace()));
  }
  //----------------------------------------------------------------------------------------------------------
}
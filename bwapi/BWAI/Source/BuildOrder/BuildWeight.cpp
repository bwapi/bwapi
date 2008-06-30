#include "BuildWeight.h"
#include <BWAPI/Globals.h>
#include <Util/Xml.h>

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BuildWeight::BuildWeight(TiXmlElement* element)
  {
    this->unitType = BWAPI::Broodwar.unitNameToType[Util::Xml::getRequiredAttribute(element, "name")];
    this->weight = Util::Xml::getRequiredU16Attribute(element, "weight");
    std::string maxTypeString = Util::Xml::getOptionalAttribute(element, "max-type","");
    if (maxTypeString == "auto-effective-gather")
      this->maxType = UnitMaxType::AutoEffectiveGather;
    else
      this->maxType = UnitMaxType::None;
  }
  //----------------------------------------------------------------------------------------------------------
}
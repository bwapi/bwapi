#include <BWAPI.h>
#include "RootBranch.h"

#include <tinyxml.h>
#include <Util/FileLogger.h>
#include <Util/Xml.h>
#include "CommandBuild.h"
#include "CommandChangeWeights.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  RootBranch::RootBranch(TiXmlElement* xmlElement) : Branch(xmlElement)
  {
    this->race = BWAPI::Races::getRace(Util::Xml::getRequiredAttribute(xmlElement, "race"));
    this->against = BWAPI::Races::getRace(Util::Xml::getRequiredAttribute(xmlElement, "against"));
  }
  //----------------------------------------------------------------------------------------------------------
}
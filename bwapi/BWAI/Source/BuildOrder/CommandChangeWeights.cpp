#include "CommandChangeWeights.h"

#include <tinyxml.h>
#include <Util/Strings.h>
#include <Util/Xml.h>
#include <Util/Logger.h>

#include "Root.h"
#include "BuildWeights.h"
 
#include <BWAI/Globals.h>
#include <BWAI/AI.h>
#include <BWAI/TaskTrain.h>

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  CommandChangeWeights::CommandChangeWeights(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->factory = Util::Xml::getRequiredAttribute(xmlElement, "building");

    for (TiXmlElement* buildElement = xmlElement->FirstChildElement("build"); 
         buildElement != NULL; 
         buildElement = buildElement->NextSiblingElement("build"))
    {
      std::string toBuildAttribute = Util::Xml::getRequiredAttribute(buildElement, "name");
      u16 weight = Util::Xml::getRequiredU16Attribute(buildElement, "weight");
      this->weights.push_back(std::pair<std::string, int>(toBuildAttribute, weight));
    }
  }
  //--------------------------------  DESTRUCTOR ----------------------------
  CommandChangeWeights::~CommandChangeWeights()
  {
  }
  //---------------------------------- EXECUTE ------------------------------
  bool CommandChangeWeights::execute()
  {
    BWAI::ai->root->log->log("Command change weights for '%s' called", this->factory.c_str());
    BuildWeights* weight = new BuildWeights(this->factory, this->weights);
    BWAI::ai->root->weights.insert(std::pair<std::string, BuildWeights* >(this->factory, weight));
    BWAI::ai->plannedUnits.push_back(new BWAI::TaskTrain(weight->factory, weight));
    return true;
  }
  //---------------------------------------------------------------------------
}
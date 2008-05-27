#include "CommandChangeWeights.h"

#include <tinyxml.h>
#include <StringUtil.h>

#include "Root.h"
#include "BuildWeights.h"
 
#include "../BWAI/Globals.h"
#include "../BWAI/AI.h"

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  CommandChangeWeights::CommandChangeWeights(TiXmlElement* xmlElement)
  {
    const char * buildingAttribute = xmlElement->Attribute("building");
    if (buildingAttribute == NULL)
      throw XmlException("Expected attribute 'building' in <set-building-rate> element");
    this->factory = buildingAttribute;

    for (TiXmlElement* buildElement = xmlElement->FirstChildElement("build"); 
         buildElement != NULL; 
         buildElement = buildElement->NextSiblingElement("build"))
    {
      const char* toBuildAttribute = buildElement->Attribute("name");
      if (toBuildAttribute == NULL)
        throw XmlException("Expected attribute 'name' in <build> element");

     const char* weight = buildElement->Attribute("weight");
      if (buildElement == NULL)
        throw XmlException("Expected attribute 'weight' in <build> element");
    
     this->weights.push_back(std::pair<std::string, int>(toBuildAttribute,StringUtil::stringToInt(weight)));
    }
  }
  //--------------------------------  DESTRUCTOR ----------------------------
  CommandChangeWeights::~CommandChangeWeights()
  {
  }
  //---------------------------------- EXECUTE ------------------------------
  bool CommandChangeWeights::execute()
  {
    BWAI::ai->root->log.log("Command change weights for '%s' called", this->factory.c_str());
    BWAI::ai->root->weights.insert(std::pair<std::string, BuildWeights* >(this->factory, new BuildWeights(this->factory, this->weights)));
    return true;
  }
  //---------------------------------------------------------------------------
}
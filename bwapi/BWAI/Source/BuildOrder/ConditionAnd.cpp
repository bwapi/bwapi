#include "ConditionAnd.h"

#include <tinyxml.h>
#include <Util/Strings.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>
#include <Util/Logger.h>
#include <BWAI/Globals.h>
#include <Formula/Base.h>

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  ConditionAnd::ConditionAnd(TiXmlElement* xmlElement)
  {
    for (TiXmlElement* i = xmlElement->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
      this->nested.push_back(Condition::load(i));
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  ConditionAnd::~ConditionAnd()
  {
    for each (Condition* i in this->nested)
      delete i;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  ConditionType::Enum ConditionAnd::getType()
  {
    return ConditionType::And;
  }
  //------------------------------------------------ APPLIES -------------------------------------------------
  bool ConditionAnd::applies()
  {
    for each (Condition* i in this->nested)
      if (!i->applies())
        return false;
    return true;
  }
  //-------------------------------------------- DEBUG EVALUATE ----------------------------------------------
  void ConditionAnd::debugEvaluate(std::string& view)
  {
    for each (Condition* i in this->nested)
    {
      if (i != this->nested.front())
        view += " && ";
      i->debugEvaluate(view);
    }
  }
  //----------------------------------------------------------------------------------------------------------
}
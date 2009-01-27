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
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandChangeWeights::CommandChangeWeights(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->weights = new BuildWeights(xmlElement);
  }
  //----------------------------------------------  DESTRUCTOR -----------------------------------------------
  CommandChangeWeights::~CommandChangeWeights()
  {
    delete this->weights;
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  bool CommandChangeWeights::executeInternal(Executor* executor)
  {
    BWAI::ai->root->log->log("Command change weights for '%s' called", this->weights->factory.getName());
    for (std::list<BWAI::TaskTrain*>::iterator i = BWAI::ai->plannedUnits.begin(); i != BWAI::ai->plannedUnits.end(); ++i)
      if ((*i)->getBuildingType() == this->weights->factory)
      {
         BWAI::ai->plannedUnits.erase(i);
         break;
      }
    BWAI::ai->plannedUnits.push_back(new BWAI::TaskTrain(this->weights, priority));
    return true;
  }
  //----------------------------------------------------------------------------------------------------------
}
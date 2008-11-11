#include "CommandCall.h"

#include <map>
#include <Util/Xml.h>
#include <Util/Logger.h>
#include <BWAI/Globals.h>
#include "CommandPointer.h"
#include "Executor.h"
#include "Root.h"
#include "Condition.h"
#include "Branch.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCall::CommandCall(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCall::CommandCall(const std::string& name)
  :Command()
  ,name(name)
  {
  }  
  //----------------------------------------------------------------------------------------------------------
  bool CommandCall::executeInternal(Executor* executor)
  {
    if (!BWAI::ai->root->functions.count(name))
      throw GeneralException("Unknown function to call :'" + this->name + "'");
    Branch* target = BWAI::ai->root->functions[this->name];
    /*if (target->condition)
    {
      std::string debugResult;
      target->condition->debugEvaluate(debugResult);
      BWAI::ai->root->log->log("DEBUG: %s", debugResult.c_str());
    }*/
    if (!target->condition ||
        target->condition->applies())
      executor->callStack.push_back(CommandPointer(target));
    return true;
  }
  //----------------------------------------------------------------------------------------------------------
}

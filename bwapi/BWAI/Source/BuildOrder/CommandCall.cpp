#include "CommandCall.h"

#include <map>
#include <Util/Xml.h>
#include <BWAI/Globals.h>
#include "CommandPointer.h"
#include "Executor.h"
#include "Root.h"

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  CommandCall::CommandCall(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
  }
  //---------------------------------------------------------------------------
  bool CommandCall::executeInternal(Executor* executor)
  {
    if (!BWAI::ai->root->functions.count(name))
      throw GeneralException("Unknown function to call :'" + this->name + "'");
    Branch* target = BWAI::ai->root->functions[this->name];
    executor->callStack.push_back(CommandPointer(target));
    return true;
  }
  //---------------------------------------------------------------------------
}

#include "CommandRegisterException.h"

#include <map>
#include <Util/Xml.h>
#include <Util/Logger.h>
#include <BWAI/Globals.h>
#include "CommandPointer.h"
#include "Executor.h"
#include "Root.h"
#include "Condition.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandRegisterException::CommandRegisterException(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
  }
  //----------------------------------------------------------------------------------------------------------
  bool CommandRegisterException::executeInternal(Executor* executor)
  {
    if (!BWAI::ai->root->functions.count(name))
      throw GeneralException("Unknown function to register as exception: '" + this->name + "'");
    executor->registeredExceptions.insert(this->name);
    BWAI::ai->root->log->log("Registered exception '%s'", this->name.c_str());
    return true;
  }
  //----------------------------------------------------------------------------------------------------------
}

#include "Branch.h"

#include <tinyxml.h>
#include <Util/FileLogger.h>

#include "CommandBuild.h"
#include "CommandChangeWeights.h"

namespace BuildOrder
{
  //------------------------------- CONSTRUCTOR -------------------------------
  Branch::Branch(TiXmlElement* xmlElement)
  {
    Util::Logger* log = new Util::FileLogger("elements", Util::LogLevel::MicroDetailed);
    for (TiXmlElement* i = xmlElement->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
      if (i->ValueTStr() == "build")
        this->commands.push_back(new CommandBuild(i));
      else if (i->ValueTStr() == "set-building-rate")
        this->commands.push_back(new CommandChangeWeights(i));
      else log->log("Unknown element %s found in <bulid-order>", i->ValueTStr().c_str());
    delete log;
  }
  //---------------------------------------------------------------------------
}
#include "Branch.h"

#include <tinyxml.h>
#include <Logger.h>

#include "CommandBuild.h"
#include "CommandChangeWeights.h"

namespace BuildOrder
{
  //------------------------------- CONSTRUCTOR -------------------------------
  Branch::Branch(TiXmlElement* xmlElement)
  {
    Util::Logger* log = new Util::Logger("elements", LogLevel::MicroDetailed);
    for (TiXmlElement* i = xmlElement->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
      if (i->ValueTStr() == "build")
        this->commands.push_back(new CommandBuild(i));
      else if (i->ValueTStr() == "set-building-rate")
        this->commands.push_back(new CommandChangeWeights(i));
    delete log;
  }
  //---------------------------------------------------------------------------
}
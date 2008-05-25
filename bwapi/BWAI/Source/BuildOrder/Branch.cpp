#include "Branch.h"

#include <tinyxml.h>
#include <Logger.h>

#include "CommandBuild.h"

namespace BuildOrder
{
  //------------------------------- CONSTRUCTOR -------------------------------
  Branch::Branch(TiXmlElement* xmlElement)
  {
    Util::Logger* log = new Util::Logger("elements", LogLevel::MicroDetailed);
    for (TiXmlElement* i = xmlElement->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
      if (i->ValueTStr() == "build")
        this->commands.push_back(new CommandBuild(i));
    delete log;
//    <build name="Terran barracks" minPopulation="11" place="barracks" />
  }
  //---------------------------------------------------------------------------
}
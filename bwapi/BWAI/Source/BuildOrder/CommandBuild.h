#include "Command.h"

#include <string>

#include <tinyxml.h>

namespace BuildOrder
{
  class CommandBuild : public Command
  {
    public :
      CommandBuild(TiXmlElement* xmlElement);
      virtual bool execute();
    private :
      /** Wont be direct pointer so the game doesn't need to be running when loading the build-order */
      std::string name;
      std::string place;
      int minimalPopulation;
  };
}
#include "Command.h"

#include <string>

#include <tinyxml.h>

namespace BuildOrder
{
 /** Represnts task to make building in the build order. */
  class CommandBuild : public Command
  {
    public :
      CommandBuild(TiXmlElement* xmlElement);
      virtual bool execute();
    private :
      /**
       * Wont be direct pointer so the game doesn't need to be running when loading the build-order
       * @todo store UnitType instead of name as soon as the nameToType table will be set-up
       */
      std::string name;
      /**
       * Name of the place to build the building, this must be specified by string as the name will point to
       * different BuildingPosition set every game as every map has it different.
       */
      std::string place;
  };
}
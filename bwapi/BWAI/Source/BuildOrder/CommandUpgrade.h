#include "Command.h"

#include <string>

#include <tinyxml.h>
#include <Util/Types.h>

namespace BuildOrder
{
 /** Represnts task to make invent Tech in the build order. */
  class CommandUpgrade : public Command
  {
    public :
      CommandUpgrade(TiXmlElement* xmlElement);
      virtual bool execute();
    private :
      /**
       * Wont be direct pointer so the game doesn't need to be running when loading the build-order
       * @todo store TechType instead of name as soon as the techToType table will be set-up
       */
      std::string name;
      u8 level;
  };
}
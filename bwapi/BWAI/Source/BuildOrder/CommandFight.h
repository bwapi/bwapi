#include "Command.h"

#include <string>

#include <tinyxml.h>
#include <Util/Types.h>

namespace BuildOrder
{
  /** Represnts task to make invent Tech in the build order. */
  class CommandFight : public Command
  {
    public :
      CommandFight(TiXmlElement* xmlElement);
      virtual bool executeInternal(Executor* execute);
    private :
      /**
       * Wont be direct pointer so the game doesn't need to be running when loading the build-order
       */
      std::string dest_type;
      std::string position;
  };
}
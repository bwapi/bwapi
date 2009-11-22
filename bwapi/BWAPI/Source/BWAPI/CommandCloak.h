#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Cloak command. */
  class CommandCloak: public Command
  {
    public :
      /**
       * @param executor Executor of the Cloak command (will be passed as executor
       *        to Command ancestor)
       */
      CommandCloak(UnitImpl* executor);
      virtual void execute();
      int getType();
      std::string describe();
    private :
  };
}
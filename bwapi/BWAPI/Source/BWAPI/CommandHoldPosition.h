#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Hold command. */
  class CommandHoldPosition : public Command
  {
    public :
      /**
       * @param executor Executor of the Hold command (will be passed as executor
                to Command ancestor)
       */
      CommandHoldPosition(UnitImpl* executor);
      virtual void execute();
      int getType();
    private :
  };
};

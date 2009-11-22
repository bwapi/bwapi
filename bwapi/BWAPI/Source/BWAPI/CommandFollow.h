#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Follow command. */
  class CommandFollow : public Command
  {
    public :
      /**
       * @param executor Executor of the Follow command (will be passed as executor
                to Command ancestor)
       * @param target Target of the Follow command.
       */
      CommandFollow(UnitImpl* executor, UnitImpl* target);
      virtual void execute();
      int getType();
      std::string describe();
    private :
      UnitImpl* target;
  };
}
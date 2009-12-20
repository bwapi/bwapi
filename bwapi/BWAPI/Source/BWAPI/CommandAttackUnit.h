#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Attack Unit command. */
  class CommandAttackUnit : public Command
  {
    public :
      /**
       * @param executor Executor of the Attack Unit command (will be passed as executor
                to Command ancestor)
       * @param target Target of the Attack Unit command.
       */
      CommandAttackUnit(UnitImpl* executor, UnitImpl* target);
      virtual void execute();
      int getType();
      std::string describe();
    private :
      UnitImpl* target;
  };
};

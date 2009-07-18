#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Load command. */
  class CommandLoad : public Command
  {
    public :
      /**
       * @param executor Executor of the Load command (will be passed as executor
                to Command ancestor)
       * @param target Target of the AttackUnit command.
       */
      CommandLoad(UnitImpl* executor, UnitImpl* target);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      UnitImpl* target;
  };
}
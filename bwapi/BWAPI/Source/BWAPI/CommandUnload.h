#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Unload command. */
  class CommandUnload : public Command
  {
    public :
      /**
       * @param executor Executor of the Unload command (will be passed as executor
                to Command ancestor)
       * @param target Target of the AttackUnit command.
       */
      CommandUnload(UnitImpl* executor, UnitImpl* target);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      UnitImpl* target;
  };
}
#pragma once
#include "Command.h"
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Cancel Addon command. */
  class CommandCancelAddon : public Command
  {
    public :
      /**
       * @param executor Executor of the Cancel Addon command (will be passed as executor
                to Command ancestor)
       */
      CommandCancelAddon(UnitImpl* building);
      BWAPI::CommandTypes::Enum getType();
      virtual void execute();
      std::string describe();
  };
}
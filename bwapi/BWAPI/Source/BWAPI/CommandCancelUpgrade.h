#pragma once
#include "Command.h"
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Cancel Upgrade command. */
  class CommandCancelUpgrade : public Command
  {
    public :
      /**
       * @param executor Executor of the Cancel Upgrade command (will be passed as executor
                to Command ancestor)
       */
      CommandCancelUpgrade(UnitImpl* building);
      int getType();
      virtual void execute();
      std::string describe();
  };
}
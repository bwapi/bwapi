#pragma once
#include "Command.h"
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Cancel Research command. */
  class CommandCancelResearch : public Command
  {
    public :
      /**
       * @param executor Executor of the Cancel Research command (will be passed as executor
                to Command ancestor)
       */
      CommandCancelResearch(UnitImpl* building);
      BWAPI::CommandTypes::Enum getType();
      virtual void execute();
      std::string describe();
  };
}
#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Repair command. */
  class CommandRepair : public Command
  {
    public :
      /**
       * @param executor Executor of the Repair command (will be passed as executor
                to Command ancestor)
       * @param targetPosition Target of the Repair command.
       */
      CommandRepair(UnitImpl* executor, UnitImpl* target);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      UnitImpl* target;
  };
}
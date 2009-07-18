#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Lift command. */
  class CommandLift : public Command
  {
    public :
      /**
       * @param executor Executor of the Lift command (will be passed as executor
       *        to Command ancestor)
       */
      CommandLift(UnitImpl* executor);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
  };
}
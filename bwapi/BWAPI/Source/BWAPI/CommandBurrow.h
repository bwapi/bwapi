#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Burrow command. */
  class CommandBurrow : public Command
  {
    public :
      /**
       * @param executor Executor of the Burrow command (will be passed as executor
       *        to Command ancestor)
       */
      CommandBurrow(UnitImpl* executor);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
  };
}
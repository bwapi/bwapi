#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of ReturnCargo command. */
  class CommandReturnCargo : public Command
  {
    public :
      /**
       * @param executor Executor of the ReturnCargo command (will be passed as executor
       *        to Command ancestor)
       */
      CommandReturnCargo(UnitImpl* executor);
      virtual void execute();
      int getType();
    private :
  };
};

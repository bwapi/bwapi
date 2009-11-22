#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Unburrow command. */
  class CommandUnburrow : public Command
  {
    public :
      /**
       * @param executor Executor of the Unburrow command (will be passed as executor
       *        to Command ancestor)
       */
      CommandUnburrow(UnitImpl* executor);
      virtual void execute();
      int getType();
      std::string describe();
    private :
  };
}
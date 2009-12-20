#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Stop command. */
  class CommandStop : public Command
  {
    public :
      /**
       * @param executor Executor of the Stop command (will be passed as executor
       *        to Command ancestor)
       */
      CommandStop(UnitImpl* executor);
      virtual void execute();
      int getType();
      std::string describe();
    private :
  };
};

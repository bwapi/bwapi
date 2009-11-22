#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Set Rally command. */
  class CommandSetRally : public Command
  {
    public :
      /**
       * @param executor Executor of the Set Rally command (will be passed as executor
                to Command ancestor)
       * @param target Target of the Set Rally command.
       */
      CommandSetRally(UnitImpl* executor, BW::Position targetPosition);
      CommandSetRally(UnitImpl* executor, UnitImpl* targetUnit);
      virtual void execute();
      int getType();
      std::string describe();
    private :
      BW::Position targetPosition;
      UnitImpl* targetUnit;
  };
}
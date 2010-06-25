#pragma once
#include "Command.h"
#include <BWAPI/TechType.h>
#include <BWAPI/Position.h>
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Use Tech command. */
  class CommandUseTech : public Command
  {
    public :
      /**
       * @param executor Executor of the Stop command (will be passed as executor
       *        to Command ancestor)
       */
      CommandUseTech(UnitImpl* executor, BWAPI::TechType);
      CommandUseTech(UnitImpl* executor, BWAPI::TechType, BWAPI::Position targetPosition);
      CommandUseTech(UnitImpl* executor, BWAPI::TechType, UnitImpl* targetUnit);
      virtual void execute();
      int getType();
    private :
      BWAPI::TechType tech;
      BWAPI::Position targetPosition;
      UnitImpl* targetUnit;
  };
};

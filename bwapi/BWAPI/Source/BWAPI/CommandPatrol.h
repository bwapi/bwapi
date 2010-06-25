#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Patrol command. */
  class CommandPatrol : public Command
  {
    public :
      /**
       * @param executor Executor of the Patrol command (will be passed as executor
                to Command ancestor)
       * @param targetPosition Target of the Patrol command.
       */
      CommandPatrol(UnitImpl* executor, const BW::Position& targetPosition);
      virtual void execute();
      int getType();
    private :
      BW::Position targetPosition;
  };
};

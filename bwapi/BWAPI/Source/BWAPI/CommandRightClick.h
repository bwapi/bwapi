#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of right click command. */
  class CommandRightClick : public Command
  {
    public :
      /**
       * @param executor Executor of the right click (will be passed as executor
                to Command ancestor)
       * @param targetPosition Target of the right click.
       */
      CommandRightClick(UnitImpl* executor, const BW::Position& targetPosition);
      /**
       * @param executor Executor of the right click (will be passed as executor
                to Command ancestor)
       * @param targetUnit target of the right click.
       */
      CommandRightClick(UnitImpl* executor, UnitImpl* targetUnit);
      virtual void execute();
      int getType();
      std::string describe();
    private :
      UnitImpl* targetUnit;
      BW::Position targetPosition;
      bool isPosition;
  };
};

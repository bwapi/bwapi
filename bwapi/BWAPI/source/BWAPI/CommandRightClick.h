#pragma once
#include "Command.h"
#include "../BW/PositionUnitTarget.h"
namespace BWAPI { class Unit; }
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
      CommandRightClick(Unit* executor, const BW::Position& targetPosition);
      /** 
       * @param executor Executor of the right click (will be passed as executor 
                to Command ancestor)
       * @param targetUnit target of the right click.
       */
      CommandRightClick(Unit* executor, BWAPI::Unit* targetUnit);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
    private :
      BWAPI::Unit *targetUnit;
      BW::Position targetPosition;
  };
}
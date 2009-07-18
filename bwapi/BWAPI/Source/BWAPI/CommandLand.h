#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
#include <BW/TilePosition.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Land command. */
  class CommandLand : public Command
  {
    public :
      /**
       * @param executor Executor of the Attack Location command (will be passed as executor
                to Command ancestor)
       * @param targetPosition Target of the Land command.
       */
      CommandLand(UnitImpl* executor, const BW::TilePosition& targetPosition);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      BW::TilePosition targetPosition;
  };
}
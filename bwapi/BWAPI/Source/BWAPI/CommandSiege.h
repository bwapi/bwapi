#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Siege command. */
  class CommandSiege : public Command
  {
    public :
      /**
       * @param executor Executor of the Siege command (will be passed as executor
       *        to Command ancestor)
       */
      CommandSiege(UnitImpl* executor);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
  };
}
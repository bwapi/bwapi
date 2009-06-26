#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Unsiege command. */
  class CommandUnsiege : public Command
  {
    public :
      /** 
       * @param executor Executor of the Unsiege command (will be passed as executor 
       *        to Command ancestor)
       */
      CommandUnsiege(UnitImpl* executor);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
  };
}
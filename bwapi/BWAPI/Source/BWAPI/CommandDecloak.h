#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Decloak command. */
  class CommandDecloak: public Command
  {
    public :
      /**
       * @param executor Executor of the Decloak command (will be passed as executor
       *        to Command ancestor)
       */
      CommandDecloak(UnitImpl* executor);
      virtual void execute();
      int getType();
    private :
  };
};

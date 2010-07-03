#pragma once
#include "Command.h"
#include <BWAPI/UnitType.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Cancel Morph command. */
  class CommandCancelMorph : public Command
  {
    public :
      /**
       * @param executor Executor of the Cancel Morph command (will be passed as executor
                to Command ancestor)
       */
      CommandCancelMorph(UnitImpl* building);
      int getType();
      virtual void execute();
      int startFrame;
      UnitType morphType;
  };
};

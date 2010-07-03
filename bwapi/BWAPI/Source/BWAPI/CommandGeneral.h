#pragma once
#include "Command.h"
#include <BWAPI/UnitCommand.h>
namespace BWAPI
{
  class CommandGeneral : public Command
  {
    public :
      CommandGeneral(UnitCommand command);
      virtual void execute();
      int getType();
    private :
      UnitCommand command;
      int startFrame;
      int savedExtra;
  };
};

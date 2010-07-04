#pragma once
#include "Command.h"
#include <BWAPI/UnitCommand.h>
namespace BWAPI
{
  class Command
  {
    public :
      Command(UnitCommand command);
      void execute();
    private :
      UnitCommand command;
      int startFrame;
      int savedExtra;
  };
};

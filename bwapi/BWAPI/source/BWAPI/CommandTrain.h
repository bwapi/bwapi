#pragma once
#include "Command.h"
#include "../BW/UnitTypes.h"
namespace BWAPI { class UnitPrototype; }
namespace BWAPI
{
  class CommandTrain : public Command
  {
    public :
      CommandTrain(Unit* building, UnitPrototype* toTrain);
      ~CommandTrain();
      virtual void execute(); // TODO: return value
      BWAPI::CommandTypes::Enum getType();
    private :
      UnitPrototype* toTrain;
  };
}
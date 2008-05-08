#pragma once
#include "Command.h"
#include "../BW/UnitTypes.h"
namespace BWAPI { class UnitPrototype; }
namespace BWAPI
{
  /**
   * BWAPI internal representation of train command.
   */
  class CommandTrain : public Command
  {
    public :
      /** 
       * @param building Building that constructs the specified unit 
       *         (will be passed as executor to Command ancestor
       * @param toTrain Unit that should be constructed
       */
      CommandTrain(Unit* building, UnitPrototype* toTrain);
      ~CommandTrain();
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
    private :
      UnitPrototype* toTrain;
  };
}
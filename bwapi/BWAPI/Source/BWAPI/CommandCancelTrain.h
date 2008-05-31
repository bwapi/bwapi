#pragma once
#include "Command.h"
namespace BWAPI { class Unit; }
namespace BWAPI
{
  /** BWAPI internal representation of cancel command. */
  class CommandCancelTrain : public Command
  {
    public :
      /** 
       * @param building Building that constructs the specified unit 
       *         (will be passed as executor to Command ancestor
       */
      CommandCancelTrain(Unit* building);
      ~CommandCancelTrain();
      BWAPI::CommandTypes::Enum getType();
      /** 
        * @todo Give back money
        * @todo Give back supply
        * @todo Take supply if another unit in slot
        */
      virtual void execute();
      std::string describe();
  };
}
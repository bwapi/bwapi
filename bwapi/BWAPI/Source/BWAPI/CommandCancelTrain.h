#pragma once
#include "Command.h"
namespace BWAPI { class UnitImpl; }
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
      CommandCancelTrain(UnitImpl* building);
      CommandCancelTrain(UnitImpl* building, int slot);
      ~CommandCancelTrain();
      int getType();
      /**
        * @todo Give back money
        * @todo Give back supply
        * @todo Take supply if another unit in slot
        */
      virtual void execute();
    private:
      int slot;
  };
};

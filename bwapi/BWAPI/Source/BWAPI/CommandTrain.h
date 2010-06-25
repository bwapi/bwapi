#pragma once

namespace BWAPI { class UnitPrototype; }

#include <BW/UnitType.h>

#include "Command.h"

namespace BWAPI { class UnitImpl; }
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
      CommandTrain(UnitImpl* building, BW::UnitType toTrain);
      ~CommandTrain();
      virtual void execute();
      int getType();
    private :
      BW::UnitType toTrain;
  };
};

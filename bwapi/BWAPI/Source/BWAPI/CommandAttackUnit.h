#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Attack Location command. */
  class CommandAttackUnit : public Command
  {
    public :
      /** 
       * @param executor Executor of the attack location (will be passed as executor 
                to Command ancestor)
       * @param targetPosition Target of the attack location.
       */
      CommandAttackUnit(UnitImpl* executor, UnitImpl* target);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      UnitImpl* target;
  };
}
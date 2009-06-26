#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of UnloadAll command. */
  class CommandUnloadAll : public Command
  {
    public :
      /** 
       * @param executor Executor of the UnloadAll command (will be passed as executor 
       *        to Command ancestor)
       */
      CommandUnloadAll(UnitImpl* executor);
      CommandUnloadAll(UnitImpl* executor, BW::Position targetPosition);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      BW::Position targetPosition;
      bool usePosition;
  };
}
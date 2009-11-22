#pragma once

#include "Command.h"
#include <BW/UnitType.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Morph Unit command. */
  class CommandMorphUnit : public Command
  {
    public :
      /**
       * @param executor Executor of the Morph Unit command (will be passed as executor
       *       to Command ancestor)
       * @param toMorph The Unit to be morphed into
       */
      CommandMorphUnit(UnitImpl* executor, BW::UnitType toMorph);
      virtual void execute();
      int getType();
      std::string describe();
    private :
      BW::UnitType toMorph;
  };
}
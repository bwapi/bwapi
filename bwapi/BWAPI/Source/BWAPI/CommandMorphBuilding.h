#pragma once

#include "Command.h"
#include <BW/UnitType.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Morph Building command. */
  class CommandMorphBuilding : public Command
  {
    public :
      /**
       * @param executor Executor of the Morph Building command (will be passed as executor
       *       to Command ancestor)
       * @param toMorph The Building to be morphed into
       */
      CommandMorphBuilding(UnitImpl* executor, BW::UnitType toMorph);
      virtual void execute();
      int getType();
    private :
      BW::UnitType toMorph;
  };
};

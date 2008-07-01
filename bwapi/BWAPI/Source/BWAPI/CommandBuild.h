#pragma once

#include "Command.h"

#include <BW/UnitType.h>
#include <BW/Position.h>

namespace BWAPI
{
  /** BWAPI internal representation of make building command. */
  class CommandBuild : public Command
  {
    public :
      /** 
       * @param builder Worker that builds the building
       *         (will be passed as executor to Command ancestor)
       * @param toBuild Building that should be constructed
       * @param Position of the building (in tiles)
       */
      CommandBuild(Unit* builder, BW::UnitType toBuild, BW::TilePosition position);
      ~CommandBuild();
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      BW::UnitType toBuild;
      BW::Position position;
  };
}
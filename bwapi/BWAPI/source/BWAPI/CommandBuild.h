#pragma once
#include "Command.h"
#include "../BW/UnitType.h"
#include "../BW/Position.h"
namespace BWAPI { class UnitPrototype; }
namespace BWAPI
{
  /**
   * BWAPI internal representation of make building command.
   */
  class CommandBuild : public Command
  {
    public :
      /** 
       * @param builder Worker that builds the building
       *         (will be passed as executor to Command ancestor)
       * @param toBuild Building that should be constructed
       */
      CommandBuild(Unit* builder, BW::UnitType toBuild, BW::Position position);
      ~CommandBuild();
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      BW::UnitType toBuild;
      BW::Position position;
  };
}
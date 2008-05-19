#pragma once
#include "Command.h"
#include "../BW/UnitTypes.h"
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
      CommandBuild(Unit* builder, UnitPrototype* toBuild);
      ~CommandBuild();
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      UnitPrototype* toBuild;
  };
}
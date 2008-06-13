#pragma once

#include "Command.h"

#include <BW/TechType.h>

namespace BWAPI
{
  /** BWAPI internal representation of make building command. */
  class CommandInvent : public Command
  {
    public :
      /** 
       * @param builder Worker that builds the building
       *         (will be passed as executor to Command ancestor)
       * @param toBuild Building that should be constructed
       * @param Position of the building (in tiles)
       */
      CommandInvent(Unit* building, BW::TechType tech);
      ~CommandInvent();
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      BW::TechType tech;
  };
}
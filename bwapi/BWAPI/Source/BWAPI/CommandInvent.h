#pragma once

#include "Command.h"

#include <BW/TechType.h>

namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of invent tech command. */
  class CommandInvent : public Command
  {
    public :
      /** 
       * @param building Building that invents the tech
       * @param tech Tech to be invented
       */
      CommandInvent(UnitImpl* building, BW::TechType tech);
      ~CommandInvent();
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      BW::TechType tech;
  };
}
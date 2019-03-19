#pragma once
#include "CommandTemp.h"

namespace BWAPI
{
  class Command : public CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>
  {
    public:
      Command(const UnitCommand& command) : CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>(command) {}
  };
}
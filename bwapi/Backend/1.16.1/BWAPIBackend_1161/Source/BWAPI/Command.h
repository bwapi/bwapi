#pragma once
#include "CommandTemp.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"

namespace BWAPI
{
  class Command : public CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>
  {
    public:
      Command(const UnitCommand& command) : CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>(command) {}
  };
}
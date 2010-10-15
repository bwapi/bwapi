#pragma once
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include "CommandTemp.h"

namespace BWAPI
{
  class Command : public CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>
  {
    public:
      Command(UnitCommand command) : CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>(command) {}
  };
}
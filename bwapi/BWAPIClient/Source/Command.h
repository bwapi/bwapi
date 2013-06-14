#pragma once
#include <BWAPI/Client/UnitImpl.h>
#include <BWAPI/Client/PlayerImpl.h>

#include "../../BWAPI/Source/BWAPI/CommandTemp.h"

namespace BWAPI
{
  class Command : public CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>
  {
    public:
      Command(UnitCommand command) : CommandTemp<BWAPI::UnitImpl,BWAPI::PlayerImpl>(command) {}
  };
}
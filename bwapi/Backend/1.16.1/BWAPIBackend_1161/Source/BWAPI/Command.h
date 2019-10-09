#pragma once
#include <BWAPI4/Client/CommandTemp.h>
#include "UnitImpl.h"
#include "PlayerImpl.h"

namespace BWAPI4
{
  using Command = CommandTemp<UnitImpl, PlayerImpl>;
}
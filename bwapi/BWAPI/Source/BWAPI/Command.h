#pragma once
#include "CommandTemp.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"

namespace BWAPI
{
  using Command = CommandTemp<UnitImpl, PlayerImpl>;
}
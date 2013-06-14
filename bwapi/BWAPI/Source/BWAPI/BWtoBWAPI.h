#pragma once

#include <BWAPI/Order.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  extern int BWtoBWAPI_Order[Orders::Enum::MAX];
  extern int AttackAnimationRestFrame[UnitTypes::Enum::MAX];
  void BWtoBWAPI_init();
}
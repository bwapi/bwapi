#pragma once

#include <BWAPI4/Order.h>
#include <BWAPI4/UnitType.h>

namespace BWAPI4
{
  extern int BWtoBWAPI_Order[Orders::Enum::MAX];
  extern int AttackAnimationRestFrame[UnitTypes::Enum::MAX];
  void BWtoBWAPI_init();
}
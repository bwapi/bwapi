#pragma once

#include <BWAPI/Order.h>
#include <BWAPI/UnitType.h>

namespace BWAPI4
{
  extern int BWtoBWAPI_Order[BWAPI::Orders::Enum::MAX];
  extern int AttackAnimationRestFrame[BWAPI::UnitTypes::Enum::MAX];
  void BWtoBWAPI_init();
}
#pragma once

#include <BW/OrderID.h>
#include <BW/UnitID.h>
namespace BWAPI
{
  extern int BWtoBWAPI_Order[BW::OrderID::MAX];
  extern int AttackAnimationRestFrame[BW::UnitID::MAX];
  void BWtoBWAPI_init();
}
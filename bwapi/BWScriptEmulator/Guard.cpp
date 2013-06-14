#include "UnitInfo.h"

void UnitWrap::RunGuard()
{
  SetOrderTimer( rand()%16 ); // 0-15

  if ( GetControlType() == ControlTypes::Guard )
    SetVirtualUnitOrder(Orders::Enum::GuardPost);
  else
    SetVirtualUnitOrder(Orders::Enum::ComputerAI);
}

void UnitWrap::RunGuardPost()
{
  if ( GetOrderTimer() != 0 )
    return;
  SetOrderTimer(15);

  if ( TaskSpellcaster(false) )
    return;

  if ( TaskAggression() )
    return;

  if ( pUnit->getType() == UnitTypes::Protoss_Observer )
    return;

  if ( GetControlType() == ControlTypes::Guard && pUnit->getPosition() != GetGuardReturnPosition() )
  {
    if ( pUnit->getType() == UnitTypes::Terran_Siege_Tank_Siege_Mode )
      pUnit->unsiege();
    SetVirtualUnitOrder(Orders::Enum::ComputerReturn, GetGuardReturnPosition() );
  }
  else
  {
    SetVirtualUnitOrder(Orders::Enum::ComputerAI);
  }
}

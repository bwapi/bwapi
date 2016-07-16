#include "UnitInfo.h"


void UnitWrap::RunComputerReturn()
{
  if ( TaskAggression() )
    return;

  if ( pUnit->getType() == UnitTypes::Terran_Medic /*&& MedicOrders()*/ )
    return;

  // order state is 0
  if ( !GetOrderState() )
  {
    pUnit->move( GetOrderTargetPosition() );  // temporary
    
    //if ( MoveToTarget() and not getting AI Transport Assist )
      SetOrderState(1);
  }

  if ( GetOrderState() )
  {
    if ( GetControlType() == ControlTypes::Guard )
    {
      if ( GetOrderTargetPosition() == pUnit->getPosition() ) // Reached destination
      {
        SetGuardReturnPosition(pUnit->getPosition());
        SetVirtualUnitOrder(Orders::Enum::Guard);
      }
    }
    else
    {
      SetVirtualUnitOrder(Orders::Enum::ComputerAI);
    }
  }

}

#include "OrderEmulate.h"
#include "UnitInfo.h"

// random number within [min,max]
int randBetween(int min, int max)
{
  return (rand()%((max-min)+1)) + min;
}

void UnitWrap::RunJunkYardDog()
{
  if ( GetOrderState() == 0 )
  {
    // Retrieve a random target position
    BWAPI::Position targ = directionPositions[ randBetween(0,255) ];
    BWAPI::Position p1, p2, p3, p4;

    // Modify the target position to be a random larger vector
    int distance = randBetween(1024,2048);
    targ *= distance;
    targ /= 256;
    
    // Add the unit's position and make the position valid
    targ += pUnit->getPosition();
    targ.makeValid();

    // Issue move order and set the order state to 1
    pUnit->move(targ);
    SetOrderState(1);
  }
  else
  {
    BWAPI::Unit pAutoTarget = GetAttackTarget();
    if ( pAutoTarget == nullptr ||
      !Broodwar->self()->isEnemy(pAutoTarget->getPlayer())  ||
      !pAutoTarget->isVisible() ||
      pAutoTarget->isInvincible() ||
      !pAutoTarget->isDetected() )
    {
      if ( GetOrderTimer() == 0 )
      {
        SetOrderTimer(15);
        if ( ( pUnit->getTargetPosition() != pUnit->getPosition() /* || !unmovable */ ) /*&&
               (pUnit->getSpellCooldown() != 0 || pUnit->isFlying() )*/  )
        {
          pAutoTarget = FindNewAttackTarget(); // Look for new closest target!
          if ( pAutoTarget != nullptr )
          {
            if ( pUnit->attack(pAutoTarget) )
              SetVirtualUnitOrder(Orders::Enum::Guard);
            // queue Junk Yard Dog
            //pUnit->holdPosition();
          }
        }
        else
        {
          SetOrderState(0);
        }
      } // if order timer^
    }
    else
    {
      if ( pUnit->attack(pAutoTarget) )
        SetVirtualUnitOrder(Orders::Enum::Guard);
      // queue Junk Yard Dog
      SetAttackTarget(nullptr);
      //pUnit->holdPosition();
    }
  }

}

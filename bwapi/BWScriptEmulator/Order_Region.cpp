#include "Order_Region.h"
#include "Controller.h"
#include "UnitInfo.h"

using namespace AISCRIPT;

Order_Region make_patrol_impl(Enum::MAKE_PATROL);
Order_Region junkyard_dog_impl(Enum::JUNKYARD_DOG);

bool Order_Region::execute(aithread &thread) const
{
  // Get units in region
  Unitset rgnUnits( Broodwar->getUnitsInRectangle(thread.getLocation().topLeft, thread.getLocation().bottomRight, 
                                                    GetPlayer == Broodwar->self() && IsCompleted) );
  
  // Execute action
  if ( this->getOpcode() == Enum::MAKE_PATROL )
  {
    rgnUnits.patrol( MainController.genCmdTarget.center() );
  }
  else if ( this->getOpcode() == Enum::JUNKYARD_DOG )
  {
    for each ( Unit u in rgnUnits )
      UnitWrap(u).SetVirtualUnitOrder(Orders::Enum::JunkYardDog);
  }

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}

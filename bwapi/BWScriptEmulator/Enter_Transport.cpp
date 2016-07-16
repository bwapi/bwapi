#include "Enter_Transport.h"
#include "Controller.h"

using namespace AISCRIPT;

Enter_Transport enter_bunker_impl(AISCRIPT::Enum::ENTER_BUNKER);
Enter_Transport enter_transport_impl(AISCRIPT::Enum::ENTER_TRANSPORT);
Enter_Transport exit_transport_impl(AISCRIPT::Enum::EXIT_TRANSPORT);

auto baseProc   = [](Unit u)->bool{ return u->getPlayer() == Broodwar->self() && u->isCompleted(); };
bool bunkerProc(Unit u)
{
  return (GetRace == Races::Terran && IsOrganic && baseProc)(u);
}
bool transProc(Unit u)
{
  return (SpaceRequired < 8 && baseProc)(u);
}

bool Enter_Transport::execute(aithread &thread) const
{
  if ( this->getOpcode() == AISCRIPT::Enum::ENTER_BUNKER || this->getOpcode() == AISCRIPT::Enum::ENTER_TRANSPORT )
  {
    Unitset unitsForTrans( Broodwar->getUnitsInRectangle(thread.getLocation().topLeft, thread.getLocation().bottomRight, 
                            this->getOpcode() == AISCRIPT::Enum::ENTER_BUNKER ? bunkerProc : transProc) );
  
    // Iterate the units that are to enter the bunkers
    for ( auto u : unitsForTrans )
    {
      // Find a bunker closest to the current unit that has space available
      Unit pClosest = u->getClosestUnit( (this->getOpcode() == AISCRIPT::Enum::ENTER_BUNKER ? GetType == UnitTypes::Terran_Bunker : GetType != UnitTypes::Terran_Bunker) &&
                                            IsTransport &&
                                            GetPlayer == Broodwar->self() && 
                                            SpaceRemaining >= u->getType().spaceRequired() );

      if ( pClosest )  // If a bunker was found
        u->rightClick(pClosest);
    }
  }
  else if ( this->getOpcode() == AISCRIPT::Enum::EXIT_TRANSPORT )
  {
    // Normally we are supposed to check the type here, but we don't need to save processing time and can let BWAPI sort it out
    Broodwar->getUnitsInRectangle(thread.getLocation().topLeft, thread.getLocation().bottomRight).unloadAll();
  }

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}

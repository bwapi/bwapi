#include "Groundmap_Jump.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Groundmap_Jump);

bool Groundmap_Jump::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wOffset;
  thread.readTuple( std::tie(wOffset) );

  // Get all enemy structures
  Unitset enemyUnits( Broodwar->enemies().getUnits() );
  enemyUnits.erase_if( !(Exists && IsBuilding && !IsLifted) );

  // Check if there is a path to at least one enemy structure, then jump
  for ( auto u : enemyUnits )
  {
    if ( u->hasPath(thread.getLocation().center()) )
    {
      thread.setScriptOffset( wOffset );
      break;
    }
  }

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "p_%04X", wOffset);
  return true;
}

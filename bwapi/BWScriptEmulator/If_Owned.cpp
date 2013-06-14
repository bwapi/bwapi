#include "If_Owned.h"
#include "Controller.h"

using namespace AISCRIPT;

If_Owned if_owned_impl(AISCRIPT::Enum::IF_OWNED);
If_Owned enemyowns_jump_impl(AISCRIPT::Enum::ENEMYOWNS_JUMP);

bool If_Owned::execute(aithread &thread) const
{
  // Parameters
  WORD wUnitType, wJump;
  thread.readTuple( std::tie(wUnitType, wJump) );

  // Get Player's units depending on opcode
  Unitset units( this->getOpcode() == Enum::IF_OWNED ? Broodwar->self()->getUnits() : Broodwar->enemies().getUnits() );

  // Check if the player owns at least one of the given unit type
  if ( std::find_if(units.begin(), units.end(), GetType == wUnitType) != units.end() )
    thread.setScriptOffset( wJump );

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%s p_%04X", AISCRIPT::getUnitName(wUnitType), wJump );
  return true;
}

#include "Race_Jump.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Race_Jump);

bool Race_Jump::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD t_jmp, z_jmp, p_jmp;
  thread.readTuple( std::tie(t_jmp, z_jmp, p_jmp) );

  // should be AI_getClosestEnemyRace; gets closest enemy unit to region
  // @TODO: BWAPI: Filters: IsEnemy, IsAlly, IsNeutral?

  // Get enemy race
  Race race = Races::None;
  if ( Broodwar->enemy() )
    race = Broodwar->enemy()->getRace();

  /// Set the new script offset
  switch ( race )
  {
  case Races::Enum::Protoss:
    thread.setScriptOffset( p_jmp );
    break;
  case Races::Enum::Zerg:
    thread.setScriptOffset( z_jmp );
    break;
  default:
    thread.setScriptOffset( t_jmp );
    break;
  }

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "p_%04X p_%04X p_%04X", t_jmp, z_jmp, p_jmp);
  return true;
}

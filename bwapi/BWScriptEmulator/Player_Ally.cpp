#include "Player_Ally.h"
#include "Controller.h"

using namespace AISCRIPT;

Player_Ally player_ally_impl(AISCRIPT::Enum::PLAYER_ALLY);
Player_Ally player_enemy_impl(AISCRIPT::Enum::PLAYER_ENEMY);

bool Player_Ally::execute(aithread &thread) const
{
  // @TODO: BWAPI: Unitset::getPlayers for retrieving set of players owning the units, not important
  /* can become
  unitsInRect( bw->getUnitsInRectangle(location.topLeft, location.bottomRight, GetPlayer != self) ).getPlayers().setAlliance(bOpcode == AISCRIPT::Enum::PLAYER_ALLY);
                                        */
  // Set the alliance of all players who own units inside the thread's execution location
  Unitset units( Broodwar->getUnitsInRectangle(thread.getLocation().topLeft, thread.getLocation().bottomRight, GetPlayer != Broodwar->self()) );
  for ( auto u : units)
    Broodwar->setAlliance(u->getPlayer(), this->getOpcode() == AISCRIPT::Enum::PLAYER_ALLY);

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}

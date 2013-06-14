#include "Resources_Jump.h"
#include "Controller.h"

using namespace AISCRIPT;

Resources_Jump resources_jump_impl(AISCRIPT::Enum::RESOURCES_JUMP);
Resources_Jump enemyresources_jump_impl(AISCRIPT::Enum::ENEMYRESOURCES_JUMP);

bool Resources_Jump::execute(aithread &thread) const
{
  // Parameters
  WORD wOre, wGas, wJump;
  thread.readTuple( std::tie(wOre, wGas, wJump) );

  // Get Player
  Player player = this->getOpcode() == Enum::RESOURCES_JUMP ? Broodwar->self() : Broodwar->enemy();

  // Check if player has the given amount of resources
  if ( player && player->minerals() >= wOre && player->gas() >= wGas )
    thread.setScriptOffset( wJump );

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %u p_%04X", wOre, wGas, wJump );
  return true;
}

#pragma once

#include "Game.h"
class Util::Dictionary;

/** 
 * Is loaded in Game::Game (init of program), but I made it global to simplify the usage of that, in fact
 * it is just pointer to BWAPI::Broodwar.configuration
 */
extern Util::Dictionary* config;

namespace BWAPI
{
  /**
   * Broodwar is, and always should be the ONLY instance of the Game class, it is singleton.
   * As there is only one instance, the class is defined globaly and as value (not pointer), so the instance 
   * is constructed automatically and there is also no need to care about the destructor.
   * We wanted to save passing the Game parameter everywhere, so we expect everywhere in the code that this
   * variable is instantialised.
   */
  extern Game Broodwar;
};

#pragma once
namespace Util { class Dictionary; }

/**
 * Is loaded in Game::Game (init of program), but I made it global to simplify the usage of that, in fact
 * it is just pointer to BWAPI::Broodwar.configuration
 */
extern Util::Dictionary* config;

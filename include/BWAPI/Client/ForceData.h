#pragma once

#include <string>

#include <BWAPI/IDs.h>

namespace BWAPI
{
  class Game;

  struct ForceData
  {
    ForceData(Game &g, ForceID fid) : id{ fid }, game{ g } { }
    ForceID id;
    Game &game;
    PlayerID::list players;
    std::string name;
  };
}

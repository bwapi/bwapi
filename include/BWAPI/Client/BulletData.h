#pragma once
#include <BWAPI/BulletType.h>
#include <BWAPI/IDs.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class Game;

  struct BulletData
  {
    BulletData(Game &g, BulletID b) : game{ g }, id{ b } {
    }

    BWAPI::Game &game;
    BWAPI::BulletID id{ BulletID::None };
    BWAPI::PlayerID player{ PlayerID::None };
    BWAPI::BulletType type{ BulletTypes::None };
    BWAPI::UnitID source{ UnitID::None };
    BWAPI::Position position{ Positions::None };
    double angle = 0;
    double velocityX = 0;
    double velocityY = 0;
    BWAPI::UnitID target{ UnitID::None };
    BWAPI::Position targetPosition{ Positions::None };
    int removeTimer = 0;
    bool exists = false;
    bool isVisible[9] = {};
  };
}

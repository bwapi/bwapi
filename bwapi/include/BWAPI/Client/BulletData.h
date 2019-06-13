#pragma once
#include "BWAPI/BulletType.h"

namespace BWAPI
{
  class Game;

  struct BulletData
  {
    BulletData(Game &g, BulletID b) : game{ g }, id{ b } {
    }

    BWAPI::Game &game;
    BWAPI::BulletID id{ -1 };
    BWAPI::PlayerID player{ -1 };
    BWAPI::BulletType type;
    BWAPI::UnitID source{ -1 };
    BWAPI::Position position;
    double angle;
    double velocityX;
    double velocityY;
    BWAPI::UnitID target{ -1 };
    BWAPI::Position targetPosition;
    int removeTimer;
    bool exists;
    bool isVisible[9];
  };
}

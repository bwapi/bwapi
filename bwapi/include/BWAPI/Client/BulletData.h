#pragma once
#include "BWAPI/BulletType.h"

namespace BWAPI
{
  struct BulletData
  {
    BWAPI::BulletID id;
    BWAPI::PlayerID player;
    BWAPI::BulletType type;
    BWAPI::UnitID source;
    BWAPI::Position position;
    double angle;
    double velocityX;
    double velocityY;
    BWAPI::UnitID target;
    BWAPI::Position targetPosition;
    int removeTimer;
    bool exists;
    bool isVisible[9];
  };
}

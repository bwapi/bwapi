#pragma once

namespace BWAPI
{
  struct BulletData
  {
    int id;
    int player;
    int type;
    int source;
    int positionX;
    int positionY;
    double angle;
    double velocityX;
    double velocityY;
    int target;
    int targetPositionX;
    int targetPositionY;
    int removeTimer;
    bool exists;
    bool isVisible[9];
  };
}

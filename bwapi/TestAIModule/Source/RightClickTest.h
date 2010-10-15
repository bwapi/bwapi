#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class RightClickTest : public TestCase
{
  public:
    RightClickTest(BWAPI::UnitType unitType, BWAPI::UnitType enemyType, BWAPI::UnitType friendlyType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    int startFrame;
    int nextFrame;
    BWAPI::Unit* unit;
    BWAPI::Unit* enemyUnit;
    BWAPI::Unit* friendUnit;
    BWAPI::UnitType unitType;
    BWAPI::UnitType enemyType;
    BWAPI::UnitType friendlyType;
    BWAPI::Position targetPosition;
};
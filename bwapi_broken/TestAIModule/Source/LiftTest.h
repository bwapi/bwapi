#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class LiftTest : public TestCase
{
  public:
    LiftTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    int startFrame;
    int nextFrame;
    BWAPI::Unit* unit;
    BWAPI::UnitType unitType;
    bool completedLift;
    BWAPI::TilePosition originalTilePosition;
};
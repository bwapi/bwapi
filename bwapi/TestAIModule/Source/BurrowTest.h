#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class BurrowTest : public TestCase
{
  public:
    BurrowTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    int startFrame;
    int nextFrame;
    BWAPI::Unit* unit;
    BWAPI::UnitType unitType;
    bool completedBurrow;
};
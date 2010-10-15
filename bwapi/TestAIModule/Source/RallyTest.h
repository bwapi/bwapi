#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class RallyTest : public TestCase
{
  public:
    RallyTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    int startFrame;
    int nextFrame;
    BWAPI::Unit* unit;
    BWAPI::Unit* rallyUnit;
    BWAPI::Position rallyPosition;
    BWAPI::UnitType unitType;
};
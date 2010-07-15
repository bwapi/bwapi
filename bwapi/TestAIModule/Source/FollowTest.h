#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class FollowTest : public TestCase
{
  public:
    FollowTest(BWAPI::UnitType unitType, BWAPI::UnitType targetType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning();
  private:
    bool running;
    bool fail;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* unit;
    BWAPI::UnitType unitType;
    BWAPI::Unit* target;
    BWAPI::UnitType targetType;
    bool started;
};
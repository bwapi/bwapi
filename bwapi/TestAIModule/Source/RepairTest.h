#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class RepairTest : public TestCase
{
  public:
    RepairTest(BWAPI::UnitType unitType, BWAPI::UnitType targetType);
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
    bool stopped;
};
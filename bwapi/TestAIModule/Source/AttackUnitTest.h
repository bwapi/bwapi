#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class AttackUnitTest : public TestCase
{
  public:
    AttackUnitTest(BWAPI::UnitType unitType, BWAPI::UnitType targetType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning();
  private:
    bool running;
    bool fail;
    int startFrame;
    int nextFrame;
    int startingAttackFrame;
    int reachedDamagePointFrame;
    BWAPI::Unit* unit;
    BWAPI::UnitType unitType;
    BWAPI::Unit* target;
    BWAPI::UnitType targetType;
    int startingHPS;
    bool stopped;
    int initialScarabCount;
};
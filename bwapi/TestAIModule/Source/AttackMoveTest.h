#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class AttackMoveTest : public TestCase
{
  public:
    AttackMoveTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    int startFrame;
    int nextFrame;
    int doneFrame;
    BWAPI::Unit* unit;
    BWAPI::UnitType unitType;
    BWAPI::Position targetPosition;
};
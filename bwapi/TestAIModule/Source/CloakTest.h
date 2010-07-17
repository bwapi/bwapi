#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class CloakTest : public TestCase
{
  public:
    CloakTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    int startFrame;
    int nextFrame;
    BWAPI::Unit* unit;
    BWAPI::UnitType unitType;
    bool completedCloak;
    int currentEnergy;
};
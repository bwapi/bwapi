#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class CancelUpgradeTest : public TestCase
{
  public:
    CancelUpgradeTest(BWAPI::UpgradeType upgradeType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    BWAPI::UpgradeType upgradeType;
    BWAPI::UnitType upgraderType;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* upgrader;
    int correctMineralCount;
    int correctGasCount;
    int correctSupplyUsedCount;
};
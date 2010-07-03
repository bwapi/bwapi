#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class UpgradeTest : public TestCase
{
  public:
    UpgradeTest(BWAPI::UpgradeType upgradeType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning(); 
  private:
    bool running;
    bool fail;
    BWAPI::UpgradeType upgradeType;
    BWAPI::UnitType upgraderType;
    int previousUpgradeLevel;
    int startUpgradeFrame;
    int nextUpdateFrame;
    BWAPI::Unit* upgrader;
    int correctMineralCount;
    int correctGasCount;
};
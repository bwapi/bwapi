#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class TrainTest : public TestCase
{
  public:
    TrainTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    BWAPI::UnitType unitType;
    BWAPI::UnitType producerType;
    int previousUnitCount;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* producer;
    int correctMineralCount;
    int correctGasCount;
    int correctSupplyUsedCount;
};
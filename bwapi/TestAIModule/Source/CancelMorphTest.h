#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class CancelMorphTest : public TestCase
{
  public:
    CancelMorphTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning(); 
  private:
    bool running;
    bool fail;
    BWAPI::UnitType unitType;
    BWAPI::UnitType producerType;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* producer;
    int correctMineralCount;
    int correctGasCount;
    int correctSupplyUsedCount;
    int correctCompletedUnitCount;
    int correctIncompleteUnitCount;
    int correctAllUnitCount;
};
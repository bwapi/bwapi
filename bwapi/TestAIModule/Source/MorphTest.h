#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class MorphTest : public TestCase
{
  public:
    MorphTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning(); 
  private:
    bool running;
    bool fail;
    BWAPI::UnitType unitType;
    BWAPI::UnitType producerType;
    int previousUnitCount;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* producer;
    bool finishingMorph;
    int finishFrame;
    int correctMineralCount;
    int correctGasCount;
    int correctSupplyUsedCount;
};
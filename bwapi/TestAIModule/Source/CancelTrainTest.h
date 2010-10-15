#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class CancelTrainTest : public TestCase
{
  public:
    CancelTrainTest(BWAPI::UnitType unitType1,BWAPI::UnitType unitType2,BWAPI::UnitType unitType3);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    bool verifyTrainingQueue();
    BWAPI::UnitType unitType1;
    BWAPI::UnitType unitType2;
    BWAPI::UnitType unitType3;
    BWAPI::UnitType producerType;
    BWAPI::Unit* producer;
    int startFrame;
    int nextFrame;
    int correctMineralCount;
    int correctGasCount;
    int correctSupplyUsedCount;
    int originalMineralCount;
    int originalGasCount;
    int originalSupplyUsedCount;
    int originalAllUnit1Count;
    int originalAllUnit2Count;
    int originalAllUnit3Count;
    std::list<BWAPI::UnitType> correctTrainingQueue;
    enum State
    {
      Start,
      CancelledFirstSlot,
      CancelledLast1,
      CancelledLast2
    } state;
};
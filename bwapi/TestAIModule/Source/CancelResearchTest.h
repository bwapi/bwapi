#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class CancelResearchTest : public TestCase
{
  public:
    CancelResearchTest(BWAPI::TechType techType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning(); 
  private:
    bool running;
    bool fail;
    BWAPI::TechType techType;
    BWAPI::UnitType researcherType;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* researcher;
    int correctMineralCount;
    int correctGasCount;
    int correctSupplyUsedCount;
};
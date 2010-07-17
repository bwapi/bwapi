#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class ResearchTest : public TestCase
{
  public:
    ResearchTest(BWAPI::TechType techType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    BWAPI::TechType techType;
    BWAPI::UnitType researcherType;
    int startResearchFrame;
    int nextUpdateFrame;
    BWAPI::Unit* researcher;
    int correctMineralCount;
    int correctGasCount;
};
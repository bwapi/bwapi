#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class CancelConstructionTest : public TestCase
{
  public:
    CancelConstructionTest(BWAPI::UnitType unitType, bool terranHalt=false);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    BWAPI::UnitType unitType;
    BWAPI::UnitType builderType;
    int previousUnitCount;
    int startFrame;
    int nextFrame;
    int finishFrame;
    BWAPI::TilePosition buildLocation;
    BWAPI::Unit* builder;
    BWAPI::Unit* building;
    int correctMineralCount;
    int correctGasCount;
    int correctSupplyUsedCount;
    bool terranHalt;
};
#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class BuildTest : public TestCase
{
  public:
    BuildTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning(); 
  private:
    bool running;
    bool fail;
    BWAPI::UnitType unitType;
    BWAPI::UnitType builderType;
    int previousUnitCount;
    int startFrame;
    int nextFrame;
    int finishFrame;
    BWAPI::TilePosition buildLocation;
    BWAPI::Unit* builder;
    BWAPI::Unit* building;
    bool finishingBuilding;
};
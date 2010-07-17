#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class GatherTest : public TestCase
{
  public:
    GatherTest();
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning();
  private:
    bool running;
    bool fail;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* worker;
    BWAPI::Unit* mineral;
    BWAPI::Unit* refinery;
    int initialMinerals;
    int initialGas;
    int state;
};
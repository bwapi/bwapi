#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class SiegeTest : public TestCase
{
  public:
    SiegeTest();
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning();
  private:
    bool running;
    bool fail;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* unit;
    bool completedSiege;
};
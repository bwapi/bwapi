#pragma once
#include "TestCase.h"
#include <BWAPI.h>

class UseTechTest : public TestCase
{
  public:
    UseTechTest(BWAPI::TechType techType);
    virtual void start();
    virtual void update();
    virtual void stop();
    virtual bool isRunning(); 
  private:
    bool running;
    bool fail;
    BWAPI::TechType techType;
    BWAPI::UnitType userType;
    int startFrame;
    int nextFrame;
    BWAPI::Unit* user;
};
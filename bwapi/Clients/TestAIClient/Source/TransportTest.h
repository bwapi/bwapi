#pragma once
#include "TestCase.h"
#include <BWAPI.h>
#include <vector>

#include <BWAPi/Unitset.h>

class TransportTest : public TestCase
{
  public:
    TransportTest(BWAPI::UnitType unitType);
    virtual void start();
    virtual void update();
    virtual void stop();
  private:
    bool verifyLoadedUnits();
    int startFrame;
    int nextFrame;
    BWAPI::UnitType transportType;
    BWAPI::Unit transport;
    BWAPI::Unit currentUnit;
    BWAPI::Unitset unloadedUnits;
    BWAPI::Unitset loadedUnits;
    int waitFrameStart;
    enum LoadState
    {
      Init,
      Start,
      WaitForFirstUnit,
      WaitForSecondUnit,
      WaitForThirdUnit,
      WaitForFourthUnit,
      WaitDelay1,
      WaitUnloadUnit,
      WaitReloadUnit,
      WaitUnloadAll,
      WaitReloadAll,
      WaitUnloadAllPosition,
      End
    } state;
};
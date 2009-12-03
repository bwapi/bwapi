#pragma once
#include <..\..\BWAPITypes\UnitStruct.h>

namespace BWAgent
{
  class Unit
  {
  public:
    Unit();
    int getID();
    int getHitPoints();
    bool holdPosition();
    void _update(BWAPI::ClearanceLevels level,BWAPI::State* data);
  private:
    BWAPI::ClearanceLevels level;//type of struct that data points to
    BWAPI::State* data;
  };
}

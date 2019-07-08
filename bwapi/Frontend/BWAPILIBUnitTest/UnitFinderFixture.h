#pragma once

#include "GameFixture.h"

struct UnitCounter
{
  int count = 0;

  void operator()(BWAPI::Unit)
  {
    ++count;
  }
};

class UnitFinderFixture : public GameFixture {
protected:
  BWAPI::UnitFinder finder;

  int countUnits(int left = 0, int top = 0, int right = 10000, int bottom = 10000)
  {
    return finder.find(left, top, right, bottom).size();
  }
};

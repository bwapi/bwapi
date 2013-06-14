#pragma once
#include <BWAPI.h>
using namespace BWAPI;
using namespace Filter;

struct Owns
{
  UnitType type;
  int amount;
  Owns(int n, UnitType t) : type(t), amount(n) {}
  bool operator()(Unit )
  {
    return Broodwar->self() ? Broodwar->self()->completedUnitCount(type) == amount : false;
  }
};

struct Timeout
{
  int endTime;

  Timeout(int t) : endTime(Broodwar->getFrameCount() + t) {}
  bool operator() (Unit )
  {
    return endTime < Broodwar->getFrameCount();
  }
};

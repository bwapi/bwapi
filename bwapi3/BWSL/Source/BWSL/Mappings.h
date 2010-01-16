#pragma once
#include <vector>
#include <list>
#include <map>
#include <set>
namespace BWSL
{
  class Unit;
  extern std::set<Unit*> allUnits;
  extern std::map<int, Unit*> unitIdToObject;
}

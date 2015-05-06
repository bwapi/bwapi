#pragma once
#include <utility>
#include <BWAPI.h>
namespace BWTA
{
  class Region;
  class Chokepoint
  {
  public:
    virtual const std::pair<Region*,Region*>& getRegions() const=0;
    virtual const std::pair<BWAPI::Position,BWAPI::Position>& getSides() const=0;
    virtual BWAPI::Position getCenter() const=0;
    virtual double getWidth() const=0;
  };
}
#pragma once

#include <BWAPI.h>

#include <list>
#include <map>

namespace BWAI { class Unit; }

namespace BWAI
{
  class Formation
  {
    public :
      Formation(std::list<Unit*>& units);
      void generatePositions(BWAPI::Position center, float angle);
      void execute();
    private :
      class Target
      {
        public :
          Unit* unit;
          BWAPI::Position target;
          Target(Unit* unit, BWAPI::Position target) : unit(unit), target(target) {}
      };
      std::map<BWAPI::UnitType, std::list<Target> > data;
  };
}
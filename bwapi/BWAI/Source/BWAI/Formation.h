#pragma once

#include <BWAPI.h>
#include <BW/Position.h>

#include <list>
#include <map>

namespace BWAI { class Unit; }

namespace BWAI
{
  class Formation
  {
    public :
      Formation(std::list<Unit*>& units);
      void generatePositions(BW::Position center, float angle);
      void execute();
    private :
      class Target
      {
        public :
          Unit* unit;
          BW::Position target;
          Target(Unit* unit, BW::Position target) : unit(unit), target(target) {}
      };
      std::map<BWAPI::UnitType, std::list<Target> > data;
  };
}
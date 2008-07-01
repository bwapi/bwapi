#pragma once

#include <BW/UnitID.h>
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
      std::map<BW::UnitID::Enum, std::list<Target> > data;
  };
}
#pragma once
#include <vector>
namespace BWAI { class Expansion; }
namespace BWAI { class Unit; }

namespace BWAI
{
  class Mineral
  {
    public :
      Mineral(BWAI::Unit* mineral, Expansion* expansion);
      ~Mineral();
      void assignGatherer(BWAI::Unit* gatherer);
      BWAI::Unit* mineral;
      std::vector<BWAI::Unit*> gatherersAssigned;
      Expansion* expansion;
      bool removeGatherer(Unit* unit);
      bool checkAssignedWorkers();
      bool SomeoneIsMining(void);
      void checkDeadWorkers(void);
  };
}
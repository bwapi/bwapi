#pragma once
namespace BWAPI { class Unit; };
#include <vector>
#include "Mineral.h"

namespace BWAI
{
  class Expansion
  {
    public :
      Expansion(BWAI::Unit* gatherCenter);
      ~Expansion();
      BWAI::Unit* gatherCenter;
      std::vector<Mineral*> minerals;
      int asignedWorkers;
      void removeWorker(Unit* worker);
      void checkAssignedWorkers();
      void removeMineral(BWAI::Unit* mineral);
      static int maximumMineralDistance;
  };
}
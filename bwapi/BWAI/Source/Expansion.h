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
      bool checkAssignedWorkers();
      void checkDeadWorkers();
      void removeMineral(BWAI::Unit* mineral);
  };
}
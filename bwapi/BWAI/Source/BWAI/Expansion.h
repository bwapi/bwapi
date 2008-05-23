#pragma once

#include <list>

namespace BWAI { class TaskGather; };
namespace BWAI { class Unit; };

namespace BWAI
{
  class Expansion
  {
    public :
      Expansion(BWAI::Unit* gatherCenter);
      ~Expansion();
      
      BWAI::Unit* gatherCenter;
      std::list<TaskGather*> minerals;
      
      void removeMineral(Unit* mineral);
      static int maximumMineralDistance;
  };
}
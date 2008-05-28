#pragma once

#include <list>

namespace BWAI { class TaskGather; };
namespace BWAI { class TaskGatherGas; };
namespace BWAI { class Unit; };

namespace BWAI
{
  /** Represents active expansion of the ai. */
  class Expansion
  {
    public :
      Expansion(BWAI::Unit* gatherCenter);
      ~Expansion();
      
      BWAI::Unit* gatherCenter;
      std::list<TaskGather*> minerals;
     // std::list<TaskGatherGas*> gasMine;
      
      void removeMineral(Unit* mineral);
      static int maximumMineralDistance;
  };
}
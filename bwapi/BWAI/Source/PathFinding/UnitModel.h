#pragma once

namespace BWAI { class Unit; }

#include "WalkabilityPosition.h"

#include <BWAPI.h>

namespace PathFinding
{
  /** 
   * Shadow of unit that can be manipulated for the need of Path finding.
   */
  class UnitModel
  {
    public :
      UnitModel(const UnitModel& model);
      UnitModel(const BWAI::Unit* original);
      /** The unit that made this shadow - is there to avoid collision with itself. */
      const BWAI::Unit* original;  
      /** Position of the shadow, will be modified during the pathfinding algorithm. */
      BWAPI::Position position;
      WalkabilityPosition walkabilityPosition;
  };
}
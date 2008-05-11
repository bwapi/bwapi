#include "..//..//BWAPI//Source//BWAPI//Unit.h"
namespace BWAI { class Expansion; }

namespace BWAI
{
  /** 
   * Wrapper of BWAPI unit type with included information that uses BWAI
   */
  class Unit : public BWAPI::Unit
  {
    public :
      Unit(BWAPI::Unit *unit);
      Expansion* expansionAssingment;
      bool selected;
      BW::UnitType::Enum lastTrainedUnitID;
      static Unit* BWUnitToBWAIUnit(BW::UnitData* unit);
      /** 
       * Gets #bwUnitData->BW#UnitData#targetUnit. 
       * This is similar to BWAPI#Unit#getTarget, it just get's the unit from ai scope
       */
      Unit* getTarget();
      Unit* getTargetLocal();
  };
}
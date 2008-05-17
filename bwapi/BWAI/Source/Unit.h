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
      static Unit* BWUnitToBWAIUnit(BW::Unit* unit);
      /** 
       * Gets #bwUnit->BW#Unit#targetUnit. 
       * This is similar to BWAPI#Unit#getTarget, it just get's the unit from ai scope
       */
      Unit* getTarget();
      Unit* getTargetLocal();
      Unit* getNext();
      /** Updates pointer to next unit (and recursively updates that unit. */
      void updateNext();
      int lastFrameSpam;
    private :
      Unit *next; /**< Corresponds to bwOriginalUnit->nextUnit, precomputed pointer - to optimize unit listing. */
  };
}
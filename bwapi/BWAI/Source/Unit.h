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
  };
}
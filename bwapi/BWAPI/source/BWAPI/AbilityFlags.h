#include "..//Types.h"
namespace BWAPI
{
  /** 
   * Flag values specifying abilities that can perform certain unit types in BWAPI#UnitPrototype#abilityFlags
   */
  namespace AbilityFlags
  {
    enum Enum : u32 
    {
      Move            = 1 << 0,
      AttackMove      = 1 << 1,
      Ensare          = 1 << 2,
      Broodling       = 1 << 3,
      WarpArchon      = 1 << 4,
      WarpDarkArchon  = 1 << 5,
      SiegeTank       = 1 << 6,
      UnsiegeTank     = 1 << 7,
      MorphToLurker   = 1 << 8,
      MorphToGuardian = 1 << 9,
      Stims           = 1 << 10
    };
  }
}
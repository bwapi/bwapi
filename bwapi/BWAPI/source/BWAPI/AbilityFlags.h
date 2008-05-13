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
      Attack          = 1 << 1,
      AttackMove      = 1 << 2,
      Ensare          = 1 << 3,
      Broodling       = 1 << 4,
      WarpArchon      = 1 << 5,
      WarpDarkArchon  = 1 << 6,
      SiegeTank       = 1 << 7,
      UnsiegeTank     = 1 << 8,
      Gather          = 1 << 9,
      MorphToLurker   = 1 << 10,
      MorphToGuardian = 1 << 11,
      Stims           = 1 << 12
    };
  }
}
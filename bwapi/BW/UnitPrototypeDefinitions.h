#pragma once
#include "AbilityPrototype.h"
#include "UnitPrototype.h"
#include "TargetType.h"

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
namespace Prototypes
{
  /*                c++ ability name                       ID Name               mana   minerals gas */
  extern AbilityPrototype* Move;
  extern AbilityPrototype* Attack;
  extern AbilityPrototype* AttackMove;
  extern AbilityPrototype* Ensare;
  extern AbilityPrototype* Broodling; 
  extern AbilityPrototype* WarpArchon;
  extern AbilityPrototype* SiegeTank;
  extern AbilityPrototype* UnsiegeTank;
  extern AbilityPrototype* Gather;
  extern AbilityPrototype* MorphToLurker;
  extern AbilityPrototype* MorphToGuardian;
  extern AbilityPrototype* Stims;
  /*                                        Unit name         maxHp  maxSP*/
  extern UnitPrototype* SCV;
  extern UnitPrototype* Probe;
  extern UnitPrototype* Zealot;
  extern UnitPrototype* Queen;
}
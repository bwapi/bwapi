#include "UnitPrototypeDefinitions.h"
namespace BWAPI
{
  namespace Prototypes
  {
    /*                c++ ability name                       ID Name               mana   minerals gas */
    AbilityPrototype* Move            = new AbilityPrototype( 0,"Move",                0,         0,  0, TargetType::TARGET_LOCATION | 
                                                                                                         TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                         TargetType::TARGET_MY_UNITS |
                                                                                                         TargetType::TARGET_BIOLOGICAL | 
                                                                                                         TargetType::TARGET_MECHANICAL | 
                                                                                                         TargetType::TARGET_ROBOTIC);
    AbilityPrototype* Attack          = new AbilityPrototype( 1,"Attack",              0,         0,  0, TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                         TargetType::TARGET_MY_UNITS |
                                                                                                         TargetType::TARGET_BIOLOGICAL | 
                                                                                                         TargetType::TARGET_MECHANICAL | 
                                                                                                         TargetType::TARGET_ROBOTIC);
    AbilityPrototype* AttackMove      = new AbilityPrototype( 2,"Attack move",         0,        0,   0, TargetType::TARGET_LOCATION);
    AbilityPrototype* Ensare          = new AbilityPrototype( 3,"Ensare",             75,        0,   0, TargetType::TARGET_LOCATION |
                                                                                                         TargetType::TARGET_OPPONENTS_UNITS |
                                                                                                         TargetType::TARGET_MY_UNITS |
                                                                                                         TargetType::TARGET_BIOLOGICAL | 
                                                                                                         TargetType::TARGET_MECHANICAL | 
                                                                                                         TargetType::TARGET_ROBOTIC);
    AbilityPrototype* Broodling       = new AbilityPrototype( 4,"Broodling",         150,        0,   0, TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                         TargetType::TARGET_MY_UNITS | 
                                                                                                         TargetType::TARGET_BIOLOGICAL |
                                                                                                         TargetType::TARGET_MECHANICAL); 
    AbilityPrototype* WarpArchon      = new AbilityPrototype( 5, "Warp archon",        0,        0,   0, TargetType::TARGET_MY_UNITS | 
                                                                                                         TargetType::TARGET_BIOLOGICAL |
                                                                                                         TargetType::TARGET_TEMPLAR);
    AbilityPrototype* WarpDarkArchon  = new AbilityPrototype( 6, "Warp archon",        0,        0,   0, TargetType::TARGET_MY_UNITS | 
                                                                                                         TargetType::TARGET_BIOLOGICAL |
                                                                                                         TargetType::TARGET_TEMPLAR);
    AbilityPrototype* SiegeTank       = new AbilityPrototype( 7, "Siege tank",         0,        0,   0, TargetType::TARGET_SELF);
    AbilityPrototype* UnsiegeTank     = new AbilityPrototype( 8, "Unsiege tank",       0,        0,   0, TargetType::TARGET_SELF);
    AbilityPrototype* Gather          = new AbilityPrototype( 9, "Gather",             0,        0,   0, TargetType::TARGET_RESOURCE |
                                                                                                         TargetType::TARGET_OPPONENTS_UNITS);
    AbilityPrototype* MorphToLurker   = new AbilityPrototype(10, "Morph to lurker",    0,       50, 100, TargetType::TARGET_SELF);
    AbilityPrototype* MorphToGuardian = new AbilityPrototype(11, "Morph to guardian",  0,       50, 100, TargetType::TARGET_SELF);
    AbilityPrototype* Stims           = new AbilityPrototype(11, "Use stimpacks",      0,        0,   0, TargetType::TARGET_SELF);
    
    s32 MOVABLE_CAN_ATTACK = AbilityFlags::Move | AbilityFlags::AttackMove | AbilityFlags::Attack;
    /*                                        Unit name         maxHp  maxSP*/
    UnitPrototype* SCV    = new UnitPrototype("Terran SCV"    ,    60,    0, MOVABLE_CAN_ATTACK | AbilityFlags::Stims);
    UnitPrototype* Probe  = new UnitPrototype("Protoss Probe" ,    20,    0, MOVABLE_CAN_ATTACK | AbilityFlags::Gather);
    UnitPrototype* Zealot = new UnitPrototype("Protoss Zealot",    20,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Queen  = new UnitPrototype("Zerg Queen"    ,   100,  200, AbilityFlags::Move | AbilityFlags::Ensare | AbilityFlags::Broodling);
  }
}
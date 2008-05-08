#pragma once

#include "../BWAPI/AbilityPrototype.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/TargetType.h"

namespace BWAPI
{
  /** Ability and unit prototype definitions */
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
    /** Terran */
    extern UnitPrototype* SCV;
    extern UnitPrototype* Marine;
    extern UnitPrototype* CommandCenter;
    extern UnitPrototype* SupplyDepot;
    extern UnitPrototype* Barracks;
    /** Protoss */
    extern UnitPrototype* Probe;
    extern UnitPrototype* Zealot;
    extern UnitPrototype* Nexus;
    extern UnitPrototype* Pylon;
    extern UnitPrototype* Gateway;
   /** Zerg */
    extern UnitPrototype* Drone;
    extern UnitPrototype* Larva;
    extern UnitPrototype* Hatchery;
    extern UnitPrototype* Overlord;
    extern UnitPrototype* SpawningPool;
    extern UnitPrototype* Zergling;
    extern UnitPrototype* Queen;
    extern UnitPrototype* Mutalisk;
    /** */
    extern UnitPrototype* Minerals;
  }
};

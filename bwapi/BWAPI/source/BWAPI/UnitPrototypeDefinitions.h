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
    extern UnitPrototype* Firebat;
    extern UnitPrototype* Medic;
    extern UnitPrototype* SiegeTank_TankMode;
    extern UnitPrototype* TankTurret_TankMode;
    extern UnitPrototype* SiegeTank_SiegeMode;
    extern UnitPrototype* CommandCenter;
    extern UnitPrototype* SupplyDepot;
    extern UnitPrototype* Barracks;
    extern UnitPrototype* Goliath;
    extern UnitPrototype* GoliathTurret;
    extern UnitPrototype* Ghost;
    extern UnitPrototype* Wraith;
    extern UnitPrototype* BattleCruiser;
    extern UnitPrototype* Vulture;
    extern UnitPrototype* NuclearMissile;
    /** Protoss */
    extern UnitPrototype* Probe;
    extern UnitPrototype* Zealot;
    extern UnitPrototype* HighTemplar;
    extern UnitPrototype* Dragoon;
    extern UnitPrototype* Nexus;
    extern UnitPrototype* Pylon;
    extern UnitPrototype* Gateway;
    extern UnitPrototype* CyberneticsCore;
   /** Zerg */
    extern UnitPrototype* Drone;
    extern UnitPrototype* Larva;
    extern UnitPrototype* Hatchery;
    extern UnitPrototype* Overlord;
    extern UnitPrototype* SpawningPool;
    extern UnitPrototype* Zergling;
    extern UnitPrototype* Queen;
    extern UnitPrototype* Mutalisk;
    /** other */
    extern UnitPrototype* Minerals1;
    extern UnitPrototype* Minerals2;
    extern UnitPrototype* Minerals3;
    extern UnitPrototype* VaspineGayser;
  }
};

#pragma once
namespace BWAPI { class AbilityPrototype; }
namespace BWAPI { class UnitPrototype; }


namespace BWAPI
{
  /** Ability and unit prototype definitions */
  namespace Prototypes
  {
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
    extern UnitPrototype* ScienceVessel;
    extern UnitPrototype* Valkyrie;
    extern UnitPrototype* ComsatStation;
    extern UnitPrototype* NuclearSilo;
    extern UnitPrototype* Refinery;
    extern UnitPrototype* Academy;
    extern UnitPrototype* Factory;
    extern UnitPrototype* ControlTower;
    extern UnitPrototype* ScienceFacility;
    extern UnitPrototype* CovertOps;
    extern UnitPrototype* PhysicsLab;
    extern UnitPrototype* MachineShop;
    extern UnitPrototype* EngineeringBay;
    extern UnitPrototype* Armory;
    extern UnitPrototype* MissileTurret;
    extern UnitPrototype* Bunker;
    extern UnitPrototype* Starport;

    /** Protoss */
    extern UnitPrototype* Probe;
    extern UnitPrototype* Zealot;
    extern UnitPrototype* HighTemplar;
    extern UnitPrototype* Dragoon;
    extern UnitPrototype* Nexus;
    extern UnitPrototype* Pylon;
    extern UnitPrototype* Gateway;
    extern UnitPrototype* CyberneticsCore;
    extern UnitPrototype* PhotonCannon;
    extern UnitPrototype* Forge;
    extern UnitPrototype* Archon;
    extern UnitPrototype* ShieldBattery;
    extern UnitPrototype* TemplarArchives;
    extern UnitPrototype* DarkTemplar;
    extern UnitPrototype* RoboticsFacility;
    
   /** Zerg */
    extern UnitPrototype* Drone;
    extern UnitPrototype* Larva;
    extern UnitPrototype* Hatchery;
    extern UnitPrototype* Overlord;
    extern UnitPrototype* SpawningPool;
    extern UnitPrototype* Zergling;
    extern UnitPrototype* Queen;
    extern UnitPrototype* Mutalisk;
    extern UnitPrototype* Spire;
    extern UnitPrototype* Lair;
    extern UnitPrototype* CreepColony;
    extern UnitPrototype* SunkenColony;
    extern UnitPrototype* Extractor;
    extern UnitPrototype* SporeColony;
    extern UnitPrototype* HydraliskDen;
    
    
    /** other */
    extern UnitPrototype* MineralPatch1;
    extern UnitPrototype* MineralPatch2;
    extern UnitPrototype* MineralPatch3;
    extern UnitPrototype* VaspineGayser;
    extern UnitPrototype* None;

    //extern UnitPrototype* StartLocation;
    extern UnitPrototype* Critter_Bengalaas;
    extern UnitPrototype* Critter_Kakaru;
    extern UnitPrototype* Critter_Ragnasaur;
    extern UnitPrototype* Critter_Scantid;
    /** Spells */
    extern UnitPrototype* DarkSwarm;
    
    
    extern UnitPrototype* unitIDToPrototypeTable[228];
    }
};

#include "AbilityPrototype.h"
#include "UnitPrototype.h"
#include "RaceTypes.h"
#include "AbilityFlags.h"
#include "TargetType.h"

#include "..//BW//UnitTypes.h"

namespace BWAPI
{
  namespace Prototypes
  {
    /*                c++ ability name                       ID Name               mana   minerals gas */
    AbilityPrototype* Move            = new AbilityPrototype( 0, "Move",                0,         0,  0, TargetType::TARGET_LOCATION | 
                                                                                                          TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                          TargetType::TARGET_MY_UNITS |
                                                                                                          TargetType::TARGET_BIOLOGICAL | 
                                                                                                          TargetType::TARGET_MECHANICAL | 
                                                                                                          TargetType::TARGET_ROBOTIC);
    AbilityPrototype* Attack          = new AbilityPrototype( 1, "Attack",              0,         0,  0, TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                          TargetType::TARGET_MY_UNITS |
                                                                                                          TargetType::TARGET_BIOLOGICAL | 
                                                                                                          TargetType::TARGET_MECHANICAL | 
                                                                                                          TargetType::TARGET_ROBOTIC);
    AbilityPrototype* AttackMove      = new AbilityPrototype( 2, "Attack move",         0,        0,   0, TargetType::TARGET_LOCATION);
    AbilityPrototype* Ensare          = new AbilityPrototype( 3, "Ensare",             75,        0,   0, TargetType::TARGET_LOCATION |
                                                                                                          TargetType::TARGET_OPPONENTS_UNITS |
                                                                                                          TargetType::TARGET_MY_UNITS |
                                                                                                          TargetType::TARGET_BIOLOGICAL | 
                                                                                                          TargetType::TARGET_MECHANICAL | 
                                                                                                          TargetType::TARGET_ROBOTIC);
    AbilityPrototype* Broodling       = new AbilityPrototype( 4, "Broodling",         150,        0,   0, TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                          TargetType::TARGET_MY_UNITS | 
                                                                                                          TargetType::TARGET_BIOLOGICAL |
                                                                                                          TargetType::TARGET_MECHANICAL); 
    AbilityPrototype* WarpArchon      = new AbilityPrototype( 5, "Warp archon",         0,        0,   0, TargetType::TARGET_MY_UNITS | 
                                                                                                          TargetType::TARGET_BIOLOGICAL |
                                                                                                          TargetType::TARGET_TEMPLAR);
    AbilityPrototype* WarpDarkArchon  = new AbilityPrototype( 6, "Warp archon",         0,        0,   0, TargetType::TARGET_MY_UNITS | 
                                                                                                          TargetType::TARGET_BIOLOGICAL |
                                                                                                          TargetType::TARGET_TEMPLAR);
    AbilityPrototype* SiegeTank       = new AbilityPrototype( 7, "Siege tank",          0,        0,   0, TargetType::TARGET_SELF);
    AbilityPrototype* UnsiegeTank     = new AbilityPrototype( 8, "Unsiege tank",        0,        0,   0, TargetType::TARGET_SELF);
    AbilityPrototype* Gather          = new AbilityPrototype( 9, "Gather",              0,        0,   0, TargetType::TARGET_RESOURCE |
                                                                                                          TargetType::TARGET_OPPONENTS_UNITS);
    AbilityPrototype* MorphToLurker   = new AbilityPrototype(10, "Morph to lurker",     0,       50, 100, TargetType::TARGET_SELF);
    AbilityPrototype* MorphToGuardian = new AbilityPrototype(11, "Morph to guardian",   0,       50, 100, TargetType::TARGET_SELF);
    AbilityPrototype* Stims           = new AbilityPrototype(11, "Use stimpacks",       0,        0,   0, TargetType::TARGET_SELF);
    
    s32 MOVABLE_CAN_ATTACK = AbilityFlags::Move | AbilityFlags::AttackMove | AbilityFlags::Attack;
    /*                                                Unit name                 ID                                    Race                sp  target flags*/
    /** Terran */
    UnitPrototype* SCV                 = new UnitPrototype("Terran SCV"                   , BW::UnitType::Terran_SCV,                RaceType::Terran,    0, MOVABLE_CAN_ATTACK | AbilityFlags::Gather);
    UnitPrototype* Marine              = new UnitPrototype("Terran Marine"                , BW::UnitType::Terran_Marine,             RaceType::Terran,    0, MOVABLE_CAN_ATTACK | AbilityFlags::Stims);
    UnitPrototype* Firebat             = new UnitPrototype("Terran Firebat"               , BW::UnitType::Terran_Firebat,            RaceType::Terran,    0, MOVABLE_CAN_ATTACK | AbilityFlags::Stims);
    UnitPrototype* SiegeTank_TankMode  = new UnitPrototype("Siege Tank (TankMode)"        , BW::UnitType::Terran_SiegeTankTankMode,  RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* TankTurret_TankMode = new UnitPrototype("Tank Turret (TankMode)"       , BW::UnitType::Terran_TankTurretTankMode, RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* SiegeTank_SiegeMode = new UnitPrototype("Siege Tank (Siege Mode)"      , BW::UnitType::Terran_SiegeTankSiegeMode, RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
                                                                                      
    UnitPrototype* CommandCenter       = new UnitPrototype("Terran Command Center"        , BW::UnitType::Terran_CommandCenter,      RaceType::Terran,    0, 0);
    UnitPrototype* SupplyDepot         = new UnitPrototype("Terran Supply depot"          , BW::UnitType::Terran_SupplyDepot,        RaceType::Terran,    0, 0);
    UnitPrototype* Barracks            = new UnitPrototype("Terran Barracks"              , BW::UnitType::Terran_Barracks,           RaceType::Terran,    0, 0);
    UnitPrototype* Goliath             = new UnitPrototype("Goliath"                      , BW::UnitType::Terran_Goliath,            RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* GoliathTurret       = new UnitPrototype("Goliath Turret"               , BW::UnitType::Terran_GoliathTurret,      RaceType::Terran,    0, 0);
    UnitPrototype* Ghost               = new UnitPrototype("Ghost"                        , BW::UnitType::Terran_Ghost,              RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Medic               = new UnitPrototype("Medic"                        , BW::UnitType::Terran_Medic,              RaceType::Terran,    0, AbilityFlags::Move);
    UnitPrototype* Wraith              = new UnitPrototype("Wraith"                       , BW::UnitType::Terran_Wraith,             RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* BattleCruiser       = new UnitPrototype("Terran Battle Cruiser"        , BW::UnitType::Terran_BattleCruiser,      RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Vulture             = new UnitPrototype("Terran Vulture"               , BW::UnitType::Terran_Vulture,            RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* NuclearMissile      = new UnitPrototype("Terran NuclearMissile"        , BW::UnitType::Terran_NuclearMissile,     RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* ScienceVessel       = new UnitPrototype("Science Vessel"               , BW::UnitType::Terran_ScienceVessel,      RaceType::Terran,    0, AbilityFlags::Move);
    UnitPrototype* Valkyrie            = new UnitPrototype("Valkyrie"                     , BW::UnitType::Terran_Valkyrie,           RaceType::Terran,    0, MOVABLE_CAN_ATTACK);
    UnitPrototype* ComsatStation       = new UnitPrototype("Comsat Station"               , BW::UnitType::Terran_ComsatStation,      RaceType::Terran,    0, 0);
    UnitPrototype* NuclearSilo         = new UnitPrototype("Nuclear silo"                 , BW::UnitType::Terran_NuclearSilo,        RaceType::Terran,    0, 0);
    UnitPrototype* Refinery            = new UnitPrototype("Refinery"                     , BW::UnitType::Terran_Refinery,           RaceType::Terran,    0, 0);
    UnitPrototype* Academy             = new UnitPrototype("Academy"                      , BW::UnitType::Terran_Academy,            RaceType::Terran,    0, 0);
    UnitPrototype* Factory             = new UnitPrototype("Factory"                      , BW::UnitType::Terran_Factory,            RaceType::Terran,    0, 0);
    UnitPrototype* ControlTower        = new UnitPrototype("Control tower"                , BW::UnitType::Terran_ControlTower,       RaceType::Terran,    0, 0);
    UnitPrototype* ScienceFacility     = new UnitPrototype("Science Facility"             , BW::UnitType::Terran_ScienceFacility,    RaceType::Terran,    0, 0);
    UnitPrototype* CovertOps           = new UnitPrototype("Covert Ops"                   , BW::UnitType::Terran_CovertOps,          RaceType::Terran,    0, 0);
    UnitPrototype* PhysicsLab          = new UnitPrototype("Physics Lab"                  , BW::UnitType::Terran_PhysicsLab,         RaceType::Terran,    0, 0);
    UnitPrototype* Armory              = new UnitPrototype("Armory"                       , BW::UnitType::Terran_Armory,             RaceType::Terran,    0, 0);
    UnitPrototype* MissileTurret       = new UnitPrototype("Missile Turret"               , BW::UnitType::Terran_MissileTurret,      RaceType::Terran,    0, 0);
    UnitPrototype* Bunker              = new UnitPrototype("Bunker"                       , BW::UnitType::Terran_Bunker,             RaceType::Terran,    0, 0);
    UnitPrototype* EngineeringBay      = new UnitPrototype("Engineering Bay"              , BW::UnitType::Terran_EngineeringBay,     RaceType::Terran,    0, 0);
    UnitPrototype* MachineShop         = new UnitPrototype("Machine Shop"                 , BW::UnitType::Terran_MachineShop,        RaceType::Terran,    0, 0);
    UnitPrototype* Starport            = new UnitPrototype("Starport"                     , BW::UnitType::Terran_Starport,        RaceType::Terran,    0, 0);


    /** Protoss*/
    UnitPrototype* Probe               = new UnitPrototype("Protoss Probe"                , BW::UnitType::Protoss_Probe,             RaceType::Protoss,   0, MOVABLE_CAN_ATTACK | AbilityFlags::Gather);
    UnitPrototype* Zealot              = new UnitPrototype("Protoss Zealot"               , BW::UnitType::Protoss_Zealot,            RaceType::Protoss,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* HighTemplar         = new UnitPrototype("Protoss High Templar"         , BW::UnitType::Protoss_HighTemplar,       RaceType::Protoss, 150, 0);
    UnitPrototype* Dragoon             = new UnitPrototype("Protoss Dragoon"              , BW::UnitType::Protoss_Dragoon,           RaceType::Protoss,   0, 0);
    UnitPrototype* Nexus               = new UnitPrototype("Protoss Nexus"                , BW::UnitType::Protoss_Nexus,             RaceType::Protoss,   0, 0);
    UnitPrototype* Pylon               = new UnitPrototype("Protoss Pylon"                , BW::UnitType::Protoss_Pylon,             RaceType::Protoss,   0, 0);
    UnitPrototype* Gateway             = new UnitPrototype("Protoss Gateway"              , BW::UnitType::Protoss_Gateway,           RaceType::Protoss,   0, 0);
    UnitPrototype* CyberneticsCore     = new UnitPrototype("Protoss Cybernetics core"     , BW::UnitType::Protoss_CyberneticsCore,   RaceType::Protoss,   0, 0);
    UnitPrototype* PhotonCannon        = new UnitPrototype("Protoss Photon cannon"        , BW::UnitType::Protoss_PhotonCannon,      RaceType::Protoss,   0, 0);
    UnitPrototype* Forge               = new UnitPrototype("Protoss Forge"                , BW::UnitType::Protoss_Forge,             RaceType::Protoss,   0, 0);
    UnitPrototype* Archon              = new UnitPrototype("Protoss Archon"               , BW::UnitType::Protoss_Archon,            RaceType::Protoss,   0, 0);
    UnitPrototype* ShieldBattery       = new UnitPrototype("Protoss Shield battery"       , BW::UnitType::Protoss_ShieldBattery,     RaceType::Protoss,   0, 0);
    UnitPrototype* TemplarArchives     = new UnitPrototype("Protoss Templar Archives"     , BW::UnitType::Protoss_TemplarArchives,   RaceType::Protoss,   0, 0);
    UnitPrototype* DarkTemplar         = new UnitPrototype("Protoss Dark Templar"         , BW::UnitType::Protoss_DarkTemplar,       RaceType::Protoss,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* RoboticsFacility    = new UnitPrototype("Protoss Robotics facility"    , BW::UnitType::Protoss_RoboticsFacility,  RaceType::Protoss,   0, 0);
    UnitPrototype* Assimilator         = new UnitPrototype("Protoss Assimilator"          , BW::UnitType::Protoss_Assimilator,       RaceType::Protoss,   0, 0);
    UnitPrototype* RoboticsSupportBay  = new UnitPrototype("Protoss Robotics Support bay" , BW::UnitType::Protoss_RoboticsSupportBay,RaceType::Protoss,   0, 0);
    UnitPrototype* Shuttle             = new UnitPrototype("Protoss Shuttle"              , BW::UnitType::Protoss_Shuttle,           RaceType::Protoss,   0, 0);
    UnitPrototype* Observer            = new UnitPrototype("Protoss Observer"             , BW::UnitType::Protoss_Observer,          RaceType::Protoss,   0, 0);
    UnitPrototype* Reaver              = new UnitPrototype("Protoss Reaver"               , BW::UnitType::Protoss_Reaver,            RaceType::Protoss,   0, 0);

    /** Zerg */
    UnitPrototype* Drone               = new UnitPrototype("Drone"                        , BW::UnitType::Zerg_Drone,                RaceType::Zerg,      0, MOVABLE_CAN_ATTACK | AbilityFlags::Gather);
    UnitPrototype* Larva               = new UnitPrototype("Larva"                        , BW::UnitType::Zerg_Larva,                RaceType::Zerg,      0, 0);
    UnitPrototype* Overlord            = new UnitPrototype("Overlord"                     , BW::UnitType::Zerg_Overlord,             RaceType::Zerg,      0, AbilityFlags::Move);
    UnitPrototype* Zergling            = new UnitPrototype("Zergling"                     , BW::UnitType::Zerg_Zergling,             RaceType::Zerg,      0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Egg                 = new UnitPrototype("Zerg Egg"                     , BW::UnitType::Zerg_Egg,                  RaceType::Zerg,      0, 0);
    UnitPrototype* Hydralisk           = new UnitPrototype("Zerg Hydralisk"               , BW::UnitType::Zerg_Hydralisk,            RaceType::Zerg,      0, 0);
    UnitPrototype* Queen               = new UnitPrototype("Zerg Queen"                   , BW::UnitType::Zerg_Queen,                RaceType::Zerg,    200, AbilityFlags::Move | AbilityFlags::Ensare | AbilityFlags::Broodling);
    UnitPrototype* Mutalisk            = new UnitPrototype("Zerg Mutalisk"                , BW::UnitType::Zerg_Mutalisk,             RaceType::Zerg,      0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Hatchery            = new UnitPrototype("Zerg Hatchery"                , BW::UnitType::Zerg_Hatchery,             RaceType::Zerg,      0, 0);
    UnitPrototype* SpawningPool        = new UnitPrototype("Zerg Spawning pool"           , BW::UnitType::Zerg_SpawningPool,         RaceType::Zerg,      0, 0);
    UnitPrototype* Spire               = new UnitPrototype("Zerg Spire"                   , BW::UnitType::Zerg_Spire,                RaceType::Zerg,      0, 0);
    UnitPrototype* Lair                = new UnitPrototype("Zerg Lair"                    , BW::UnitType::Zerg_Lair,                 RaceType::Zerg,      0, 0);
    UnitPrototype* Hive                = new UnitPrototype("Zerg Hive"                    , BW::UnitType::Zerg_Hive,                 RaceType::Zerg,      0, 0);
    UnitPrototype* CreepColony         = new UnitPrototype("Zerg Creep colony"            , BW::UnitType::Zerg_CreepColony,          RaceType::Zerg,      0, 0);
    UnitPrototype* SunkenColony        = new UnitPrototype("Zerg Sunken colony"           , BW::UnitType::Zerg_SunkenColony,         RaceType::Zerg,      0, 0);
    UnitPrototype* SporeColony         = new UnitPrototype("Zerg Spore colony"            , BW::UnitType::Zerg_SporeColony,          RaceType::Zerg,      0, 0);
    UnitPrototype* Extractor           = new UnitPrototype("Zerg extractor"               , BW::UnitType::Zerg_Extractor,            RaceType::Zerg,      0, 0);
    UnitPrototype* HydraliskDen        = new UnitPrototype("Zerg hydralisk den"           , BW::UnitType::Zerg_HydraliskDen,         RaceType::Zerg,      0, 0);
    UnitPrototype* Lurker              = new UnitPrototype("Zerg Lurker"                  , BW::UnitType::Zerg_Lurker,               RaceType::Zerg,      0, 0);
    UnitPrototype* QueensNest          = new UnitPrototype("Zerg Queens nest"             , BW::UnitType::Zerg_QueensNest,           RaceType::Zerg,      0, 0);
    UnitPrototype* EvolutionChamber    = new UnitPrototype("Zerg Evolution chamber"       , BW::UnitType::Zerg_EvolutionChamber,     RaceType::Zerg,      0, 0);
    UnitPrototype* Guardian            = new UnitPrototype("Zerg Guardian"                , BW::UnitType::Zerg_Guardian,             RaceType::Zerg,      0, 0);
    UnitPrototype* DefilerMound        = new UnitPrototype("Zerg Defiler mound"           , BW::UnitType::Zerg_DefilerMound,         RaceType::Zerg,      0, 0);
    UnitPrototype* Defiler             = new UnitPrototype("Zerg Defiler"                 , BW::UnitType::Zerg_Defiler,              RaceType::Zerg,      0, 0);
                                                                                         
                                                                                         
    /** Neutral */                                                                       
    UnitPrototype* MineralPatch1       = new UnitPrototype("Minerals1"                    , BW::UnitType::Resource_MineralPatch1,     RaceType::Other,     0, 0);    
    UnitPrototype* MineralPatch2       = new UnitPrototype("Minerals2"                    , BW::UnitType::Resource_MineralPatch2,     RaceType::Other,     0, 0);    
    UnitPrototype* MineralPatch3       = new UnitPrototype("Minerals3"                    , BW::UnitType::Resource_MineralPatch3,     RaceType::Other,     0, 0);    
    UnitPrototype* VaspineGayser       = new UnitPrototype("Vaspine gas"                  , BW::UnitType::Resource_VespeneGeyser,     RaceType::Other,     0, 0);    
                                                                                          
    /** Critters */                                                                     
    UnitPrototype* Critter_Bengalaas   = new UnitPrototype("Critter Bengalaas"            , BW::UnitType::Critter_Bengalaas,          RaceType::Other,     0, 0);    
    UnitPrototype* Critter_Kakaru      = new UnitPrototype("Critter Kakaru"               , BW::UnitType::Critter_Kakaru,             RaceType::Other,     0, 0);    
    UnitPrototype* Critter_Ragnasaur   = new UnitPrototype("Critter Rangasaur"            , BW::UnitType::Critter_Ragnasaur,          RaceType::Other,     0, 0);    
    UnitPrototype* Critter_Scantid     = new UnitPrototype("Critter_Scantid"              , BW::UnitType::Critter_Scantid,            RaceType::Other,     0, 0);    
                                                                                         
                                                                                         
    /** Spells */                                                                        
    UnitPrototype* DarkSwarm           = new UnitPrototype("Dark swarm"                   , BW::UnitType::Spell_DarkSwarm,            RaceType::Other,     0, 0);    
                                                                                         
    UnitPrototype* None                = new UnitPrototype("None"                         , BW::UnitType::None,                       RaceType::Other,     0, 0);    
                                                                                        
                                                                                        
    UnitPrototype* unitIDToPrototypeTable[228] =                                        
    {                                                                                   
     /** 0x00 */ Marine,                                                                
     /** 0x01 */ Ghost,                                                                 
     /** 0x02 */ Vulture,
     /** 0x03 */ Goliath,
     /** 0x04 */ GoliathTurret,
     /** 0x05 */ SiegeTank_TankMode,
     /** 0x06 */ NULL, //TankTurretTankMode,
     /** 0x07 */ SCV,
     /** 0x08 */ Wraith,
     /** 0x09 */ ScienceVessel,
     /** 0x0A */ NULL, // Gui Montang (Firebat)
     /** 0x0B */ NULL, //DropShip,
     /** 0x0C */ BattleCruiser,
     /** 0x0D */ NULL, //VultureSpiderMine,
     /** 0x0E */ NuclearMissile,
     /** 0x0F */ NULL, //Terran Civilian
     /** 0x10 */ NULL, //Sarah Kerrigan (Ghost)
     /** 0x11 */ NULL,//  Alan Schezar (Goliath)
     /** 0x12 */ NULL,
     /** 0x13 */ NULL,
     /** 0x14 */ NULL,
     /** 0x15 */ NULL,
     /** 0x16 */ NULL,
     /** 0x17 */ NULL,
     /** 0x18 */ NULL,
     /** 0x19 */ NULL,
     /** 0x1A */ NULL,
     /** 0x1B */ NULL,
     /** 0x1C */ NULL,
     /** 0x1D */ NULL,
     /** 0x1E */ SiegeTank_SiegeMode,
     /** 0x1F */ NULL,
     /** 0x20 */ Firebat,
     /** 0x21 */ NULL,
     /** 0x22 */ Medic,
     /** 0x23 */ Larva,
     /** 0x24 */ Egg,
     /** 0x25 */ Zergling,
     /** 0x26 */ Hydralisk,
     /** 0x27 */ NULL,
     /** 0x28 */ NULL,
     /** 0x29 */ Drone,
     /** 0x2A */ Overlord,
     /** 0x2B */ Mutalisk,
     /** 0x2C */ Guardian,
     /** 0x2D */ Queen,
     /** 0x2E */ Defiler,
     /** 0x2F */ NULL,
     /** 0x30 */ NULL,
     /** 0x31 */ NULL,
     /** 0x32 */ NULL,
     /** 0x33 */ NULL,
     /** 0x34 */ NULL,
     /** 0x35 */ NULL,
     /** 0x36 */ NULL,
     /** 0x37 */ NULL,
     /** 0x38 */ NULL,
     /** 0x39 */ NULL,
     /** 0x3A */ Valkyrie,
     /** 0x3B */ NULL,
     /** 0x3C */ NULL,
     /** 0x3D */ DarkTemplar,
     /** 0x3E */ NULL,
     /** 0x3F */ NULL,
     /** 0x40 */ Probe,
     /** 0x41 */ Zealot,
     /** 0x42 */ Dragoon,
     /** 0x43 */ HighTemplar,
     /** 0x44 */ NULL,
     /** 0x45 */ Shuttle,
     /** 0x46 */ NULL,
     /** 0x47 */ NULL,
     /** 0x48 */ NULL,
     /** 0x49 */ NULL,
     /** 0x4A */ NULL,
     /** 0x4B */ NULL,
     /** 0x4C */ NULL,
     /** 0x4D */ NULL,
     /** 0x4E */ NULL,
     /** 0x4F */ NULL,
     /** 0x50 */ NULL,
     /** 0x51 */ NULL,
     /** 0x52 */ NULL,
     /** 0x53 */ Reaver,
     /** 0x54 */ Observer,
     /** 0x55 */ NULL,
     /** 0x56 */ NULL,
     /** 0x57 */ NULL,
     /** 0x58 */ NULL,
     /** 0x59 */ NULL,
     /** 0x5A */ Critter_Bengalaas,
     /** 0x5B */ NULL,
     /** 0x5C */ NULL,
     /** 0x5D */ Critter_Scantid,
     /** 0x5E */ Critter_Kakaru,
     /** 0x5F */ Critter_Ragnasaur,
     /** 0x60 */ NULL,
     /** 0x61 */ NULL,
     /** 0x62 */ NULL,
     /** 0x63 */ NULL,
     /** 0x64 */ NULL,
     /** 0x65 */ NULL,
     /** 0x66 */ NULL,
     /** 0x67 */ Lurker,
     /** 0x68 */ NULL,
     /** 0x69 */ NULL,
     /** 0x6A */ CommandCenter,
     /** 0x6B */ ComsatStation,
     /** 0x6C */ NuclearSilo,
     /** 0x6D */ SupplyDepot,
     /** 0x6E */ Refinery,
     /** 0x6F */ Barracks,
     /** 0x70 */ Academy,
     /** 0x71 */ Factory,
     /** 0x72 */ Starport,
     /** 0x73 */ ControlTower,
     /** 0x74 */ ScienceFacility,
     /** 0x75 */ CovertOps,
     /** 0x76 */ PhysicsLab,
     /** 0x77 */ NULL,
     /** 0x78 */ MachineShop,
     /** 0x79 */ NULL,
     /** 0x7A */ EngineeringBay,
     /** 0x7B */ Armory,
     /** 0x7C */ MissileTurret,
     /** 0x7D */ Bunker,
     /** 0x7E */ NULL,
     /** 0x7F */ NULL,
     /** 0x80 */ NULL,
     /** 0x81 */ NULL,
     /** 0x82 */ NULL,
     /** 0x83 */ Hatchery,
     /** 0x84 */ Lair,
     /** 0x85 */ Hive,
     /** 0x86 */ NULL,
     /** 0x87 */ HydraliskDen,
     /** 0x88 */ DefilerMound,
     /** 0x89 */ NULL,
     /** 0x8A */ QueensNest,
     /** 0x8B */ EvolutionChamber,
     /** 0x8C */ NULL,
     /** 0x8D */ Spire,
     /** 0x8E */ SpawningPool,
     /** 0x8F */ CreepColony,
     /** 0x90 */ SporeColony,
     /** 0x91 */ NULL,
     /** 0x92 */ SunkenColony,
     /** 0x93 */ NULL,
     /** 0x94 */ NULL,
     /** 0x95 */ Extractor,
     /** 0x96 */ NULL,
     /** 0x97 */ NULL,
     /** 0x98 */ NULL,
     /** 0x99 */ NULL,
     /** 0x9A */ Nexus,
     /** 0x9B */ RoboticsFacility,
     /** 0x9C */ Pylon,
     /** 0x9D */ Assimilator,
     /** 0x9E */ NULL,
     /** 0x9F */ NULL,
     /** 0xA0 */ Gateway,
     /** 0xA1 */ NULL,
     /** 0xA2 */ PhotonCannon,
     /** 0xA3 */ NULL,
     /** 0xA4 */ CyberneticsCore,
     /** 0xA5 */ TemplarArchives,
     /** 0xA6 */ Forge,
     /** 0xA7 */ NULL,
     /** 0xA8 */ NULL,
     /** 0xA9 */ NULL,
     /** 0xAA */ NULL,
     /** 0xAB */ RoboticsSupportBay,
     /** 0xAC */ NULL,
     /** 0xAD */ NULL,
     /** 0xAE */ NULL,
     /** 0xAF */ NULL,
     /** 0xB0 */ MineralPatch1,
     /** 0xB1 */ MineralPatch2,
     /** 0xB2 */ MineralPatch3,
     /** 0xB3 */ NULL,
     /** 0xB4 */ NULL,
     /** 0xB5 */ NULL,
     /** 0xB6 */ NULL,
     /** 0xB7 */ NULL,
     /** 0xB8 */ NULL,
     /** 0xB9 */ NULL,
     /** 0xBA */ NULL,
     /** 0xBB */ NULL,
     /** 0xBC */ VaspineGayser,
     /** 0xBD */ NULL,
     /** 0xBE */ NULL,
     /** 0xBF */ NULL,
     /** 0xC0 */ NULL,
     /** 0xC1 */ NULL,
     /** 0xC2 */ NULL,
     /** 0xC3 */ NULL,
     /** 0xC4 */ NULL,
     /** 0xC5 */ NULL,
     /** 0xC6 */ NULL,
     /** 0xC7 */ NULL,
     /** 0xC8 */ NULL,
     /** 0xC9 */ NULL,
     /** 0xCA */ DarkSwarm,
     /** 0xCB */ NULL,
     /** 0xCC */ NULL,
     /** 0xCD */ NULL,
     /** 0xCE */ NULL,
     /** 0xCF */ NULL,
     /** 0xD0 */ NULL,
     /** 0xD1 */ NULL,
     /** 0xD2 */ NULL,
     /** 0xD3 */ NULL,
     /** 0xD4 */ NULL,
     /** 0xD5 */ NULL,
     /** 0xD6 */ NULL,
     /** 0xD7 */ NULL,
     /** 0xD8 */ NULL,
     /** 0xD9 */ NULL,
     /** 0xDA */ NULL,
     /** 0xDB */ NULL,
     /** 0xDC */ NULL,
     /** 0xDD */ NULL,
     /** 0xDE */ NULL,
     /** 0xDF */ NULL,
     /** 0xE0 */ NULL,
     /** 0xE1 */ NULL,
     /** 0xE2 */ NULL,
     /** 0xE3 */ NULL
     };
  }
}

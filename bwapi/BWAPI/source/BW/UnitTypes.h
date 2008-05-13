#pragma once

#include "../Types.h"

namespace BW
{
  /// Enumeration of the unit types. 
  /// Most values from http://www.bwhacks.com/forums/starcraft-hacking-related/27282-unit-id-list.html
  /// Not all values (IE: hero values) are listed, since these are not needed in 
  /// normal games and I am lazy.
  namespace UnitType
  {
    enum Enum : u16 
    {
      // Terran Ground
      Terran_Marine                 = 0x00,
      Terran_Ghost                  = 0x01,
      Terran_Vulture                = 0x02,
      Terran_Goliath                = 0x03,
      Terran_GoliathTurret          = 0x04,
      Terran_SiegeTankTankMode      = 0x05,
      Terran_TankTurretTankMode     = 0x06,
      Terran_SCV                    = 0x07,
      Terran_JimRaynor_Hero         = 0x14,
      Terran_Firebat                = 0x20,
      Terran_Medic                  = 0x22,
      Terran_SiegeTankSiegeMode     = 0x1E,
      Terran_VultureSpiderMine      = 0x0D,
      Terran_NuclearMissile         = 0x0E,

      // Terran Air      
      Terran_Wraith                 = 0x08,
      Terran_ScienceVessel          = 0x09,
      Terran_DropShip               = 0x0B,
      Terran_BattleCruiser          = 0x0C,
      Terran_Valkyrie               = 0x3A,

      // Terran Buildings
      Terran_CommandCenter          = 0x6A,
      Terran_SupplyDepot            = 0x6D,
      Terran_Refinery               = 0x6E,
      Terran_Barracks               = 0x6F,
      Terran_Academy                = 0x70,
      Terran_Factory                = 0x71,
      Terran_Starport               = 0x72,
      Terran_ScienceFacility        = 0x74,
      Terran_EngineeringBay         = 0x7A,
      Terran_Armory                 = 0x7B,
      Terran_MissileTurret          = 0x7C,
      Terran_Bunker                 = 0x7D,

      //Terran Add-ons
      Terran_ComsatStation          = 0x6B,
      Terran_NuclearSilo            = 0x6C,
      Terran_ControlTower           = 0x73,
      Terran_CovertOps              = 0x75,
      Terran_PhysicsLab             = 0x76,
      Terran_MachineShop            = 0x78,

      //Protoss Ground
      Protoss_DarkTemplar           = 0x3D,
      Protoss_DarkArchon            = 0x3F,
      Protoss_Probe                 = 0x40,
      Protoss_Zealot                = 0x41,
      Protoss_Dragoon               = 0x42,
      Protoss_HighTemplar           = 0x43,
      Protoss_Archon                = 0x44,
      Protoss_Reaver                = 0x53,
      Protoss_Scarab                = 0x55,

      //Protoss Air
      Protoss_Corsair               = 0x3C,
      Protoss_Shuttle               = 0x45,
      Protoss_Scout                 = 0x46,
      Protoss_Arbiter               = 0x47,
      Protoss_Carrier               = 0x48,
      Protoss_Interceptor           = 0x49,
      Protoss_Observer              = 0x54,

      //Protoss Buildings
      Protoss_Nexus                 = 0x9A,
      Protoss_RoboticsFacility      = 0x9B,
      Protoss_Pylon                 = 0x9C,
      Protoss_Assimilator           = 0x9E,
      Protoss_Observatory           = 0x9F,
      Protoss_Gateway               = 0xA0,
      Protoss_PhotonCannon          = 0xA2,
      Protoss_CitadelOfAdun         = 0xA3,
      Protoss_CyberneticsCore       = 0xA4,
      Protoss_TemplarArchives       = 0xA5,
      Protoss_Forge                 = 0xA6,
      Protoss_Stargate              = 0xA7,
      Protoss_FleetBeacon           = 0xA9,
      Protoss_ArbiterTribunal       = 0xAA,
      Protoss_RoboticsSupportBay    = 0xAB,
      Protoss_ShieldBattery         = 0xAC,

      //Zerg Ground
      Zerg_Larva                    = 0x23,
      Zerg_Egg                      = 0x24,
      Zerg_LurkerEgg                = 0x61,
      Zerg_Zergling                 = 0x25,
      Zerg_Hydralisk                = 0x26,
      Zerg_Ultralisk                = 0x27,
      Zerg_Broodling                = 0x28,
      Zerg_Drone                    = 0x29,
      Zerg_Defiler                  = 0x2E,
      Zerg_InfestedTerran           = 0x32,
      Zerg_Lurker                   = 0x67,

      //Zerg_Air
      Zerg_Overlord                 = 0x2A,
      Zerg_Mutalisk                 = 0x2B,
      Zerg_Guardian                 = 0x2C,
      Zerg_Devourer                 = 0x3E,
      Zerg_Queen                    = 0x2D,
      Zerg_Scourge                  = 0x2F,
      Zerg_DevourerGuardianCocoon   = 0x3B,

      //Zerg_Buildings
      Zerg_InfestedCommandCenter    = 0x82,
      Zerg_Hatchery                 = 0x83,
      Zerg_Lair                     = 0x84,
      Zerg_Hive                     = 0x85,
      Zerg_NydusCanal               = 0x86,
      Zerg_HydraliskDen             = 0x87,
      Zerg_DefilerMound             = 0x88,
      Zerg_GreaterSpire             = 0x89,
      Zerg_QueensNest               = 0x8A,
      Zerg_EvolutionChamber         = 0x8B,
      Zerg_UltraliskCavern          = 0x8C,
      Zerg_Spire                    = 0x8D,
      Zerg_SpawningPool             = 0x8E,
      Zerg_CreepColony              = 0x8F,
      Zerg_SporeColony              = 0x90,
      Zerg_SunkenColony             = 0x92,
      Zerg_Extractor                = 0x95,

      //Resources
      Resource_MineralPatch1        = 0xB0,
      Resource_MineralPatch2        = 0xB1,
      Resource_MineralPatch3        = 0xB2,
      Resource_VespeneGeyser        = 0xBC,

      //Beacons
      Beacon_Zerg                   = 0xC2,
      Beacon_Terran                 = 0xC3,
      Beacon_Protoss                = 0xC4,
      Beacon_ZergFlag               = 0xC5,
      Beacon_TerranFlag             = 0xC6,
      Beacon_ProtossFlag            = 0xC7,

      //Critters
      Critter_Rhynadon              = 0x59,
      Critter_Bengalaas             = 0x5A,
      Critter_Scantid               = 0x5D,
      Critter_Kakaru                = 0x5E,
      Critter_Ragnasaur             = 0x5F,
      Critter_Ursadon               = 0x60,

      //Spells
      Spell_ScannerSweep            = 0x21,
      Spell_DisruptionWeb           = 0x69,
      Spell_DarkSwarm               = 0xCA,
      //Others
      Mineral_Chunk1                = 0xDC,
      Mineral_Chunk2                = 0xDD,
      Vespene_Orb1                  = 0xDE,
      Vespene_Orb2                  = 0xDF,
      Vaspene_Sac1                  = 0xE0,
      Vaspene_Sac2                  = 0xE1,
      None                          = 0XE4
    };
  };
};

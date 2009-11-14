#pragma once
#include <string>
#include <map>
#include <set>
#include <BWAPI/Race.h>
#include <BWAPI/UnitSizeType.h>
namespace BWAPI
{
  class TechType;
  class UpgradeType;
  class WeaponType;
  class UnitType
  {
    public:
      UnitType();
      UnitType(int id);
      UnitType(const UnitType& other);
      UnitType& __cdecl operator=(const UnitType& other);
      bool __cdecl operator==(const UnitType& other) const;
      bool __cdecl operator!=(const UnitType& other) const;
      bool __cdecl operator<(const UnitType& other) const;
      int __cdecl getID() const;
      std::string __cdecl getName() const;
      std::string __cdecl getSubLabel() const;
      Race __cdecl getRace() const;

      const std::pair< const UnitType*, int > __cdecl whatBuilds() const;
      const std::map< const UnitType*, int >& __cdecl requiredUnits() const;
      const TechType* __cdecl requiredTech() const;
      const std::set< const TechType* >& __cdecl abilities() const;
      const UpgradeType* __cdecl armorUpgrade() const;

      int __cdecl maxHitPoints() const;
      int __cdecl maxShields() const;
      int __cdecl maxEnergy() const;
      int __cdecl armor() const;

      int __cdecl mineralPrice() const;
      int __cdecl gasPrice() const;
      int __cdecl buildTime() const;

      int __cdecl supplyRequired() const;
      int __cdecl supplyProvided() const;
      int __cdecl spaceRequired() const;
      int __cdecl spaceProvided() const;
      int __cdecl buildScore() const;
      int __cdecl destroyScore() const;

      UnitSizeType __cdecl size() const;
      int __cdecl tileWidth() const;
      int __cdecl tileHeight() const;
      int __cdecl dimensionLeft() const;
      int __cdecl dimensionUp() const;
      int __cdecl dimensionRight() const;
      int __cdecl dimensionDown() const;

      int __cdecl seekRange() const;
      int __cdecl sightRange() const;
      const WeaponType* __cdecl groundWeapon() const;
      int __cdecl maxGroundHits() const;
      const WeaponType* __cdecl airWeapon() const;
      int __cdecl maxAirHits() const;

      int __cdecl topSpeed() const;
      int __cdecl acceleration() const;
      int __cdecl haltDistance() const;
      int __cdecl turnRadius() const;

      bool __cdecl canProduce() const;
      bool __cdecl canAttack() const;
      bool __cdecl canMove() const;

      bool __cdecl isFlyer() const;
      bool __cdecl regeneratesHP() const;
      bool __cdecl isSpellcaster() const;
      bool __cdecl hasPermanentCloak() const;
      bool __cdecl isInvincible() const;
      bool __cdecl isOrganic() const;
      bool __cdecl isMechanical() const;
      bool __cdecl isRobotic() const;
      bool __cdecl isDetector() const;
      bool __cdecl isResourceContainer() const;
      bool __cdecl isResourceDepot() const;
      bool __cdecl isRefinery() const;
      bool __cdecl isWorker() const;
      bool __cdecl requiresPsi() const;
      bool __cdecl requiresCreep() const;
      bool __cdecl isTwoUnitsInOneEgg() const;
      bool __cdecl isBurrowable() const;
      bool __cdecl isCloakable() const;
      bool __cdecl isBuilding() const;
      bool __cdecl isAddon() const;
      bool __cdecl isFlyingBuilding() const;
      bool __cdecl isNeutral() const;
    private:
      int id;
  };
  namespace UnitTypes
  {
    UnitType __cdecl getUnitType(std::string& name);
    std::set<UnitType>& __cdecl allUnitTypes();
    void __cdecl init();
    extern const UnitType Terran_Marine;
    extern const UnitType Terran_Ghost;
    extern const UnitType Terran_Vulture;
    extern const UnitType Terran_Goliath;
    extern const UnitType Terran_Siege_Tank_Tank_Mode;
    extern const UnitType Terran_SCV;
    extern const UnitType Terran_Wraith;
    extern const UnitType Terran_Science_Vessel;
    extern const UnitType Terran_Dropship;
    extern const UnitType Terran_Battlecruiser;
    extern const UnitType Terran_Vulture_Spider_Mine;
    extern const UnitType Terran_Nuclear_Missile;
    extern const UnitType Terran_Siege_Tank_Siege_Mode;
    extern const UnitType Terran_Firebat;
    extern const UnitType Spell_Scanner_Sweep;
    extern const UnitType Terran_Medic;
    extern const UnitType Zerg_Larva;
    extern const UnitType Zerg_Egg;
    extern const UnitType Zerg_Zergling;
    extern const UnitType Zerg_Hydralisk;
    extern const UnitType Zerg_Ultralisk;
    extern const UnitType Zerg_Broodling;
    extern const UnitType Zerg_Drone;
    extern const UnitType Zerg_Overlord;
    extern const UnitType Zerg_Mutalisk;
    extern const UnitType Zerg_Guardian;
    extern const UnitType Zerg_Queen;
    extern const UnitType Zerg_Defiler;
    extern const UnitType Zerg_Scourge;
    extern const UnitType Zerg_Infested_Terran;
    extern const UnitType Terran_Valkyrie;
    extern const UnitType Zerg_Cocoon;
    extern const UnitType Protoss_Corsair;
    extern const UnitType Protoss_Dark_Templar;
    extern const UnitType Zerg_Devourer;
    extern const UnitType Protoss_Dark_Archon;
    extern const UnitType Protoss_Probe;
    extern const UnitType Protoss_Zealot;
    extern const UnitType Protoss_Dragoon;
    extern const UnitType Protoss_High_Templar;
    extern const UnitType Protoss_Archon;
    extern const UnitType Protoss_Shuttle;
    extern const UnitType Protoss_Scout;
    extern const UnitType Protoss_Arbiter;
    extern const UnitType Protoss_Carrier;
    extern const UnitType Protoss_Interceptor;
    extern const UnitType Protoss_Reaver;
    extern const UnitType Protoss_Observer;
    extern const UnitType Protoss_Scarab;
    extern const UnitType Critter_Rhynadon;
    extern const UnitType Critter_Bengalaas;
    extern const UnitType Critter_Scantid;
    extern const UnitType Critter_Kakaru;
    extern const UnitType Critter_Ragnasaur;
    extern const UnitType Critter_Ursadon;
    extern const UnitType Zerg_Lurker_Egg;
    extern const UnitType Zerg_Lurker;
    extern const UnitType Spell_Disruption_Web;
    extern const UnitType Terran_Command_Center;
    extern const UnitType Terran_Comsat_Station;
    extern const UnitType Terran_Nuclear_Silo;
    extern const UnitType Terran_Supply_Depot;
    extern const UnitType Terran_Refinery;
    extern const UnitType Terran_Barracks;
    extern const UnitType Terran_Academy;
    extern const UnitType Terran_Factory;
    extern const UnitType Terran_Starport;
    extern const UnitType Terran_Control_Tower;
    extern const UnitType Terran_Science_Facility;
    extern const UnitType Terran_Covert_Ops;
    extern const UnitType Terran_Physics_Lab;
    extern const UnitType Terran_Machine_Shop;
    extern const UnitType Terran_Engineering_Bay;
    extern const UnitType Terran_Armory;
    extern const UnitType Terran_Missile_Turret;
    extern const UnitType Terran_Bunker;
    extern const UnitType Special_Crashed_Norad_II;
    extern const UnitType Special_Ion_Cannon;
    extern const UnitType Zerg_Infested_Command_Center;
    extern const UnitType Zerg_Hatchery;
    extern const UnitType Zerg_Lair;
    extern const UnitType Zerg_Hive;
    extern const UnitType Zerg_Nydus_Canal;
    extern const UnitType Zerg_Hydralisk_Den;
    extern const UnitType Zerg_Defiler_Mound;
    extern const UnitType Zerg_Greater_Spire;
    extern const UnitType Zerg_Queens_Nest;
    extern const UnitType Zerg_Evolution_Chamber;
    extern const UnitType Zerg_Ultralisk_Cavern;
    extern const UnitType Zerg_Spire;
    extern const UnitType Zerg_Spawning_Pool;
    extern const UnitType Zerg_Creep_Colony;
    extern const UnitType Zerg_Spore_Colony;
    extern const UnitType Zerg_Sunken_Colony;
    extern const UnitType Special_Overmind_With_Shell;
    extern const UnitType Special_Overmind;
    extern const UnitType Zerg_Extractor;
    extern const UnitType Special_Mature_Chrysalis;
    extern const UnitType Special_Cerebrate;
    extern const UnitType Special_Cerebrate_Daggoth;
    extern const UnitType Protoss_Nexus;
    extern const UnitType Protoss_Robotics_Facility;
    extern const UnitType Protoss_Pylon;
    extern const UnitType Protoss_Assimilator;
    extern const UnitType Protoss_Observatory;
    extern const UnitType Protoss_Gateway;
    extern const UnitType Protoss_Photon_Cannon;
    extern const UnitType Protoss_Citadel_of_Adun;
    extern const UnitType Protoss_Cybernetics_Core;
    extern const UnitType Protoss_Templar_Archives;
    extern const UnitType Protoss_Forge;
    extern const UnitType Protoss_Stargate;
    extern const UnitType Special_Stasis_Cell_Prison;
    extern const UnitType Protoss_Fleet_Beacon;
    extern const UnitType Protoss_Arbiter_Tribunal;
    extern const UnitType Protoss_Robotics_Support_Bay;
    extern const UnitType Protoss_Shield_Battery;
    extern const UnitType Special_Khaydarin_Crystal_Form;
    extern const UnitType Special_Protoss_Temple;
    extern const UnitType Special_XelNaga_Temple;
    extern const UnitType Resource_Mineral_Field;
    extern const UnitType Resource_Vespene_Geyser;
    extern const UnitType Special_Warp_Gate;
    extern const UnitType Special_Psi_Disrupter;
    extern const UnitType Special_Power_Generator;
    extern const UnitType Special_Overmind_Cocoon;
    extern const UnitType Spell_Dark_Swarm;
    extern const UnitType None;
    extern const UnitType Unknown;

  }
}

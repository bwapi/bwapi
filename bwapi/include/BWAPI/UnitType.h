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
      UnitType& operator=(const UnitType& other);
      bool operator==(const UnitType& other) const;
      bool operator!=(const UnitType& other) const;
      bool operator<(const UnitType& other) const;
      int getID() const;
      std::string getName() const;
      std::string getSubLabel() const;
      Race getRace() const;

      const std::pair< const UnitType*, int > whatBuilds() const;
      const std::map< const UnitType*, int >& requiredUnits() const;
      const TechType* requiredTech() const;
      const std::set< const TechType* >& abilities() const;
      const std::set< const UpgradeType* >& upgrades() const;
      const UpgradeType* armorUpgrade() const;

      int maxHitPoints() const;
      int maxShields() const;
      int maxEnergy() const;
      int armor() const;

      int mineralPrice() const;
      int gasPrice() const;
      int buildTime() const;

      int supplyRequired() const;
      int supplyProvided() const;
      int spaceRequired() const;
      int spaceProvided() const;
      int buildScore() const;
      int destroyScore() const;

      UnitSizeType size() const;
      int tileWidth() const;
      int tileHeight() const;
      int dimensionLeft() const;
      int dimensionUp() const;
      int dimensionRight() const;
      int dimensionDown() const;

      int seekRange() const;
      int sightRange() const;
      const WeaponType* groundWeapon() const;
      int maxGroundHits() const;
      const WeaponType* airWeapon() const;
      int maxAirHits() const;

      double topSpeed() const;
      int acceleration() const;
      int haltDistance() const;
      int turnRadius() const;

      bool canProduce() const;
      bool canAttack() const;
      bool canMove() const;

      bool isFlyer() const;
      bool regeneratesHP() const;
      bool isSpellcaster() const;
      bool hasPermanentCloak() const;
      bool isInvincible() const;
      bool isOrganic() const;
      bool isMechanical() const;
      bool isRobotic() const;
      bool isDetector() const;
      bool isResourceContainer() const;
      bool isResourceDepot() const;
      bool isRefinery() const;
      bool isWorker() const;
      bool requiresPsi() const;
      bool requiresCreep() const;
      bool isTwoUnitsInOneEgg() const;
      bool isBurrowable() const;
      bool isCloakable() const;
      bool isBuilding() const;
      bool isAddon() const;
      bool isFlyingBuilding() const;
      bool isNeutral() const;
    private:
      int id;
  };
  namespace UnitTypes
  {
    UnitType getUnitType(std::string& name);
    std::set<UnitType>& allUnitTypes();
    void init();
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

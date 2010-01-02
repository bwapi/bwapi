#pragma once
#include "BWAPI2Interface.h"
#include "Race.h"
#include "UnitSizeType.h"

#include <string>
#include <map>
#include <set>
namespace BWAPI2
{
  class TechType;
  class UpgradeType;
  class WeaponType;
  class BWAPI2_EXPORT UnitType
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
    BWAPI2_EXPORT extern const UnitType Terran_Marine;
    BWAPI2_EXPORT extern const UnitType Terran_Ghost;
    BWAPI2_EXPORT extern const UnitType Terran_Vulture;
    BWAPI2_EXPORT extern const UnitType Terran_Goliath;
    BWAPI2_EXPORT extern const UnitType Terran_Siege_Tank_Tank_Mode;
    BWAPI2_EXPORT extern const UnitType Terran_SCV;
    BWAPI2_EXPORT extern const UnitType Terran_Wraith;
    BWAPI2_EXPORT extern const UnitType Terran_Science_Vessel;
    BWAPI2_EXPORT extern const UnitType Terran_Dropship;
    BWAPI2_EXPORT extern const UnitType Terran_Battlecruiser;
    BWAPI2_EXPORT extern const UnitType Terran_Vulture_Spider_Mine;
    BWAPI2_EXPORT extern const UnitType Terran_Nuclear_Missile;
    BWAPI2_EXPORT extern const UnitType Terran_Siege_Tank_Siege_Mode;
    BWAPI2_EXPORT extern const UnitType Terran_Firebat;
    BWAPI2_EXPORT extern const UnitType Spell_Scanner_Sweep;
    BWAPI2_EXPORT extern const UnitType Terran_Medic;
    BWAPI2_EXPORT extern const UnitType Zerg_Larva;
    BWAPI2_EXPORT extern const UnitType Zerg_Egg;
    BWAPI2_EXPORT extern const UnitType Zerg_Zergling;
    BWAPI2_EXPORT extern const UnitType Zerg_Hydralisk;
    BWAPI2_EXPORT extern const UnitType Zerg_Ultralisk;
    BWAPI2_EXPORT extern const UnitType Zerg_Broodling;
    BWAPI2_EXPORT extern const UnitType Zerg_Drone;
    BWAPI2_EXPORT extern const UnitType Zerg_Overlord;
    BWAPI2_EXPORT extern const UnitType Zerg_Mutalisk;
    BWAPI2_EXPORT extern const UnitType Zerg_Guardian;
    BWAPI2_EXPORT extern const UnitType Zerg_Queen;
    BWAPI2_EXPORT extern const UnitType Zerg_Defiler;
    BWAPI2_EXPORT extern const UnitType Zerg_Scourge;
    BWAPI2_EXPORT extern const UnitType Zerg_Infested_Terran;
    BWAPI2_EXPORT extern const UnitType Terran_Valkyrie;
    BWAPI2_EXPORT extern const UnitType Zerg_Cocoon;
    BWAPI2_EXPORT extern const UnitType Protoss_Corsair;
    BWAPI2_EXPORT extern const UnitType Protoss_Dark_Templar;
    BWAPI2_EXPORT extern const UnitType Zerg_Devourer;
    BWAPI2_EXPORT extern const UnitType Protoss_Dark_Archon;
    BWAPI2_EXPORT extern const UnitType Protoss_Probe;
    BWAPI2_EXPORT extern const UnitType Protoss_Zealot;
    BWAPI2_EXPORT extern const UnitType Protoss_Dragoon;
    BWAPI2_EXPORT extern const UnitType Protoss_High_Templar;
    BWAPI2_EXPORT extern const UnitType Protoss_Archon;
    BWAPI2_EXPORT extern const UnitType Protoss_Shuttle;
    BWAPI2_EXPORT extern const UnitType Protoss_Scout;
    BWAPI2_EXPORT extern const UnitType Protoss_Arbiter;
    BWAPI2_EXPORT extern const UnitType Protoss_Carrier;
    BWAPI2_EXPORT extern const UnitType Protoss_Interceptor;
    BWAPI2_EXPORT extern const UnitType Protoss_Reaver;
    BWAPI2_EXPORT extern const UnitType Protoss_Observer;
    BWAPI2_EXPORT extern const UnitType Protoss_Scarab;
    BWAPI2_EXPORT extern const UnitType Critter_Rhynadon;
    BWAPI2_EXPORT extern const UnitType Critter_Bengalaas;
    BWAPI2_EXPORT extern const UnitType Critter_Scantid;
    BWAPI2_EXPORT extern const UnitType Critter_Kakaru;
    BWAPI2_EXPORT extern const UnitType Critter_Ragnasaur;
    BWAPI2_EXPORT extern const UnitType Critter_Ursadon;
    BWAPI2_EXPORT extern const UnitType Zerg_Lurker_Egg;
    BWAPI2_EXPORT extern const UnitType Zerg_Lurker;
    BWAPI2_EXPORT extern const UnitType Spell_Disruption_Web;
    BWAPI2_EXPORT extern const UnitType Terran_Command_Center;
    BWAPI2_EXPORT extern const UnitType Terran_Comsat_Station;
    BWAPI2_EXPORT extern const UnitType Terran_Nuclear_Silo;
    BWAPI2_EXPORT extern const UnitType Terran_Supply_Depot;
    BWAPI2_EXPORT extern const UnitType Terran_Refinery;
    BWAPI2_EXPORT extern const UnitType Terran_Barracks;
    BWAPI2_EXPORT extern const UnitType Terran_Academy;
    BWAPI2_EXPORT extern const UnitType Terran_Factory;
    BWAPI2_EXPORT extern const UnitType Terran_Starport;
    BWAPI2_EXPORT extern const UnitType Terran_Control_Tower;
    BWAPI2_EXPORT extern const UnitType Terran_Science_Facility;
    BWAPI2_EXPORT extern const UnitType Terran_Covert_Ops;
    BWAPI2_EXPORT extern const UnitType Terran_Physics_Lab;
    BWAPI2_EXPORT extern const UnitType Terran_Machine_Shop;
    BWAPI2_EXPORT extern const UnitType Terran_Engineering_Bay;
    BWAPI2_EXPORT extern const UnitType Terran_Armory;
    BWAPI2_EXPORT extern const UnitType Terran_Missile_Turret;
    BWAPI2_EXPORT extern const UnitType Terran_Bunker;
    BWAPI2_EXPORT extern const UnitType Special_Crashed_Norad_II;
    BWAPI2_EXPORT extern const UnitType Special_Ion_Cannon;
    BWAPI2_EXPORT extern const UnitType Zerg_Infested_Command_Center;
    BWAPI2_EXPORT extern const UnitType Zerg_Hatchery;
    BWAPI2_EXPORT extern const UnitType Zerg_Lair;
    BWAPI2_EXPORT extern const UnitType Zerg_Hive;
    BWAPI2_EXPORT extern const UnitType Zerg_Nydus_Canal;
    BWAPI2_EXPORT extern const UnitType Zerg_Hydralisk_Den;
    BWAPI2_EXPORT extern const UnitType Zerg_Defiler_Mound;
    BWAPI2_EXPORT extern const UnitType Zerg_Greater_Spire;
    BWAPI2_EXPORT extern const UnitType Zerg_Queens_Nest;
    BWAPI2_EXPORT extern const UnitType Zerg_Evolution_Chamber;
    BWAPI2_EXPORT extern const UnitType Zerg_Ultralisk_Cavern;
    BWAPI2_EXPORT extern const UnitType Zerg_Spire;
    BWAPI2_EXPORT extern const UnitType Zerg_Spawning_Pool;
    BWAPI2_EXPORT extern const UnitType Zerg_Creep_Colony;
    BWAPI2_EXPORT extern const UnitType Zerg_Spore_Colony;
    BWAPI2_EXPORT extern const UnitType Zerg_Sunken_Colony;
    BWAPI2_EXPORT extern const UnitType Special_Overmind_With_Shell;
    BWAPI2_EXPORT extern const UnitType Special_Overmind;
    BWAPI2_EXPORT extern const UnitType Zerg_Extractor;
    BWAPI2_EXPORT extern const UnitType Special_Mature_Chrysalis;
    BWAPI2_EXPORT extern const UnitType Special_Cerebrate;
    BWAPI2_EXPORT extern const UnitType Special_Cerebrate_Daggoth;
    BWAPI2_EXPORT extern const UnitType Protoss_Nexus;
    BWAPI2_EXPORT extern const UnitType Protoss_Robotics_Facility;
    BWAPI2_EXPORT extern const UnitType Protoss_Pylon;
    BWAPI2_EXPORT extern const UnitType Protoss_Assimilator;
    BWAPI2_EXPORT extern const UnitType Protoss_Observatory;
    BWAPI2_EXPORT extern const UnitType Protoss_Gateway;
    BWAPI2_EXPORT extern const UnitType Protoss_Photon_Cannon;
    BWAPI2_EXPORT extern const UnitType Protoss_Citadel_of_Adun;
    BWAPI2_EXPORT extern const UnitType Protoss_Cybernetics_Core;
    BWAPI2_EXPORT extern const UnitType Protoss_Templar_Archives;
    BWAPI2_EXPORT extern const UnitType Protoss_Forge;
    BWAPI2_EXPORT extern const UnitType Protoss_Stargate;
    BWAPI2_EXPORT extern const UnitType Special_Stasis_Cell_Prison;
    BWAPI2_EXPORT extern const UnitType Protoss_Fleet_Beacon;
    BWAPI2_EXPORT extern const UnitType Protoss_Arbiter_Tribunal;
    BWAPI2_EXPORT extern const UnitType Protoss_Robotics_Support_Bay;
    BWAPI2_EXPORT extern const UnitType Protoss_Shield_Battery;
    BWAPI2_EXPORT extern const UnitType Special_Khaydarin_Crystal_Form;
    BWAPI2_EXPORT extern const UnitType Special_Protoss_Temple;
    BWAPI2_EXPORT extern const UnitType Special_XelNaga_Temple;
    BWAPI2_EXPORT extern const UnitType Resource_Mineral_Field;
    BWAPI2_EXPORT extern const UnitType Resource_Vespene_Geyser;
    BWAPI2_EXPORT extern const UnitType Special_Warp_Gate;
    BWAPI2_EXPORT extern const UnitType Special_Psi_Disrupter;
    BWAPI2_EXPORT extern const UnitType Special_Power_Generator;
    BWAPI2_EXPORT extern const UnitType Special_Overmind_Cocoon;
    BWAPI2_EXPORT extern const UnitType Spell_Dark_Swarm;
    BWAPI2_EXPORT extern const UnitType None;
    BWAPI2_EXPORT extern const UnitType Unknown;

  }
}

#pragma once
#include "BWSLInterface.h"
#include "Race.h"
#include "UnitSizeType.h"

#include <string>
#include <map>
#include <set>
namespace BWSL
{
  class TechType;
  class UpgradeType;
  class WeaponType;
  class BWSL_EXPORT UnitType
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
    BWSL_EXPORT UnitType getUnitType(std::string& name);
    BWSL_EXPORT std::set<UnitType>& allUnitTypes();
    void init();
    BWSL_EXPORT extern const UnitType Terran_Marine;
    BWSL_EXPORT extern const UnitType Terran_Ghost;
    BWSL_EXPORT extern const UnitType Terran_Vulture;
    BWSL_EXPORT extern const UnitType Terran_Goliath;
    BWSL_EXPORT extern const UnitType Terran_Siege_Tank_Tank_Mode;
    BWSL_EXPORT extern const UnitType Terran_SCV;
    BWSL_EXPORT extern const UnitType Terran_Wraith;
    BWSL_EXPORT extern const UnitType Terran_Science_Vessel;
    BWSL_EXPORT extern const UnitType Terran_Dropship;
    BWSL_EXPORT extern const UnitType Terran_Battlecruiser;
    BWSL_EXPORT extern const UnitType Terran_Vulture_Spider_Mine;
    BWSL_EXPORT extern const UnitType Terran_Nuclear_Missile;
    BWSL_EXPORT extern const UnitType Terran_Siege_Tank_Siege_Mode;
    BWSL_EXPORT extern const UnitType Terran_Firebat;
    BWSL_EXPORT extern const UnitType Spell_Scanner_Sweep;
    BWSL_EXPORT extern const UnitType Terran_Medic;
    BWSL_EXPORT extern const UnitType Zerg_Larva;
    BWSL_EXPORT extern const UnitType Zerg_Egg;
    BWSL_EXPORT extern const UnitType Zerg_Zergling;
    BWSL_EXPORT extern const UnitType Zerg_Hydralisk;
    BWSL_EXPORT extern const UnitType Zerg_Ultralisk;
    BWSL_EXPORT extern const UnitType Zerg_Broodling;
    BWSL_EXPORT extern const UnitType Zerg_Drone;
    BWSL_EXPORT extern const UnitType Zerg_Overlord;
    BWSL_EXPORT extern const UnitType Zerg_Mutalisk;
    BWSL_EXPORT extern const UnitType Zerg_Guardian;
    BWSL_EXPORT extern const UnitType Zerg_Queen;
    BWSL_EXPORT extern const UnitType Zerg_Defiler;
    BWSL_EXPORT extern const UnitType Zerg_Scourge;
    BWSL_EXPORT extern const UnitType Zerg_Infested_Terran;
    BWSL_EXPORT extern const UnitType Terran_Valkyrie;
    BWSL_EXPORT extern const UnitType Zerg_Cocoon;
    BWSL_EXPORT extern const UnitType Protoss_Corsair;
    BWSL_EXPORT extern const UnitType Protoss_Dark_Templar;
    BWSL_EXPORT extern const UnitType Zerg_Devourer;
    BWSL_EXPORT extern const UnitType Protoss_Dark_Archon;
    BWSL_EXPORT extern const UnitType Protoss_Probe;
    BWSL_EXPORT extern const UnitType Protoss_Zealot;
    BWSL_EXPORT extern const UnitType Protoss_Dragoon;
    BWSL_EXPORT extern const UnitType Protoss_High_Templar;
    BWSL_EXPORT extern const UnitType Protoss_Archon;
    BWSL_EXPORT extern const UnitType Protoss_Shuttle;
    BWSL_EXPORT extern const UnitType Protoss_Scout;
    BWSL_EXPORT extern const UnitType Protoss_Arbiter;
    BWSL_EXPORT extern const UnitType Protoss_Carrier;
    BWSL_EXPORT extern const UnitType Protoss_Interceptor;
    BWSL_EXPORT extern const UnitType Protoss_Reaver;
    BWSL_EXPORT extern const UnitType Protoss_Observer;
    BWSL_EXPORT extern const UnitType Protoss_Scarab;
    BWSL_EXPORT extern const UnitType Critter_Rhynadon;
    BWSL_EXPORT extern const UnitType Critter_Bengalaas;
    BWSL_EXPORT extern const UnitType Critter_Scantid;
    BWSL_EXPORT extern const UnitType Critter_Kakaru;
    BWSL_EXPORT extern const UnitType Critter_Ragnasaur;
    BWSL_EXPORT extern const UnitType Critter_Ursadon;
    BWSL_EXPORT extern const UnitType Zerg_Lurker_Egg;
    BWSL_EXPORT extern const UnitType Zerg_Lurker;
    BWSL_EXPORT extern const UnitType Spell_Disruption_Web;
    BWSL_EXPORT extern const UnitType Terran_Command_Center;
    BWSL_EXPORT extern const UnitType Terran_Comsat_Station;
    BWSL_EXPORT extern const UnitType Terran_Nuclear_Silo;
    BWSL_EXPORT extern const UnitType Terran_Supply_Depot;
    BWSL_EXPORT extern const UnitType Terran_Refinery;
    BWSL_EXPORT extern const UnitType Terran_Barracks;
    BWSL_EXPORT extern const UnitType Terran_Academy;
    BWSL_EXPORT extern const UnitType Terran_Factory;
    BWSL_EXPORT extern const UnitType Terran_Starport;
    BWSL_EXPORT extern const UnitType Terran_Control_Tower;
    BWSL_EXPORT extern const UnitType Terran_Science_Facility;
    BWSL_EXPORT extern const UnitType Terran_Covert_Ops;
    BWSL_EXPORT extern const UnitType Terran_Physics_Lab;
    BWSL_EXPORT extern const UnitType Terran_Machine_Shop;
    BWSL_EXPORT extern const UnitType Terran_Engineering_Bay;
    BWSL_EXPORT extern const UnitType Terran_Armory;
    BWSL_EXPORT extern const UnitType Terran_Missile_Turret;
    BWSL_EXPORT extern const UnitType Terran_Bunker;
    BWSL_EXPORT extern const UnitType Special_Crashed_Norad_II;
    BWSL_EXPORT extern const UnitType Special_Ion_Cannon;
    BWSL_EXPORT extern const UnitType Zerg_Infested_Command_Center;
    BWSL_EXPORT extern const UnitType Zerg_Hatchery;
    BWSL_EXPORT extern const UnitType Zerg_Lair;
    BWSL_EXPORT extern const UnitType Zerg_Hive;
    BWSL_EXPORT extern const UnitType Zerg_Nydus_Canal;
    BWSL_EXPORT extern const UnitType Zerg_Hydralisk_Den;
    BWSL_EXPORT extern const UnitType Zerg_Defiler_Mound;
    BWSL_EXPORT extern const UnitType Zerg_Greater_Spire;
    BWSL_EXPORT extern const UnitType Zerg_Queens_Nest;
    BWSL_EXPORT extern const UnitType Zerg_Evolution_Chamber;
    BWSL_EXPORT extern const UnitType Zerg_Ultralisk_Cavern;
    BWSL_EXPORT extern const UnitType Zerg_Spire;
    BWSL_EXPORT extern const UnitType Zerg_Spawning_Pool;
    BWSL_EXPORT extern const UnitType Zerg_Creep_Colony;
    BWSL_EXPORT extern const UnitType Zerg_Spore_Colony;
    BWSL_EXPORT extern const UnitType Zerg_Sunken_Colony;
    BWSL_EXPORT extern const UnitType Special_Overmind_With_Shell;
    BWSL_EXPORT extern const UnitType Special_Overmind;
    BWSL_EXPORT extern const UnitType Zerg_Extractor;
    BWSL_EXPORT extern const UnitType Special_Mature_Chrysalis;
    BWSL_EXPORT extern const UnitType Special_Cerebrate;
    BWSL_EXPORT extern const UnitType Special_Cerebrate_Daggoth;
    BWSL_EXPORT extern const UnitType Protoss_Nexus;
    BWSL_EXPORT extern const UnitType Protoss_Robotics_Facility;
    BWSL_EXPORT extern const UnitType Protoss_Pylon;
    BWSL_EXPORT extern const UnitType Protoss_Assimilator;
    BWSL_EXPORT extern const UnitType Protoss_Observatory;
    BWSL_EXPORT extern const UnitType Protoss_Gateway;
    BWSL_EXPORT extern const UnitType Protoss_Photon_Cannon;
    BWSL_EXPORT extern const UnitType Protoss_Citadel_of_Adun;
    BWSL_EXPORT extern const UnitType Protoss_Cybernetics_Core;
    BWSL_EXPORT extern const UnitType Protoss_Templar_Archives;
    BWSL_EXPORT extern const UnitType Protoss_Forge;
    BWSL_EXPORT extern const UnitType Protoss_Stargate;
    BWSL_EXPORT extern const UnitType Special_Stasis_Cell_Prison;
    BWSL_EXPORT extern const UnitType Protoss_Fleet_Beacon;
    BWSL_EXPORT extern const UnitType Protoss_Arbiter_Tribunal;
    BWSL_EXPORT extern const UnitType Protoss_Robotics_Support_Bay;
    BWSL_EXPORT extern const UnitType Protoss_Shield_Battery;
    BWSL_EXPORT extern const UnitType Special_Khaydarin_Crystal_Form;
    BWSL_EXPORT extern const UnitType Special_Protoss_Temple;
    BWSL_EXPORT extern const UnitType Special_XelNaga_Temple;
    BWSL_EXPORT extern const UnitType Resource_Mineral_Field;
    BWSL_EXPORT extern const UnitType Resource_Vespene_Geyser;
    BWSL_EXPORT extern const UnitType Special_Warp_Gate;
    BWSL_EXPORT extern const UnitType Special_Psi_Disrupter;
    BWSL_EXPORT extern const UnitType Special_Power_Generator;
    BWSL_EXPORT extern const UnitType Special_Overmind_Cocoon;
    BWSL_EXPORT extern const UnitType Spell_Dark_Swarm;
    BWSL_EXPORT extern const UnitType None;
    BWSL_EXPORT extern const UnitType Unknown;

  }
}

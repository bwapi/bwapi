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

  /** The UnitType class is used to get information about a particular type of unit, such as the build time
   * of a Lurker, or the mineral price of an Ultralisk. TODO Add the unittype table from the wiki*/
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

      /** Returns a unique ID for this unit type. */
      int getID() const;

      /** Returns the name of the unit. */
      std::string getName() const;

      /** Returns the rank/sublabel of the unit, i.e. Private, Captain, Major, etc. */
      std::string getSubLabel() const;

      /** Returns the race that the unit belongs to. For example UnitTypes::Terran_SCV.getRace() will return
       * Races::Terran. */
      Race getRace() const;

      /** Returns what builds this unit type. The second number will usually be 1 unless the unit type is
       * Protoss_Archon or Protoss_Dark_Archon. Units that cannot be created, such as critters and mineral
       * fields, will return a pair where the unit type is UnitTypes::None, and the second component is 0.
       *
       * Example: UnitTypes::Terran_Marine.whatBuilds() will return an std::pair, where the first component
       * is UnitTypes::Terran_Barracks. */
      const std::pair< UnitType, int > whatBuilds() const;

      /** Returns the units the player is required to have before it can train or build the given unit type.
       *
       * Example: UnitTypes::Terran_Battlecruiser.requiredUnits() will return a map of three keys:
       * UnitTypes::Terran_Starport, UnitTypes::Terran_Control_Tower, and UnitTypes::Terran_Physics_Lab. */
      const std::map< UnitType, int >& requiredUnits() const;

      /** Included in the API for completeness, since the only units that actually needs tech to be trained
       * are the Zerg_Lurker and Zerg_Lurker_Egg. The tech type needed is TechTypes::Lurker_Aspect. */
      TechType requiredTech() const;

      /** Returns the set of tech types this unit can use, provided the tech types have been researched and
       * the unit has enough energy. */
      const std::set< TechType >& abilities() const;

      /** Returns the set of upgrade types that can affect this unit. */
      const std::set< UpgradeType >& upgrades() const;

      /** Returns the upgrade that increase's the unit's armor, or UpgradeTypes::None if no upgrade
       * increase's this unit's armor. For example UnitTypes::Terran_Marine.armorUpgrade() will return a
       * pointer to UpgradeTypes::Terran_Infantry_Armor. */
      UpgradeType armorUpgrade() const;

      /** Returns the maximum amount of hit points the unit type can have. */
      int maxHitPoints() const;

      /** Returns the maximum amount of shields the unit type can have. */
      int maxShields() const;

      /** Returns the maximum amount of energy the unit type can have. */
      int maxEnergy() const;

      /** Returns the amount of armor the non-upgraded unit type has. */
      int armor() const;

      /** Returns the mineral price of the unit.
       *
       * Example: UnitTypes::Siege_Tank_Tank_Mode.mineralPrice() returns 150. */
      int mineralPrice() const;

      /** UnitTypes::Siege_Tank_Tank_Mode.gasPrice() returns 100. */
      int gasPrice() const;

      /** Returns the number of frames needed to make this unit type. */
      int buildTime() const;

      /** Returns the amount of supply used by this unit. Supply counts returned by BWAPI are double what you
       *  would expect to see from playing the game. This is because zerglings take up 0.5 in-game supply. */
      int supplyRequired() const;

      /** Returns the amount of supply produced by this unit (i.e. for a Protoss_Pylon). Supply counts
       * returned by BWAPI are double what you would expect to see from playing the game. This is because
       * zerglings take up 0.5 in-game supply. */
      int supplyProvided() const;

      /** Returns the amount of space this unit type takes up inside a bunker or transport unit. */
      int spaceRequired() const;

      /** Returns the amount of space this unit type provides. */
      int spaceProvided() const;

      /** Returns the score which is used to determine the total scores in the after-game stats screen. */
      int buildScore() const;

      /** Returns the score which is used to determine the total scores in the after-game stats screen. */
      int destroyScore() const;

      /** Returns the size of the unit - either Small, Medium, Large, or Independent. */
      UnitSizeType size() const;

      /** Returns the tile width of the unit. Useful for determining the size of buildings. For example
       * UnitTypes::Terran_Supply_Depot.tileWidth() will return 3. */
      int tileWidth() const;

      /** Returns the tile height of the unit. Useful for determining the size of buildings. For example
       * UnitTypes::Terran_Supply_Depot.tileHeight() will return 2. */
      int tileHeight() const;

      /** Distance from the center of the unit to the left edge of the unit, measured in pixels. */
      int dimensionLeft() const;

      /** Distance from the center of the unit to the top edge of the unit, measured in pixels. */
      int dimensionUp() const;

      /** Distance from the center of the unit to the right edge of the unit, measured in pixels. */
      int dimensionRight() const;

      /** Distance from the center of the unit to the bottom edge of the unit, measured in pixels. */
      int dimensionDown() const;

      /** Returns the range at which the unit will start targeting enemy units, measured in pixels. */
      int seekRange() const;

      /** Returns how far the un-upgraded unit type can see into the fog of war, measured in pixels. */
      int sightRange() const;

      /** Returns the unit's ground weapon. */
      WeaponType groundWeapon() const;

      // TODO: add doc
      int maxGroundHits() const;

      /** Returns the unit's air weapon. */
      WeaponType airWeapon() const;

      // TODO: add doc
      int maxAirHits() const;

      /** Returns the unit's non-upgraded top speed in pixels per frame. For Terran buildings that can lift
       * off and the Zerg Infested Command Center, this returns how fast the building moves when it is
       * lifted. */
      double topSpeed() const;

      /** Returns how fast the unit can accelerate to its top speed. What units this quantity is measured in
       * is currently unknown. */
      int acceleration() const;

      /** Related to how fast the unit can halt. What units this quantity is measured in is currently
       * unknown. */
      int haltDistance() const;

      /** Related to how fast the unit can turn. What units this quantity is measured in is currently
       * unknown. */
      int turnRadius() const;

      /** Returns true if the unit can train other units. For example, UnitTypes::Terran_Barracks.canProduce()
       * will return true, while UnitTypes::Terran_Marine?.canProduce() will return false. This is also true
       * for two non-building units: Protoss Carrier (can produce interceptors) and Protoss Reaver
       * (can produce scarabs). */
      bool canProduce() const;

      /** Returns true if the unit can attack (either ground or air). Returns false for units that can only
       * inflict damage via special abilities (such as Protoss High Templar). */
      bool canAttack() const;

      /** Returns true if the unit can move. Note that buildings will return false, even Terran buildings
       * which can move once lifted. */
      bool canMove() const;

      /** Returns true for flying/air units. */
      bool isFlyer() const;

      /** Returns true for units that regenerate health (i.e. zerg units). */
      bool regeneratesHP() const;

      /** Returns true if the unit type is capable of casting spells / using technology. */
      bool isSpellcaster() const;

      /** Returns true for the two units that are permanently cloaked - Protoss Observer and Protoss Dark
       * Templar. */
      bool hasPermanentCloak() const;

      /** Returns true for units that cannot be destroyed (i.e. Terran Nuclear Missile, Mineral Field,
       * Vespene Geyser, etc) */
      bool isInvincible() const;

      /** Returns true if the unit is organic, such as a Terran Marine. */
      bool isOrganic() const;

      /** Returns true if the unit is mechanical such as a Terran Vulture. */
      bool isMechanical() const;

      /** Returns true for the four robotic Protoss units - Probe, Shuttle, Reaver, and Observer. */
      bool isRobotic() const;

      /** Returns true for the seven units that can detect cloaked units - Terran Science Vessel, Spell
       * Scanner Sweep, Zerg Overlord, Protoss Observer, Terran Missile Turret, Zerg Spore Colony, and Protoss
       * Photon Cannon. */
      bool isDetector() const;

      /** Returns true for the five units that hold resources - Mineral Field, Vespene Geyser,
       * Terran Refinery, Zerg Extractor, and Protoss Assimilator. */
      bool isResourceContainer() const;

      /** Returns true for the five units that can accept resources - Terran Command Center, Protoss Nexus,
       * Zerg Hatchery, Zerg Lair, and Zerg Hive. */
      bool isResourceDepot() const;

      /** Returns true for Terran Refinery, Zerg Extractor, and Protoss Assimilator. */
      bool isRefinery() const;

      /** Returns true for Protoss Probe, Terran SCV, and Zerg Drone. */
      bool isWorker() const;

      /** Returns true for buildings that must be near a pylon to be constructed. */
      bool requiresPsi() const;

      /** Returns true for buildings that can only be built on zerg creep. */
      bool requiresCreep() const;

      /** Returns true for Zergling and Scourge. */
      bool isTwoUnitsInOneEgg() const;

      /** Returns true for Zerg Lurker and units that can burrow when burrow tech is researched. */
      bool isBurrowable() const;

      /** Returns true for units that can be cloaked - Terran Ghost and Terran Wraith. Does not include units
       * which have permanent cloak (Protoss Observer and Protoss Dark Templar). */
      bool isCloakable() const;

      /** Returns true if the unit is a building (also true for mineral field and vespene geyser). */
      bool isBuilding() const;

      /** Returns true if the unit is an add-on, such as a Terran Comsat Station. */
      bool isAddon() const;

      /** Returns true for Terran buildings that can lift off (i.e. Barracks). */
      bool isFlyingBuilding() const;

      /** Returns true if the unit is neutral, such as a critter or mineral field. */
      bool isNeutral() const;

      /** Returns true if the unit produces larva. */
      bool producesLarva() const;

    private:
      int id;
  };
  namespace UnitTypes
  {

    /** Given the name of a unit type, this function will return the unit type.
     * For example, UnitTypes::getUnitType("Terran Marine") will return UnitTypes::Terran_Marine. */
    UnitType getUnitType(std::string name);

    /** Returns the set of all the UnitTypes. */
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

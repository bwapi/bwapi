#pragma once
#include <map>

#include <BWAPI/Position.h>
#include <BWAPI/Type.h>

namespace BWAPI
{
  // Forward Declarations
  class TechType;
  class UpgradeType;
  class WeaponType;
  class UnitSizeType;
  class Race;

  namespace UnitTypes
  {
    /// Enumeration of unit types
    namespace Enum
    {
      /// Enumeration of unit types
      enum Enum
      {
        Terran_Marine = 0,
        Terran_Ghost,
        Terran_Vulture,
        Terran_Goliath,
        Terran_Goliath_Turret,
        Terran_Siege_Tank_Tank_Mode,
        Terran_Siege_Tank_Tank_Mode_Turret,
        Terran_SCV,
        Terran_Wraith,
        Terran_Science_Vessel,
        Hero_Gui_Montag,
        Terran_Dropship,
        Terran_Battlecruiser,
        Terran_Vulture_Spider_Mine,
        Terran_Nuclear_Missile,
        Terran_Civilian,
        Hero_Sarah_Kerrigan,
        Hero_Alan_Schezar,
        Hero_Alan_Schezar_Turret,
        Hero_Jim_Raynor_Vulture,
        Hero_Jim_Raynor_Marine,
        Hero_Tom_Kazansky,
        Hero_Magellan,
        Hero_Edmund_Duke_Tank_Mode,
        Hero_Edmund_Duke_Tank_Mode_Turret,
        Hero_Edmund_Duke_Siege_Mode,
        Hero_Edmund_Duke_Siege_Mode_Turret,
        Hero_Arcturus_Mengsk,
        Hero_Hyperion,
        Hero_Norad_II,
        Terran_Siege_Tank_Siege_Mode,
        Terran_Siege_Tank_Siege_Mode_Turret,
        Terran_Firebat,
        Spell_Scanner_Sweep,
        Terran_Medic,
        Zerg_Larva,
        Zerg_Egg,
        Zerg_Zergling,
        Zerg_Hydralisk,
        Zerg_Ultralisk,
        Zerg_Broodling,
        Zerg_Drone,
        Zerg_Overlord,
        Zerg_Mutalisk,
        Zerg_Guardian,
        Zerg_Queen,
        Zerg_Defiler,
        Zerg_Scourge,
        Hero_Torrasque,
        Hero_Matriarch,
        Zerg_Infested_Terran,
        Hero_Infested_Kerrigan,
        Hero_Unclean_One,
        Hero_Hunter_Killer,
        Hero_Devouring_One,
        Hero_Kukulza_Mutalisk,
        Hero_Kukulza_Guardian,
        Hero_Yggdrasill,
        Terran_Valkyrie,
        Zerg_Cocoon,
        Protoss_Corsair,
        Protoss_Dark_Templar,
        Zerg_Devourer,
        Protoss_Dark_Archon,
        Protoss_Probe,
        Protoss_Zealot,
        Protoss_Dragoon,
        Protoss_High_Templar,
        Protoss_Archon,
        Protoss_Shuttle,
        Protoss_Scout,
        Protoss_Arbiter,
        Protoss_Carrier,
        Protoss_Interceptor,
        Hero_Dark_Templar,
        Hero_Zeratul,
        Hero_Tassadar_Zeratul_Archon,
        Hero_Fenix_Zealot,
        Hero_Fenix_Dragoon,
        Hero_Tassadar,
        Hero_Mojo,
        Hero_Warbringer,
        Hero_Gantrithor,
        Protoss_Reaver,
        Protoss_Observer,
        Protoss_Scarab,
        Hero_Danimoth,
        Hero_Aldaris,
        Hero_Artanis,
        Critter_Rhynadon,
        Critter_Bengalaas,
        Special_Cargo_Ship,
        Special_Mercenary_Gunship,
        Critter_Scantid,
        Critter_Kakaru,
        Critter_Ragnasaur,
        Critter_Ursadon,
        Zerg_Lurker_Egg,
        Hero_Raszagal,
        Hero_Samir_Duran,
        Hero_Alexei_Stukov,
        Special_Map_Revealer,
        Hero_Gerard_DuGalle,
        Zerg_Lurker,
        Hero_Infested_Duran,
        Spell_Disruption_Web,
        Terran_Command_Center,
        Terran_Comsat_Station,
        Terran_Nuclear_Silo,
        Terran_Supply_Depot,
        Terran_Refinery,
        Terran_Barracks,
        Terran_Academy,
        Terran_Factory,
        Terran_Starport,
        Terran_Control_Tower,
        Terran_Science_Facility,
        Terran_Covert_Ops,
        Terran_Physics_Lab,
        Unused_Terran1,
        Terran_Machine_Shop,
        Unused_Terran2,
        Terran_Engineering_Bay,
        Terran_Armory,
        Terran_Missile_Turret,
        Terran_Bunker,
        Special_Crashed_Norad_II,
        Special_Ion_Cannon,
        Powerup_Uraj_Crystal,
        Powerup_Khalis_Crystal,
        Zerg_Infested_Command_Center,
        Zerg_Hatchery,
        Zerg_Lair,
        Zerg_Hive,
        Zerg_Nydus_Canal,
        Zerg_Hydralisk_Den,
        Zerg_Defiler_Mound,
        Zerg_Greater_Spire,
        Zerg_Queens_Nest,
        Zerg_Evolution_Chamber,
        Zerg_Ultralisk_Cavern,
        Zerg_Spire,
        Zerg_Spawning_Pool,
        Zerg_Creep_Colony,
        Zerg_Spore_Colony,
        Unused_Zerg1,
        Zerg_Sunken_Colony,
        Special_Overmind_With_Shell,
        Special_Overmind,
        Zerg_Extractor,
        Special_Mature_Chrysalis,
        Special_Cerebrate,
        Special_Cerebrate_Daggoth,
        Unused_Zerg2,
        Protoss_Nexus,
        Protoss_Robotics_Facility,
        Protoss_Pylon,
        Protoss_Assimilator,
        Unused_Protoss1,
        Protoss_Observatory,
        Protoss_Gateway,
        Unused_Protoss2,
        Protoss_Photon_Cannon,
        Protoss_Citadel_of_Adun,
        Protoss_Cybernetics_Core,
        Protoss_Templar_Archives,
        Protoss_Forge,
        Protoss_Stargate,
        Special_Stasis_Cell_Prison,
        Protoss_Fleet_Beacon,
        Protoss_Arbiter_Tribunal,
        Protoss_Robotics_Support_Bay,
        Protoss_Shield_Battery,
        Special_Khaydarin_Crystal_Form,
        Special_Protoss_Temple,
        Special_XelNaga_Temple,
        Resource_Mineral_Field,
        Resource_Mineral_Field_Type_2,
        Resource_Mineral_Field_Type_3,
        Unused_Cave,
        Unused_Cave_In,
        Unused_Cantina,
        Unused_Mining_Platform,
        Unused_Independant_Command_Center,
        Special_Independant_Starport,
        Unused_Independant_Jump_Gate,
        Unused_Ruins,
        Unused_Khaydarin_Crystal_Formation,
        Resource_Vespene_Geyser,
        Special_Warp_Gate,
        Special_Psi_Disrupter,
        Unused_Zerg_Marker,
        Unused_Terran_Marker,
        Unused_Protoss_Marker,
        Special_Zerg_Beacon,
        Special_Terran_Beacon,
        Special_Protoss_Beacon,
        Special_Zerg_Flag_Beacon,
        Special_Terran_Flag_Beacon,
        Special_Protoss_Flag_Beacon,
        Special_Power_Generator,
        Special_Overmind_Cocoon,
        Spell_Dark_Swarm,
        Special_Floor_Missile_Trap,
        Special_Floor_Hatch,
        Special_Upper_Level_Door,
        Special_Right_Upper_Level_Door,
        Special_Pit_Door,
        Special_Right_Pit_Door,
        Special_Floor_Gun_Trap,
        Special_Wall_Missile_Trap,
        Special_Wall_Flame_Trap,
        Special_Right_Wall_Missile_Trap,
        Special_Right_Wall_Flame_Trap,
        Special_Start_Location,
        Powerup_Flag,
        Powerup_Young_Chrysalis,
        Powerup_Psi_Emitter,
        Powerup_Data_Disk,
        Powerup_Khaydarin_Crystal,
        Powerup_Mineral_Cluster_Type_1,
        Powerup_Mineral_Cluster_Type_2,
        Powerup_Protoss_Gas_Orb_Type_1,
        Powerup_Protoss_Gas_Orb_Type_2,
        Powerup_Zerg_Gas_Sac_Type_1,
        Powerup_Zerg_Gas_Sac_Type_2,
        Powerup_Terran_Gas_Tank_Type_1,
        Powerup_Terran_Gas_Tank_Type_2,

        None,
        AllUnits,
        Men,
        Buildings,
        Factories,
        Unknown,
        MAX
      };

    };
  }
  /// The UnitType is used to get information about a particular type of unit, such as its cost,
  /// build time, weapon, hit points, abilities, etc.
  ///
  /// @see UnitInterface::getType
  class UnitType : public Type<UnitType, UnitTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UnitType(int id = UnitTypes::Enum::None);

    /// Retrieves the Race that the unit type belongs to.
    ///
    /// @returns Race indicating the race that owns this unit type.
    /// @retval Race::None indicating that the unit type does not belong to any particular race (a
    /// critter for example).
    Race getRace() const;

    /// Obtains the source unit type that is used to build or train this unit type, as well as the
    /// amount of them that are required.
    ///
    /// @returns std::pair in which the first value is the UnitType that builds this unit type, and
    /// the second value is the number of those types that are required (this value is 2 for
    /// @Archons, and 1 for all other types).
    /// @retval pair(UnitTypes::None,0) If this unit type cannot be made by the player.
    const std::pair< UnitType, int > whatBuilds() const;

    /// Retrieves the immediate technology tree requirements to make this unit type.
    ///
    /// @returns std::map containing a UnitType to number mapping of UnitTypes required.
    const std::map< UnitType, int >& requiredUnits() const;

    /// Identifies the required TechType in order to create certain units.
    ///
    /// @note The only unit that requires a technology is the @Lurker, which needs @Lurker_Aspect.
    /// @returns TechType indicating the technology that must be researched in order to create this
    /// unit type.
    /// @retval TechTypes::None If creating this unit type does not require a technology to be
    /// researched.
    TechType requiredTech() const;
    
    /// Retrieves the cloaking technology associated with certain units.
    ///
    /// @returns TechType referring to the cloaking technology that this unit type uses as an
    /// ability.
    /// @retval TechTypes::None If this unit type does not have an active cloak ability.
    TechType cloakingTech() const;

    /// Retrieves the set of abilities that this unit can use, provided it is available to you in
    /// the game.
    ///
    /// @returns Set of TechTypes containing ability information.
    const ConstVectorset<TechType>& abilities() const;

    /// Retrieves the set of upgrades that this unit can use to enhance its fighting ability.
    ///
    /// @return Set of UpgradeTypes containing upgrade types that will impact this unit type.
    const ConstVectorset<UpgradeType>& upgrades() const;

    /// Retrieves the upgrade type used to increase the armor of this unit type. For each upgrade,
    /// this unit type gains +1 additional armor.
    ///
    /// @returns UpgradeType indicating the upgrade that increases this unit type's armor amount.
    UpgradeType armorUpgrade() const;

    /// Retrieves the default maximum amount of hit points that this unit type can have.
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Integer indicating the maximum amount of hit points for this unit type.
    int maxHitPoints() const;

    /// Retrieves the default maximum amount of shield points that this unit type can have.
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Integer indicating the maximum amount of shield points for this unit type.
    /// @retval 0 If this unit type does not have shields.
    int maxShields() const;

    /// Retrieves the maximum amount of energy this unit type can have by default.
    ///
    /// @returns Integer indicating the maximum amount of energy for this unit type.
    /// @retval 0 If this unit does not gain energy for abilities.
    int maxEnergy() const;

    /// Retrieves the default amount of armor that the unit type starts with, excluding upgrades.
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns The amount of armor the unit type has.
    int armor() const;

    /// Retrieves the default mineral price of purchasing the unit.
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Mineral cost of the unit.
    int mineralPrice() const;

    /// Retrieves the default vespene gas price of purchasing the unit.
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Vespene gas cost of the unit.
    int gasPrice() const;

    /// Retrieves the default time, in frames, needed to train, morph, or build the unit.
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Number of frames needed in order to build the unit.
    /// @see UnitInterface::getRemainingBuildTime
    int buildTime() const;

    /// Retrieves the amount of supply that this unit type will use when created. It will use the
    /// supply pool that is appropriate for its Race.
    ///
    /// @note In Starcraft programming, the managed supply values are double than what they appear
    /// in the game. The reason for this is because @Zerglings use 0.5 visible supply.
    ///
    /// @returns Integer containing the supply required to build this unit.
    /// @see supplyProvided, PlayerInterface::supplyTotal, PlayerInterface::supplyUsed
    int supplyRequired() const;

    /// Retrieves the amount of supply that this unit type produces for its appropriate Race's
    /// supply pool.
    ///
    /// @note In Starcraft programming, the managed supply values are double than what they appear
    /// in the game. The reason for this is because @Zerglings use 0.5 visible supply.
    ///
    /// @see supplyRequired, PlayerInterface::supplyTotal, PlayerInterface::supplyUsed
    int supplyProvided() const;

    /// Retrieves the amount of space required by this unit type to fit inside a @Bunker or
    /// @Transport.
    ///
    /// @returns Amount of space required by this unit type for transport.
    /// @retval 255 If this unit type can not be transported.
    /// @see spaceProvided
    int spaceRequired() const;

    /// Retrieves the amount of space provided by this @Bunker or @Transport for unit
    /// transportation.
    /// 
    /// @returns The number of slots provided by this unit type.
    /// @see spaceRequired
    int spaceProvided() const;

    /// Retrieves the amount of score points awarded for constructing this unit type. This value is
    /// used for calculating scores in the post-game score screen.
    ///
    /// @returns Number of points awarded for constructing this unit type.
    /// @see destroyScore
    int buildScore() const;

    /// Retrieves the amount of score points awarded for killing this unit type. This value is
    /// used for calculating scores in the post-game score screen.
    ///
    /// @returns Number of points awarded for killing this unit type.
    /// @see buildScore
    int destroyScore() const;

    /// Retrieves the UnitSizeType of this unit, which is used in calculations along with weapon
    /// damage types to determine the amount of damage that will be dealt to this type.
    ///
    /// @returns UnitSizeType indicating the conceptual size of the unit type.
    /// @see WeaponType::damageType
    UnitSizeType size() const;

    /// Retrieves the width of this unit type, in tiles. Used for determining the tile size of
    /// structures.
    ///
    /// @returns Width of this unit type, in tiles.
    int tileWidth() const;

    /// Retrieves the height of this unit type, in tiles. Used for determining the tile size of
    /// structures.
    ///
    /// @returns Height of this unit type, in tiles.
    int tileHeight() const;

    /// Retrieves the tile size of this unit type. Used for determining the tile size of
    /// structures.
    ///
    /// @returns TilePosition containing the width (x) and height (y) of the unit type, in tiles.
    TilePosition tileSize() const;

    /// Retrieves the distance from the center of the unit type to its left edge.
    ///
    /// @returns Distance to this unit type's left edge from its center, in pixels.
    int dimensionLeft() const;

    /// Retrieves the distance from the center of the unit type to its top edge.
    ///
    /// @returns Distance to this unit type's top edge from its center, in pixels.
    int dimensionUp() const;

    /// Retrieves the distance from the center of the unit type to its right edge.
    ///
    /// @returns Distance to this unit type's right edge from its center, in pixels.
    int dimensionRight() const;

    /// Retrieves the distance from the center of the unit type to its bottom edge.
    ///
    /// @returns Distance to this unit type's bottom edge from its center, in pixels.
    int dimensionDown() const;

    /// A macro for retrieving the width of the unit type, which is calculated using
    /// dimensionLeft + dimensionRight + 1.
    ///
    /// @returns Width of the unit, in pixels.
    int width() const;

    /// A macro for retrieving the height of the unit type, which is calculated using
    /// dimensionUp + dimensionDown + 1.
    ///
    /// @returns Height of the unit, in pixels.
    int height() const;

    /// Retrieves the range at which this unit type will start targeting enemy units.
    ///
    /// @returns Distance at which this unit type begins to seek out enemy units, in pixels.
    int seekRange() const;

    /// Retrieves the sight range of this unit type.
    ///
    /// @returns Sight range of this unit type, measured in pixels.
    int sightRange() const;

    /// Retrieves this unit type's weapon type used when attacking targets on the ground.
    ///
    /// @returns WeaponType used as this unit type's ground weapon.
    /// @see maxGroundHits, airWeapon
    WeaponType groundWeapon() const;

    /// Retrieves the maximum number of hits this unit can deal to a ground target using its
    /// ground weapon. This value is multiplied by the ground weapon's damage to calculate the
    /// unit type's damage potential.
    ///
    /// @returns Maximum number of hits given to ground targets.
    /// @see groundWeapon, maxAirHits
    int maxGroundHits() const;

    /// Retrieves this unit type's weapon type used when attacking targets in the air.
    ///
    /// @returns WeaponType used as this unit type's air weapon.
    /// @see maxAirHits, groundWeapon
    WeaponType airWeapon() const;

    /// Retrieves the maximum number of hits this unit can deal to a flying target using its
    /// air weapon. This value is multiplied by the air weapon's damage to calculate the
    /// unit type's damage potential.
    ///
    /// @returns Maximum number of hits given to air targets.
    /// @see airWeapon, maxGroundHits
    int maxAirHits() const;

    /// Retrieves this unit type's top movement speed with no upgrades.
    ///
    /// @note That some units have inconsistent movement and this value is sometimes an
    /// approximation.
    ///
    /// @returns The approximate top speed, in pixels per frame, as a double. For liftable @Terran
    /// structures, this function returns their movement speed while lifted.
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

    /// Checks if this unit type is a flying unit. Flying units ignore ground pathing and
    /// collisions.
    /// 
    /// @returns true if this unit type is in the air by default, and false otherwise
    bool isFlyer() const;

    /// Checks if this unit type can regenerate hit points. This generally applies to @Zerg units.
    ///
    /// @returns true if this unit type regenerates its hit points, and false otherwise.
    bool regeneratesHP() const;

    /// Checks if this unit type has the capacity to store energy and use it for special abilities.
    ///
    /// @returns true if this unit type generates energy, and false if it does not have an energy
    /// pool.
    bool isSpellcaster() const;

    /// Checks if this unit type is permanently cloaked. This means the unit type is always
    /// cloaked and requires a detector in order to see it.
    ///
    /// @returns true if this unit type is permanently cloaked, and false otherwise.
    bool hasPermanentCloak() const;

    /// Checks if this unit type is invincible by default. Invincible units cannot be destroyed by
    /// other units' attacks.
    ///
    /// @returns true if this unit type is invincible, and false if it is vulnerable to attacks.
    bool isInvincible() const;

    /// Checks if this unit is an organic unit. The organic property is required for some abilities
    /// such as @Heal.
    ///
    /// @returns true if this unit type has the organic property, and false otherwise.
    bool isOrganic() const;

    /// Checks if this unit is mechanical. The mechanical property is required for some actions
    /// such as @Repair.
    ///
    /// @returns true if this unit type has the mechanical property, and false otherwise.
    bool isMechanical() const;

    /// Checks if this unit is robotic. The robotic property is @todo finish this
    ///
    /// @returns true if this unit type has the robotic property, and false otherwise.
    bool isRobotic() const;

    /// Checks if this unit type is capable of detecting units that are cloaked or burrowed.
    ///
    /// @returns true if this unit type is a detector by default, false if it does not have this
    /// property
    bool isDetector() const;

    /// Checks if this unit type is capable of storing resources such as @minerals . Resources
    /// are harvested from resource containers.
    ///
    /// @returns true if this unit type may contain resources that can be harvested, false
    /// otherwise.
    bool isResourceContainer() const;

    /// Checks if this unit type is a resource depot. Resource depots must be placed a certain
    /// distance from resources. Resource depots are typically the main building for any
    /// particular race. Workers will return resources to the nearest resource depot.
    ///
    /// Example:
    /// @code
    ///   if ( BWAPI::Broodwar->self() )
    ///   {
    ///     BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///     for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///     {
    ///       if ( u->isIdle() && u->getType().isResourceDepot() )
    ///         u->train( u->getType().getRace().getWorker() );
    ///     }
    ///   }
    /// @endcode
    /// @returns true if the unit type is a resource depot, false if it is not.
    bool isResourceDepot() const;

    /// Checks if this unit type is a refinery. A refinery is a structure that is placed on top of
    /// a @geyser . Refinery types are @refinery , @extractor , and @assimilator.
    ///
    /// Example:
    /// @code
    ///   if ( BWAPI::Broodwar->self() )
    ///   {
    ///     BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///     for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///     {
    ///       if ( u->getType().isRefinery() )
    ///       {
    ///         int nWorkersAssigned = u->getClientInfo<int>('work');
    ///         if ( nWorkersAssigned < 3 )
    ///         {
    ///           Unit pClosestIdleWorker = u->getClosestUnit(BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle);
    ///           if ( pClosestIdleWorker )
    ///           {
    ///             // gather from the refinery (and check if successful)
    ///             if ( pClosestIdleWorker->gather(*u) )
    ///             {
    ///               // set a back reference for when the unit is killed or re-assigned (code not provided)
    ///               pClosestIdleWorker->setClientInfo(*u, 'ref');
    ///
    ///               // Increment the number of workers assigned and associate it with the refinery
    ///               ++nWorkersAssigned;
    ///               u->setClientInfo(nWorkersAssigned, 'work');
    ///             }
    ///           }
    ///         } // workers < 3
    ///       } // isRefinery
    ///     } // for
    ///   }
    /// @endcode
    /// @returns true if this unit type is a refinery, and false if it is not.
    bool isRefinery() const;

    /// Checks if this unit type is a worker unit. Worker units can harvest resources and build
    /// structures. Worker unit types include the @SCV , @probe, and @drone.
    ///
    /// @returns true if this unit type is a worker, and false if it is not.
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

    /** Returns true if the unit is a Hero unit. */
    bool isHero() const;

    /** Returns true if the unit is a Powerup unit. */
    bool isPowerup() const;

    /** Returns true if the unit is a regular Beacon. */
    bool isBeacon() const;

    /** Returns true if the unit is a flag Beacon. */
    bool isFlagBeacon() const;

    /** Returns true if the unit is a special building. */
    bool isSpecialBuilding() const;

    /** Returns true if the unit is a spell unit. */
    bool isSpell() const;

    /** Returns true if the unit produces larva. */
    bool producesLarva() const;

    /** Returns true if the unit is one of the three mineral field types. */
    bool isMineralField() const;

    /// Checks if this unit type is a neutral critter.
    ///
    /// @returns true if this unit type is a critter, and false otherwise.
    ///
    /// Example usage:
    /// @code
    ///   BWAPI::Position myBasePosition( BWAPI::Broodwar->self()->getStartLocation() );
    ///   BWAPI::UnitSet unitsAroundTheBase = BWAPI::Broodwar->getUnitsInRadius(myBasePosition, 1024, !BWAPI::Filter::IsOwned && !BWAPI::Filter::IsParasited);
    ///   for ( auto u = unitsAroundTheBase.begin(); u != unitsAroundTheBase.end(); ++u )
    ///   {
    ///     if ( u->getType().isCritter() && !u->isInvincible() )
    ///     {
    ///       BWAPI::Unit myQueen = u->getClosestUnit(BWAPI::Filter::GetType == BWAPI::UnitTypes::Zerg_Queen && BWAPI::Filter::IsOwned);
    ///       if ( myQueen )
    ///         myQueen->useTech(BWAPI::TechTypes::Parasite, *u);
    ///     }
    ///   }
    /// @endcode
    bool isCritter() const;

    /// Checks if this unit type is capable of constructing an add-on. An add-on is an extension
    /// or attachment for @Terran structures, specifically the @Command_Center, @Factory,
    /// @Starport, and @Science_Facility.
    ///
    /// @returns true if this unit type can construct an add-on, and false if it can not.
    /// @see isAddon
    bool canBuildAddon() const;
  };

  /// Namespace containing unit types
  namespace UnitTypes
  {
    /// Retrieves the maximum unit width from the set of all units.
    ///
    /// @returns The maximum width of any unit, in pixels.
    int maxUnitWidth();
    
    /// Retrieves the maximum unit height from the set of all units.
    ///
    /// @returns The maximum height of any unit, in pixels.
    int maxUnitHeight();

    /// Retrieves the set of all unit types.
    ///
    /// @returns A constant set of all available unit types.
    const UnitType::const_set& allUnitTypes();

    /// Retrieves the set of all macro unit types. A macro type is a fake unit type, used by some
    /// functions. These include #AllUnits, #Men, #Buildings, and #Factories. The purpose of these
    /// types are to match the same ones used in Broodwar, also seen in the StarEdit map editor.
    ///
    /// @returns A constant set of all macro unit types.
    const UnitType::const_set& allMacroTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const UnitType x
    BWAPI_DECL(Terran_Marine);
    BWAPI_DECL(Terran_Ghost);
    BWAPI_DECL(Terran_Vulture);
    BWAPI_DECL(Terran_Goliath);
    // goliath turret 4
    BWAPI_DECL(Terran_Siege_Tank_Tank_Mode);
    // siege tank turret 6
    BWAPI_DECL(Terran_SCV);
    BWAPI_DECL(Terran_Wraith);
    BWAPI_DECL(Terran_Science_Vessel);
    BWAPI_DECL(Hero_Gui_Montag);
    BWAPI_DECL(Terran_Dropship);
    BWAPI_DECL(Terran_Battlecruiser);
    BWAPI_DECL(Terran_Vulture_Spider_Mine);
    BWAPI_DECL(Terran_Nuclear_Missile);
    BWAPI_DECL(Terran_Civilian);
    BWAPI_DECL(Hero_Sarah_Kerrigan);
    BWAPI_DECL(Hero_Alan_Schezar);
    // alan turret 18
    BWAPI_DECL(Hero_Jim_Raynor_Vulture);
    BWAPI_DECL(Hero_Jim_Raynor_Marine);
    BWAPI_DECL(Hero_Tom_Kazansky);
    BWAPI_DECL(Hero_Magellan);
    BWAPI_DECL(Hero_Edmund_Duke_Tank_Mode);
    // edmund duke turret 24
    BWAPI_DECL(Hero_Edmund_Duke_Siege_Mode);
    // edmund duke turret siege mode 26
    BWAPI_DECL(Hero_Arcturus_Mengsk);
    BWAPI_DECL(Hero_Hyperion);
    BWAPI_DECL(Hero_Norad_II);
    BWAPI_DECL(Terran_Siege_Tank_Siege_Mode);
    // siege tank siege mode turret 31
    BWAPI_DECL(Terran_Firebat);
    BWAPI_DECL(Spell_Scanner_Sweep);
    BWAPI_DECL(Terran_Medic);
    BWAPI_DECL(Zerg_Larva);
    BWAPI_DECL(Zerg_Egg);
    BWAPI_DECL(Zerg_Zergling);
    BWAPI_DECL(Zerg_Hydralisk);
    BWAPI_DECL(Zerg_Ultralisk);
    BWAPI_DECL(Zerg_Broodling);
    BWAPI_DECL(Zerg_Drone);
    BWAPI_DECL(Zerg_Overlord);
    BWAPI_DECL(Zerg_Mutalisk);
    BWAPI_DECL(Zerg_Guardian);
    BWAPI_DECL(Zerg_Queen);
    BWAPI_DECL(Zerg_Defiler);
    BWAPI_DECL(Zerg_Scourge);
    BWAPI_DECL(Hero_Torrasque);
    BWAPI_DECL(Hero_Matriarch);
    BWAPI_DECL(Zerg_Infested_Terran);
    BWAPI_DECL(Hero_Infested_Kerrigan);
    BWAPI_DECL(Hero_Unclean_One);
    BWAPI_DECL(Hero_Hunter_Killer);
    BWAPI_DECL(Hero_Devouring_One);
    BWAPI_DECL(Hero_Kukulza_Mutalisk);
    BWAPI_DECL(Hero_Kukulza_Guardian);
    BWAPI_DECL(Hero_Yggdrasill);
    BWAPI_DECL(Terran_Valkyrie);
    BWAPI_DECL(Zerg_Cocoon);
    BWAPI_DECL(Protoss_Corsair);
    BWAPI_DECL(Protoss_Dark_Templar);
    BWAPI_DECL(Zerg_Devourer);
    BWAPI_DECL(Protoss_Dark_Archon);
    BWAPI_DECL(Protoss_Probe);
    BWAPI_DECL(Protoss_Zealot);
    BWAPI_DECL(Protoss_Dragoon);
    BWAPI_DECL(Protoss_High_Templar);
    BWAPI_DECL(Protoss_Archon);
    BWAPI_DECL(Protoss_Shuttle);
    BWAPI_DECL(Protoss_Scout);
    BWAPI_DECL(Protoss_Arbiter);
    BWAPI_DECL(Protoss_Carrier);
    BWAPI_DECL(Protoss_Interceptor);
    BWAPI_DECL(Hero_Dark_Templar);
    BWAPI_DECL(Hero_Zeratul);
    BWAPI_DECL(Hero_Tassadar_Zeratul_Archon);
    BWAPI_DECL(Hero_Fenix_Zealot);
    BWAPI_DECL(Hero_Fenix_Dragoon);
    BWAPI_DECL(Hero_Tassadar);
    BWAPI_DECL(Hero_Mojo);
    BWAPI_DECL(Hero_Warbringer);
    BWAPI_DECL(Hero_Gantrithor);
    BWAPI_DECL(Protoss_Reaver);
    BWAPI_DECL(Protoss_Observer);
    BWAPI_DECL(Protoss_Scarab);
    BWAPI_DECL(Hero_Danimoth);
    BWAPI_DECL(Hero_Aldaris);
    BWAPI_DECL(Hero_Artanis);
    BWAPI_DECL(Critter_Rhynadon);
    BWAPI_DECL(Critter_Bengalaas);
    BWAPI_DECL(Special_Cargo_Ship);
    BWAPI_DECL(Special_Mercenary_Gunship);
    BWAPI_DECL(Critter_Scantid);
    BWAPI_DECL(Critter_Kakaru);
    BWAPI_DECL(Critter_Ragnasaur);
    BWAPI_DECL(Critter_Ursadon);
    BWAPI_DECL(Zerg_Lurker_Egg);
    BWAPI_DECL(Hero_Raszagal);
    BWAPI_DECL(Hero_Samir_Duran);
    BWAPI_DECL(Hero_Alexei_Stukov);
    BWAPI_DECL(Special_Map_Revealer);
    BWAPI_DECL(Hero_Gerard_DuGalle);
    BWAPI_DECL(Zerg_Lurker);
    BWAPI_DECL(Hero_Infested_Duran);
    BWAPI_DECL(Spell_Disruption_Web);
    BWAPI_DECL(Terran_Command_Center);
    BWAPI_DECL(Terran_Comsat_Station);
    BWAPI_DECL(Terran_Nuclear_Silo);
    BWAPI_DECL(Terran_Supply_Depot);
    BWAPI_DECL(Terran_Refinery);
    BWAPI_DECL(Terran_Barracks);
    BWAPI_DECL(Terran_Academy);
    BWAPI_DECL(Terran_Factory);
    BWAPI_DECL(Terran_Starport);
    BWAPI_DECL(Terran_Control_Tower);
    BWAPI_DECL(Terran_Science_Facility);
    BWAPI_DECL(Terran_Covert_Ops);
    BWAPI_DECL(Terran_Physics_Lab);
    // starbase 119
    BWAPI_DECL(Terran_Machine_Shop);
    // repair bay 121
    BWAPI_DECL(Terran_Engineering_Bay);
    BWAPI_DECL(Terran_Armory);
    BWAPI_DECL(Terran_Missile_Turret);
    BWAPI_DECL(Terran_Bunker);
    BWAPI_DECL(Special_Crashed_Norad_II);
    BWAPI_DECL(Special_Ion_Cannon);
    BWAPI_DECL(Powerup_Uraj_Crystal);
    BWAPI_DECL(Powerup_Khalis_Crystal);
    BWAPI_DECL(Zerg_Infested_Command_Center);
    BWAPI_DECL(Zerg_Hatchery);
    BWAPI_DECL(Zerg_Lair);
    BWAPI_DECL(Zerg_Hive);
    BWAPI_DECL(Zerg_Nydus_Canal);
    BWAPI_DECL(Zerg_Hydralisk_Den);
    BWAPI_DECL(Zerg_Defiler_Mound);
    BWAPI_DECL(Zerg_Greater_Spire);
    BWAPI_DECL(Zerg_Queens_Nest);
    BWAPI_DECL(Zerg_Evolution_Chamber);
    BWAPI_DECL(Zerg_Ultralisk_Cavern);
    BWAPI_DECL(Zerg_Spire);
    BWAPI_DECL(Zerg_Spawning_Pool);
    BWAPI_DECL(Zerg_Creep_Colony);
    BWAPI_DECL(Zerg_Spore_Colony);
    // unused zerg 1 145
    BWAPI_DECL(Zerg_Sunken_Colony);
    BWAPI_DECL(Special_Overmind_With_Shell);
    BWAPI_DECL(Special_Overmind);
    BWAPI_DECL(Zerg_Extractor);
    BWAPI_DECL(Special_Mature_Chrysalis);
    BWAPI_DECL(Special_Cerebrate);
    BWAPI_DECL(Special_Cerebrate_Daggoth);
    // unused zerg 2 153
    BWAPI_DECL(Protoss_Nexus);
    BWAPI_DECL(Protoss_Robotics_Facility);
    BWAPI_DECL(Protoss_Pylon);
    BWAPI_DECL(Protoss_Assimilator);
    // unused protoss 1 158
    BWAPI_DECL(Protoss_Observatory);
    BWAPI_DECL(Protoss_Gateway);
    // unused protoss 2 161
    BWAPI_DECL(Protoss_Photon_Cannon);
    BWAPI_DECL(Protoss_Citadel_of_Adun);
    BWAPI_DECL(Protoss_Cybernetics_Core);
    BWAPI_DECL(Protoss_Templar_Archives);
    BWAPI_DECL(Protoss_Forge);
    BWAPI_DECL(Protoss_Stargate);
    BWAPI_DECL(Special_Stasis_Cell_Prison);
    BWAPI_DECL(Protoss_Fleet_Beacon);
    BWAPI_DECL(Protoss_Arbiter_Tribunal);
    BWAPI_DECL(Protoss_Robotics_Support_Bay);
    BWAPI_DECL(Protoss_Shield_Battery);
    BWAPI_DECL(Special_Khaydarin_Crystal_Form);
    BWAPI_DECL(Special_Protoss_Temple);
    BWAPI_DECL(Special_XelNaga_Temple);
    BWAPI_DECL(Resource_Mineral_Field);
    BWAPI_DECL(Resource_Mineral_Field_Type_2);
    BWAPI_DECL(Resource_Mineral_Field_Type_3);
    // cave 179
    // cave-in 180
    // cantina 181
    // mining platform 182
    // independant command center 183
    BWAPI_DECL(Special_Independant_Starport);
    // independant jump gate 185
    // ruins 186
    // unused khaydarin crystal formation 187
    BWAPI_DECL(Resource_Vespene_Geyser);
    BWAPI_DECL(Special_Warp_Gate);
    BWAPI_DECL(Special_Psi_Disrupter);
    // zerg marker 191
    // terran marker 192
    // protoss marker 193
    BWAPI_DECL(Special_Zerg_Beacon);
    BWAPI_DECL(Special_Terran_Beacon);
    BWAPI_DECL(Special_Protoss_Beacon);
    BWAPI_DECL(Special_Zerg_Flag_Beacon);
    BWAPI_DECL(Special_Terran_Flag_Beacon);
    BWAPI_DECL(Special_Protoss_Flag_Beacon);
    BWAPI_DECL(Special_Power_Generator);
    BWAPI_DECL(Special_Overmind_Cocoon);
    BWAPI_DECL(Spell_Dark_Swarm);
    BWAPI_DECL(Special_Floor_Missile_Trap);
    BWAPI_DECL(Special_Floor_Hatch);
    BWAPI_DECL(Special_Upper_Level_Door);
    BWAPI_DECL(Special_Right_Upper_Level_Door);
    BWAPI_DECL(Special_Pit_Door);
    BWAPI_DECL(Special_Right_Pit_Door);
    BWAPI_DECL(Special_Floor_Gun_Trap);
    BWAPI_DECL(Special_Wall_Missile_Trap);
    BWAPI_DECL(Special_Wall_Flame_Trap);
    BWAPI_DECL(Special_Right_Wall_Missile_Trap);
    BWAPI_DECL(Special_Right_Wall_Flame_Trap);
    BWAPI_DECL(Special_Start_Location);
    BWAPI_DECL(Powerup_Flag);
    BWAPI_DECL(Powerup_Young_Chrysalis);
    BWAPI_DECL(Powerup_Psi_Emitter);
    BWAPI_DECL(Powerup_Data_Disk);
    BWAPI_DECL(Powerup_Khaydarin_Crystal);
    BWAPI_DECL(Powerup_Mineral_Cluster_Type_1);
    BWAPI_DECL(Powerup_Mineral_Cluster_Type_2);
    BWAPI_DECL(Powerup_Protoss_Gas_Orb_Type_1);
    BWAPI_DECL(Powerup_Protoss_Gas_Orb_Type_2);
    BWAPI_DECL(Powerup_Zerg_Gas_Sac_Type_1);
    BWAPI_DECL(Powerup_Zerg_Gas_Sac_Type_2);
    BWAPI_DECL(Powerup_Terran_Gas_Tank_Type_1);
    BWAPI_DECL(Powerup_Terran_Gas_Tank_Type_2);

    BWAPI_DECL(None);
    BWAPI_DECL(AllUnits);
    BWAPI_DECL(Men);
    BWAPI_DECL(Buildings);
    BWAPI_DECL(Factories);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}

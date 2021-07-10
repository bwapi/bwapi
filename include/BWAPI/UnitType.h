#pragma once
#include <map>

#include <BWAPI/Type.h>

namespace BWAPI
{
  // Forward Declarations
  class TechType;
  class UpgradeType;
  class WeaponType;
  class UnitSizeType;
  class Race;

  template<typename T, int Scale> class Point;
  typedef Point<int, 32> TilePosition;

  /// <summary>Namespace containing unit types.</summary>
  /// @see UnitType
  namespace UnitTypes
  {
    /// <summary>Enumeration of unit types</summary>
    /// @see UnitType
    namespace Enum
    {
      /// <summary>Enumeration of unit types</summary>
      /// @see UnitType
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

    }
  }
  /// <summary>The UnitType is used to get information about a particular type of unit, such as its cost,
  /// build time, weapon, hit points, abilities, etc.</summary>
  ///
  /// @see Unit::getType, UnitTypes
  /// @ingroup TypeClasses
  class UnitType : public Type<UnitType, UnitTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    constexpr explicit UnitType(int id = UnitTypes::Enum::None) : Type(id) {}

    /// <summary>Retrieves the Race that the unit type belongs to.</summary>
    ///
    /// @returns Race indicating the race that owns this unit type.
    /// @retval Race::None indicating that the unit type does not belong to any particular race (a
    /// critter for example).
    Race getRace() const;

    /// <summary>Obtains the source unit type that is used to build or train this unit type, as well as the
    /// amount of them that are required.</summary>
    ///
    /// @returns std::pair in which the first value is the UnitType that builds this unit type, and
    /// the second value is the number of those types that are required (this value is 2 for
    /// @Archons, and 1 for all other types).
    /// @retval pair(UnitTypes::None,0) If this unit type cannot be made by the player.
    const std::pair< UnitType, int > whatBuilds() const;

    /// <summary>Retrieves the immediate technology tree requirements to make this unit type.</summary>
    ///
    /// @returns std::map containing a UnitType to number mapping of UnitTypes required.
    const std::map< UnitType, int >& requiredUnits() const;

    /// <summary>Identifies the required TechType in order to create certain units.</summary>
    ///
    /// @note The only unit that requires a technology is the @Lurker, which needs @Lurker_Aspect.
    /// @returns TechType indicating the technology that must be researched in order to create this
    /// unit type.
    /// @retval TechTypes::None If creating this unit type does not require a technology to be
    /// researched.
    TechType requiredTech() const;
    
    /// <summary>Retrieves the cloaking technology associated with certain units.</summary>
    ///
    /// @returns TechType referring to the cloaking technology that this unit type uses as an
    /// ability.
    /// @retval TechTypes::None If this unit type does not have an active cloak ability.
    TechType cloakingTech() const;

    /// <summary>Retrieves the set of abilities that this unit can use, provided it is available to you in
    /// the game.</summary>
    ///
    /// @returns Set of TechTypes containing ability information.
    const SetContainer<TechType>& abilities() const;

    /// <summary>Retrieves the set of upgrades that this unit can use to enhance its fighting ability.</summary>
    ///
    /// @return Set of UpgradeTypes containing upgrade types that will impact this unit type.
    const SetContainer<UpgradeType>& upgrades() const;

    /// <summary>Retrieves the upgrade type used to increase the armor of this unit type.</summary> For each upgrade,
    /// this unit type gains +1 additional armor.
    ///
    /// @returns UpgradeType indicating the upgrade that increases this unit type's armor amount.
    UpgradeType armorUpgrade() const;

    /// <summary>Retrieves the upgrade type used to upgrade the weapon damage of this unit type.</summary>
    ///
    /// @returns UpgradeType indicating the upgrade that increases this unit type's weapon damage amount.
    /// @m_since{5,0,0}
    UpgradeType attackUpgrade() const;

    /// <summary>Retrieves the upgrade that increases this unit type's attack range.</summary>
    ///
    /// @returns UpgradeType indicating the upgrade that increases this unit type's attack range.
    /// @m_since{5,0,0}
    UpgradeType attackRangeUpgrade() const;

    /// <summary>Retrieves the default maximum amount of hit points that this unit type can have.</summary>
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Integer indicating the maximum amount of hit points for this unit type.
    int maxHitPoints() const;

    /// <summary>Retrieves the default maximum amount of shield points that this unit type can have.</summary>
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Integer indicating the maximum amount of shield points for this unit type.
    /// @retval 0 If this unit type does not have shields.
    int maxShields() const;

    /// <summary>Retrieves the maximum amount of energy this unit type can have by default.</summary>
    ///
    /// @returns Integer indicating the maximum amount of energy for this unit type.
    /// @retval 0 If this unit does not gain energy for abilities.
    int maxEnergy() const;

    /// <summary>Retrieves the default amount of armor that the unit type starts with, excluding upgrades.</summary>
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns The amount of armor the unit type has.
    int armor() const;

    /// <summary>Retrieves the default mineral price of purchasing the unit.</summary>
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Mineral cost of the unit.
    int mineralPrice() const;

    /// <summary>Retrieves the default vespene gas price of purchasing the unit.</summary>
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Vespene gas cost of the unit.
    int gasPrice() const;

    /// <summary>Retrieves the default time, in frames, needed to train, morph, or build the unit.</summary>
    ///
    /// @note This value may not necessarily match the value seen in the @UMS game type.
    ///
    /// @returns Number of frames needed in order to build the unit.
    /// @see Unit::getRemainingBuildTime
    int buildTime() const;

    /// <summary>Retrieves the amount of supply that this unit type will use when created.</summary> It will use the
    /// supply pool that is appropriate for its Race.
    ///
    /// @note In Starcraft programming, the managed supply values are double than what they appear
    /// in the game. The reason for this is because @Zerglings use 0.5 visible supply.
    ///
    /// @returns Integer containing the supply required to build this unit.
    /// @see supplyProvided, Player::supplyTotal, Player::supplyUsed
    int supplyRequired() const;

    /// <summary>Retrieves the amount of supply that this unit type produces for its appropriate Race's
    /// supply pool.</summary>
    ///
    /// @note In Starcraft programming, the managed supply values are double than what they appear
    /// in the game. The reason for this is because @Zerglings use 0.5 visible supply.
    ///
    /// @see supplyRequired, Player::supplyTotal, Player::supplyUsed
    int supplyProvided() const;

    /// <summary>Retrieves the amount of space required by this unit type to fit inside a @Bunker or
    /// @Transport.</summary>
    ///
    /// @returns Amount of space required by this unit type for transport.
    /// @retval 255 If this unit type can not be transported.
    /// @see spaceProvided
    int spaceRequired() const;

    /// <summary>Retrieves the amount of space provided by this @Bunker or @Transport for unit
    /// transportation.</summary>
    /// 
    /// @returns The number of slots provided by this unit type.
    /// @see spaceRequired
    int spaceProvided() const;

    /// <summary>Retrieves the amount of score points awarded for constructing this unit type.</summary> This value is
    /// used for calculating scores in the post-game score screen.
    ///
    /// @returns Number of points awarded for constructing this unit type.
    /// @see destroyScore
    int buildScore() const;

    /// <summary>Retrieves the amount of score points awarded for killing this unit type.</summary> This value is
    /// used for calculating scores in the post-game score screen.
    ///
    /// @returns Number of points awarded for killing this unit type.
    /// @see buildScore
    int destroyScore() const;

    /// <summary>Retrieves the UnitSizeType of this unit, which is used in calculations along with weapon
    /// damage types to determine the amount of damage that will be dealt to this type.</summary>
    ///
    /// @returns UnitSizeType indicating the conceptual size of the unit type.
    /// @see WeaponType::damageType
    UnitSizeType size() const;

    /// <summary>Retrieves the width of this unit type, in tiles.</summary> Used for determining the tile size of
    /// structures.
    ///
    /// @returns Width of this unit type, in tiles.
    int tileWidth() const;

    /// <summary>Retrieves the height of this unit type, in tiles.</summary> Used for determining the tile size of
    /// structures.
    ///
    /// @returns Height of this unit type, in tiles.
    int tileHeight() const;

    /// <summary>Retrieves the tile size of this unit type.</summary> Used for determining the tile size of
    /// structures.
    ///
    /// @returns TilePosition containing the width (x) and height (y) of the unit type, in tiles.
    TilePosition tileSize() const;

    /// <summary>Retrieves the distance from the center of the unit type to its left edge.</summary>
    ///
    /// @returns Distance to this unit type's left edge from its center, in pixels.
    int dimensionLeft() const;

    /// <summary>Retrieves the distance from the center of the unit type to its top edge.</summary>
    ///
    /// @returns Distance to this unit type's top edge from its center, in pixels.
    int dimensionUp() const;

    /// <summary>Retrieves the distance from the center of the unit type to its right edge.</summary>
    ///
    /// @returns Distance to this unit type's right edge from its center, in pixels.
    int dimensionRight() const;

    /// <summary>Retrieves the distance from the center of the unit type to its bottom edge.</summary>
    ///
    /// @returns Distance to this unit type's bottom edge from its center, in pixels.
    int dimensionDown() const;

    /// <summary>A macro for retrieving the width of the unit type, which is calculated using
    /// dimensionLeft + dimensionRight + 1.</summary>
    ///
    /// @returns Width of the unit, in pixels.
    int width() const;

    /// <summary>A macro for retrieving the height of the unit type, which is calculated using
    /// dimensionUp + dimensionDown + 1.</summary>
    ///
    /// @returns Height of the unit, in pixels.
    int height() const;

    /// <summary>Retrieves the range at which this unit type will start targeting enemy units.</summary>
    ///
    /// @returns Distance at which this unit type begins to seek out enemy units, in pixels.
    int seekRange() const;

    /// <summary>Retrieves the sight range of this unit type.</summary>
    ///
    /// @returns Sight range of this unit type, measured in pixels.
    int sightRange() const;

    /// <summary>Retrieves this unit type's weapon type used when attacking targets on the ground.</summary>
    ///
    /// @returns WeaponType used as this unit type's ground weapon.
    /// @see maxGroundHits, airWeapon
    WeaponType groundWeapon() const;

    /// <summary>Retrieves the maximum number of hits this unit can deal to a ground target using its
    /// ground weapon.</summary> This value is multiplied by the ground weapon's damage to calculate the
    /// unit type's damage potential.
    ///
    /// @returns Maximum number of hits given to ground targets.
    /// @see groundWeapon, maxAirHits
    int maxGroundHits() const;

    /// <summary>Retrieves this unit type's weapon type used when attacking targets in the air.</summary>
    ///
    /// @returns WeaponType used as this unit type's air weapon.
    /// @see maxAirHits, groundWeapon
    WeaponType airWeapon() const;

    /// <summary>Retrieves the maximum number of hits this unit can deal to a flying target using its
    /// air weapon.</summary> This value is multiplied by the air weapon's damage to calculate the
    /// unit type's damage potential.
    ///
    /// @returns Maximum number of hits given to air targets.
    /// @see airWeapon, maxGroundHits
    int maxAirHits() const;

    /// <summary>Retrieves this unit type's top movement speed with no upgrades.</summary>
    ///
    /// @note That some units have inconsistent movement and this value is sometimes an
    /// approximation.
    ///
    /// @returns The approximate top speed, in pixels per frame, as a double. For liftable @Terran
    /// structures, this function returns their movement speed while lifted.
    double topSpeed() const;

    /// <summary>Retrieves the unit's acceleration amount.</summary>
    ///
    /// @returns If the value is 1: The unit has fixed or patterned velocity (for example, a
    /// hopping Zergling). Otherwise, returns the unit's acceleration measured in N/256 pixels
    /// per frame per frame. So a unit with 64 acceleration would take 4 frames to reach 1
    /// pixel per frame.
    int acceleration() const;

    /// <summary>Retrieves the unit's halting distance.</summary> This is the distance from
    /// the unit's destination at which point the unit starts decelerating.
    ///
    /// @returns The unit's halting distance, in 1/256ths of a pixel.
    int haltDistance() const;

    /// <summary>Retrieves a unit's turning rate.</summary> This determines how far a unit can
    /// rotate in one frame.
    /// 
    /// @returns The units are N/256ths of a circle per frame. So a unit with a turnRadius of 64
    /// can turn a full circle in 4 frames.
    int turnRadius() const;

    /// <summary>Determines if a unit can train other units.</summary> For example,
    /// UnitTypes::Terran_Barracks.canProduce() will return true, while
    /// UnitTypes::Terran_Marine.canProduce() will return false. This is also true for two
    /// non-structures: @Carrier (can produce interceptors) and @Reaver (can produce scarabs).
    ///
    /// @returns true if this unit type can have a production queue, and false otherwise.
    bool canProduce() const;

    /// <summary>Checks if this unit is capable of attacking.</summary>
    ///
    /// @note This function returns false for units that can only inflict damage via special
    /// abilities, such as the @High_Templar.
    ///
    /// @returns true if this unit type is capable of damaging other units with a standard attack,
    /// and false otherwise.
    bool canAttack() const;

    /// <summary>Checks if this unit type is capable of movement.</summary>
    ///
    /// @note Buildings will return false, including @Terran liftable buildings which are capable
    /// of moving when lifted.
    ///
    /// @returns true if this unit can use a movement command, and false if they cannot move.
    bool canMove() const;

    /// <summary>Checks if this unit type is a flying unit. Flying units ignore ground pathing and
    /// collisions.</summary>
    ///
    /// @returns true if this unit type is in the air by default, and false otherwise.
    bool isFlyer() const;

    /// <summary>Checks if this unit type can regenerate hit points. This generally applies to @Zerg units.</summary>
    ///
    /// @returns true if this unit type regenerates its hit points, and false otherwise.
    bool regeneratesHP() const;

    /// <summary>Checks if this unit type has the capacity to store energy and use it for special abilities.</summary>
    ///
    /// @returns true if this unit type generates energy, and false if it does not have an energy
    /// pool.
    bool isSpellcaster() const;

    /// <summary>Checks if this unit type is permanently cloaked.</summary> This means the unit type is always
    /// cloaked and requires a detector in order to see it.
    ///
    /// @returns true if this unit type is permanently cloaked, and false otherwise.
    bool hasPermanentCloak() const;

    /// <summary>Checks if this unit type is invincible by default.</summary> Invincible units
    /// cannot take damage.
    ///
    /// @returns true if this unit type is invincible, and false if it is vulnerable to attacks.
    bool isInvincible() const;

    /// <summary>Checks if this unit is an organic unit.</summary> The organic property is required for some abilities
    /// such as @Heal.
    ///
    /// @returns true if this unit type has the organic property, and false otherwise.
    bool isOrganic() const;

    /// <summary>Checks if this unit is mechanical.</summary> The mechanical property is required for some actions
    /// such as @Repair.
    ///
    /// @returns true if this unit type has the mechanical property, and false otherwise.
    bool isMechanical() const;

    /// <summary>Checks if this unit is robotic.</summary> The robotic property is applied
    /// to robotic units such as the @Probe which prevents them from taking damage from
    /// @Irradiate.
    ///
    /// @returns true if this unit type has the robotic property, and false otherwise.
    bool isRobotic() const;

    /// <summary>Checks if this unit type is capable of detecting units that are cloaked or burrowed.</summary>
    ///
    /// @returns true if this unit type is a detector by default, false if it does not have this
    /// property
    bool isDetector() const;

    /// <summary>Checks if this unit type is capable of storing resources such as @minerals.</summary> Resources
    /// are harvested from resource containers.
    ///
    /// @returns true if this unit type may contain resources that can be harvested, false
    /// otherwise.
    bool isResourceContainer() const;

    /// <summary>Checks if this unit type is a resource depot.</summary> Resource depots must be placed a certain
    /// distance from resources. Resource depots are typically the main building for any
    /// particular race. Workers will return resources to the nearest resource depot.
    ///
    /// Example:
    /// @code{.cpp}
    ///   if ( game.self() )
    ///   {
    ///     for ( BWAPI::Unit u : game.self().getUnits() )
    ///     {
    ///       if ( u.isIdle() && u.getType().isResourceDepot() )
    ///         u.train( u.getType().getRace().getWorker() );
    ///     }
    ///   }
    /// @endcode
    /// @returns true if the unit type is a resource depot, false if it is not.
    bool isResourceDepot() const;

    /// <summary>Checks if this unit type is a refinery.</summary> A refinery is a structure that is placed on top of
    /// a @geyser . Refinery types are @refinery , @extractor , and @assimilator.
    ///
    /// Example:
    /// @code{.cpp}
    ///   if ( game.self() )
    ///   {
    ///     for ( BWAPI::Unit u : game.self().getUnits() )
    ///     {
    ///       if ( u.getType().isRefinery() )
    ///       {
    ///         int nWorkersAssigned = refineryWorkerCount[u];
    ///         if ( nWorkersAssigned < 3 )
    ///         {
    ///           Unit closestIdleWorker = u.getClosestUnit(BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle);
    ///           if ( closestIdleWorker )
    ///           {
    ///             // gather from the refinery (and check if successful)
    ///             if ( closestIdleWorker.gather(u) )
    ///             {
    ///               // set a back reference for when the unit is killed or re-assigned (code not provided)
    ///               workerRefineryMap[closestIdleWorker] = u;
    ///
    ///               // Increment the number of workers assigned and associate it with the refinery
    ///               ++nWorkersAssigned;
    ///               refineryWorkerCount[u] = nWorkersAssigned;
    ///             }
    ///           }
    ///         } // workers < 3
    ///       } // isRefinery
    ///     } // for
    ///   }
    /// @endcode
    /// @returns true if this unit type is a refinery, and false if it is not.
    bool isRefinery() const;

    /// <summary>Checks if this unit type is a worker unit.</summary> Worker units can harvest resources and build
    /// structures. Worker unit types include the @SCV , @probe, and @drone.
    ///
    /// @returns true if this unit type is a worker, and false if it is not.
    bool isWorker() const;

    /// <summary>Checks if this structure is powered by a psi field.</summary> Structures powered
    /// by psi can only be placed near a @Pylon. If the @Pylon is destroyed, then this unit will
    /// lose power.
    ///
    /// @returns true if this unit type can only be placed in a psi field, false otherwise.
    /// @implies isBuilding(), getRace() == Races::Protoss
    bool requiresPsi() const;

    /// <summary>Checks if this structure must be placed on @Zerg creep.</summary>
    ///
    /// @returns true if this unit type requires creep, false otherwise.
    /// @implies isBuilding(), getRace() == Races::Zerg
    bool requiresCreep() const;

    /// <summary>Checks if this unit type spawns two units when being hatched from an @Egg.</summary>
    /// This is only applicable to @Zerglings and @Scourges.
    ///
    /// @returns true if morphing this unit type will spawn two of them, and false if only one
    /// is spawned.
    bool isTwoUnitsInOneEgg() const;

    /// <summary>Checks if this unit type has the capability to use the @Burrow technology when it
    /// is researched.</summary>
    ///
    /// @note The @Lurker can burrow even without researching the ability.
    /// @see TechTypes::Burrow
    /// @returns true if this unit can use the @Burrow ability, and false otherwise.
    /// @implies getRace() == Races::Zerg, !isBuilding(), canMove()
    bool isBurrowable() const;

    /// <summary>Checks if this unit type has the capability to use a cloaking ability when it
    /// is researched.</summary> This applies only to @Wraiths and @Ghosts, and does not include
    /// units which are permanently cloaked.
    ///
    /// @returns true if this unit has a cloaking ability, false otherwise.
    /// @see hasPermanentCloak, TechTypes::Cloaking_Field, TechTypes::Personnel_Cloaking
    bool isCloakable() const;

    /// <summary>Checks if this unit is a structure.</summary> This includes @Mineral_Fields and
    /// @Vespene_Geysers.
    ///
    /// @returns true if this unit is a building, and false otherwise.
    bool isBuilding() const;

    /// <summary>Checks if this unit is an add-on.</summary> Add-ons are attachments used by some
    /// @Terran structures such as the @Comsat_Station.
    ///
    /// @returns true if this unit is an add-on, and false otherwise.
    /// @implies getRace() == Races::Terran, isBuilding()
    bool isAddon() const;

    /// <summary>Checks if this structure has the capability to use the lift-off command.</summary>
    ///
    /// @returns true if this unit type is a flyable building, false otherwise.
    /// @implies isBuilding()
    bool isFlyingBuilding() const;

    /// <summary>Checks if this unit type is a neutral type, such as critters and resources.</summary>
    ///
    /// @returns true if this unit is intended to be neutral, and false otherwise.
    bool isNeutral() const;

    /// <summary>Checks if this unit type is a hero.</summary> Heroes are types that the player
    /// cannot obtain normally, and are identified by the white border around their icon when
    /// selected with a group.
    ///
    /// @note There are two non-hero units included in this set, the @Civilian and @Dark_Templar_Hero.
    ///
    /// @returns true if this unit type is a hero type, and false otherwise.
    bool isHero() const;

    /// <summary>Checks if this unit type is a powerup.</summary> Powerups can be picked up and
    /// carried by workers. They are usually only seen in campaign maps and @Capture_the_flag.
    ///
    /// @returns true if this unit type is a powerup type, and false otherwise.
    bool isPowerup() const;

    /// <summary>Checks if this unit type is a beacon.</summary> Each race has exactly one beacon
    /// each. They are UnitTypes::Special_Zerg_Beacon, UnitTypes::Special_Terran_Beacon, and
    /// UnitTypes::Special_Protoss_Beacon.
    ///
    /// @see isFlagBeacon
    /// @returns true if this unit type is one of the three race beacons, and false otherwise.
    bool isBeacon() const;

    /// <summary>Checks if this unit type is a flag beacon.</summary> Each race has exactly one
    /// flag beacon each. They are UnitTypes::Special_Zerg_Flag_Beacon,
    /// UnitTypes::Special_Terran_Flag_Beacon, and UnitTypes::Special_Protoss_Flag_Beacon.
    /// Flag beacons spawn a @Flag on frame 9604.
    ///
    /// @see isBeacon
    /// @returns true if this unit type is one of the three race flag beacons, and false otherwise.
    bool isFlagBeacon() const;

    /// <summary>Checks if this structure is special and cannot be obtained normally within the
    /// game.</summary>
    ///
    /// @returns true if this structure is a special building, and false otherwise.
    /// @implies isBuilding()
    bool isSpecialBuilding() const;

    /// <summary>Identifies if this unit type is used to complement some @abilities.</summary>
    /// These include UnitTypes::Spell_Dark_Swarm, UnitTypes::Spell_Disruption_Web, and
    /// UnitTypes::Spell_Scanner_Sweep, which correspond to TechTypes::Dark_Swarm,
    /// TechTypes::Disruption_Web, and TechTypes::Scanner_Sweep respectively.
    ///
    /// @returns true if this unit type is used for an ability, and false otherwise.
    bool isSpell() const;

    /// <summary>Checks if this structure type produces creep.</summary> That is, the unit type
    /// spreads creep over a wide area so that @Zerg structures can be placed on it.
    ///
    /// @returns true if this unit type spreads creep.
    /// @implies getRace() == Races::Zerg, isBuilding()
    ///
    /// @m_since{4,1,2}
    bool producesCreep() const;

    /// <summary>Checks if this unit type produces larva.</summary> This is essentially used to
    /// check if the unit type is a @Hatchery, @Lair, or @Hive.
    ///
    /// @returns true if this unit type produces larva.
    /// @implies getRace() == Races::Zerg, isBuilding()
    bool producesLarva() const;

    /// <summary>Checks if this unit type is a mineral field and contains a resource amount.</summary>
    /// This indicates that the unit type is either UnitTypes::Resource_Mineral_Field, 
    /// UnitTypes::Resource_Mineral_Field_Type_2, or UnitTypes::Resource_Mineral_Field_Type_3.
    ///
    /// @returns true if this unit type is a mineral field resource.
    bool isMineralField() const;

    /// <summary>Checks if this unit type is a neutral critter.</summary>
    ///
    /// @returns true if this unit type is a critter, and false otherwise.
    ///
    /// Example usage:
    /// @code{.cpp}
    ///   BWAPI::Position myBasePosition( game.self().getStartLocation() );
    ///   BWAPI::UnitSet unitsAroundTheBase = game.getUnitsInRadius(myBasePosition, 1024, !BWAPI::Filter::IsOwned && !BWAPI::Filter::IsParasited);
    ///   for ( BWAPI::Unit u : unitsAroundTheBase )
    ///   {
    ///     if ( u.getType().isCritter() && !u.isInvincible() )
    ///     {
    ///       BWAPI::Unit myQueen = u.getClosestUnit(BWAPI::Filter::GetType == BWAPI::UnitTypes::Zerg_Queen && BWAPI::Filter::IsOwned);
    ///       if ( myQueen )
    ///         myQueen.useTech(BWAPI::TechTypes::Parasite, u);
    ///     }
    ///   }
    /// @endcode
    bool isCritter() const;

    /// <summary>Checks if this unit type is capable of constructing an add-on.</summary> An add-on is an extension
    /// or attachment for @Terran structures, specifically the @Command_Center, @Factory,
    /// @Starport, and @Science_Facility.
    ///
    /// @returns true if this unit type can construct an add-on, and false if it can not.
    /// @see isAddon
    bool canBuildAddon() const;

    /// <summary>Retrieves the set of units that this unit type is capable of creating.</summary>
    /// This includes training, constructing, warping, and morphing.
    ///
    /// @note Some maps have special parameters that disable construction of units that are otherwise
    /// normally available. Use Player::isUnitAvailable to determine if a unit type is
    /// actually available in the current game for a specific player.
    ///
    /// @returns UnitType::set containing the units it can build.
    /// @see Player::isUnitAvailable
    ///
    /// @m_since{4,1,2}
    const UnitType::set& buildsWhat() const;

    /// <summary>Retrieves the set of technologies that this unit type is capable of researching.</summary>
    /// 
    /// @note Some maps have special parameters that disable certain technologies. Use
    /// Player::isResearchAvailable to determine if a technology is actually available in the
    /// current game for a specific player.
    ///
    /// @returns TechType::set containing the technology types that can be researched.
    /// @see Player::isResearchAvailable
    ///
    /// @m_since{4,1,2}
    const SetContainer<TechType>& researchesWhat() const;

    /// <summary>Retrieves the set of upgrades that this unit type is capable of upgrading.</summary>
    ///
    /// @note Some maps have special upgrade limitations. Use Player::getMaxUpgradeLevel
    /// to check if an upgrade is available.
    ///
    /// @returns UpgradeType::set containing the upgrade types that can be upgraded.
    /// @see Player::getMaxUpgradeLevel
    ///
    /// @m_since{4,1,2}
    const SetContainer<UpgradeType>& upgradesWhat() const;

    /// <summary>Checks if the current type is equal to the provided type, or a successor of the
    /// provided type.</summary> For example, a Hive is a successor of a Hatchery, since it can
    /// still research the @Burrow technology.
    ///
    /// <param name="type">
    ///   The unit type to check.
    /// </param>
    ///
    /// @see TechType::whatResearches, UpgradeType::whatUpgrades
    /// @m_since{4,2,0}
    bool isSuccessorOf(UnitType type) const;

    /// <summary>Retrieves this unit type's upgrade type for upgrading speed.</summary>
    ///
    /// @returns UpgradeType used as this unit type's ground weapon.
    /// @m_since{5,0,0}
    UpgradeType speedUpgrade() const;
  };

  /// <summary>The amount of shield points that a unit recovers over 256 frames.</summary>
  /// That is, 7/256 shields are regenerated per frame.
  ///
  /// @m_since{4,1,0}
  static const int SHIELD_REGEN_RATE = 7;

  /// <summary>The amount of energy that a unit with special abilities recovers over 256 frames.</summary>
  /// That is, 8/256 energy is generated per frame.
  ///
  /// @m_since{4,1,0}
  static const int ENERGY_REGEN_RATE = 8;

  /// <summary>The amount of life that a zerg unit recovers over 256 frames.</summary>
  /// That is, 4/256 life is regenerated per frame.
  ///
  /// @m_since{4,1,0}
  static const int LIFE_REGEN_RATE = 4;

  /// @ingroup Types
  namespace UnitTypes
  {
    /// <summary>Retrieves the maximum unit width from the set of all units.</summary> Used
    /// internally to search through unit positions efficiently.
    ///
    /// @returns The maximum width of all unit types, in pixels.
    int maxUnitWidth();
    
    /// <summary>Retrieves the maximum unit height from the set of all units.</summary> Used
    /// internally to search through unit positions efficiently.
    ///
    /// @returns The maximum height of all unit types, in pixels.
    int maxUnitHeight();

    /// <summary>Retrieves the set of all defined unit types.</summary>
    ///
    /// @returns A constant set of all available unit types.
    const UnitType::set& allUnitTypes();

    /// <summary>Retrieves the set of all macro unit types.</summary> A macro type is a fake unit
    /// type, used by some functions. These include #AllUnits, #Men, #Buildings, and #Factories.
    /// The purpose of these types are to match the same ones used in Broodwar, also seen in the
    /// StarEdit map editor.
    ///
    /// @returns A constant set of all macro unit types.
    const UnitType::set& allMacroTypes();

    /// @name Terran Ground Units
    /// @{
    constexpr UnitType Terran_Firebat{Enum::Terran_Firebat};
    constexpr UnitType Terran_Ghost{Enum::Terran_Ghost};
    constexpr UnitType Terran_Goliath{Enum::Terran_Goliath};
    /// @details @Terran tier 1 unit.
    constexpr UnitType Terran_Marine{Enum::Terran_Marine};
    constexpr UnitType Terran_Medic{Enum::Terran_Medic};
    /// @details @Terran worker.
    /// @see Race::getWorker
    constexpr UnitType Terran_SCV{Enum::Terran_SCV};
    constexpr UnitType Terran_Siege_Tank_Siege_Mode{Enum::Terran_Siege_Tank_Siege_Mode};
    constexpr UnitType Terran_Siege_Tank_Tank_Mode{Enum::Terran_Siege_Tank_Tank_Mode};
    constexpr UnitType Terran_Vulture{Enum::Terran_Vulture};
    constexpr UnitType Terran_Vulture_Spider_Mine{Enum::Terran_Vulture_Spider_Mine};
    /// @}
    /// @name Terran Air Units
    /// @{
    constexpr UnitType Terran_Battlecruiser{Enum::Terran_Battlecruiser};
    /// @details @Terran transport.
    /// @see Race::getTransport
    constexpr UnitType Terran_Dropship{Enum::Terran_Dropship};
    constexpr UnitType Terran_Nuclear_Missile{Enum::Terran_Nuclear_Missile};
    constexpr UnitType Terran_Science_Vessel{Enum::Terran_Science_Vessel};
    constexpr UnitType Terran_Valkyrie{Enum::Terran_Valkyrie};
    constexpr UnitType Terran_Wraith{Enum::Terran_Wraith};
    /// @}
    /// @name Terran Heroes
    /// @{
    constexpr UnitType Hero_Alan_Schezar{Enum::Hero_Alan_Schezar};
    constexpr UnitType Hero_Alexei_Stukov{Enum::Hero_Alexei_Stukov};
    constexpr UnitType Hero_Arcturus_Mengsk{Enum::Hero_Arcturus_Mengsk};
    constexpr UnitType Hero_Edmund_Duke_Tank_Mode{Enum::Hero_Edmund_Duke_Tank_Mode};
    constexpr UnitType Hero_Edmund_Duke_Siege_Mode{Enum::Hero_Edmund_Duke_Siege_Mode};
    constexpr UnitType Hero_Gerard_DuGalle{Enum::Hero_Gerard_DuGalle};
    constexpr UnitType Hero_Gui_Montag{Enum::Hero_Gui_Montag};
    constexpr UnitType Hero_Hyperion{Enum::Hero_Hyperion};
    constexpr UnitType Hero_Jim_Raynor_Marine{Enum::Hero_Jim_Raynor_Marine};
    constexpr UnitType Hero_Jim_Raynor_Vulture{Enum::Hero_Jim_Raynor_Vulture};
    constexpr UnitType Hero_Magellan{Enum::Hero_Magellan};
    constexpr UnitType Hero_Norad_II{Enum::Hero_Norad_II};
    constexpr UnitType Hero_Samir_Duran{Enum::Hero_Samir_Duran};
    constexpr UnitType Hero_Sarah_Kerrigan{Enum::Hero_Sarah_Kerrigan};
    constexpr UnitType Hero_Tom_Kazansky{Enum::Hero_Tom_Kazansky};
    constexpr UnitType Terran_Civilian{Enum::Terran_Civilian};
    /// @}
    /// @name Terran Buildings
    /// @{
    constexpr UnitType Terran_Academy{Enum::Terran_Academy};
    constexpr UnitType Terran_Armory{Enum::Terran_Armory};
    constexpr UnitType Terran_Barracks{Enum::Terran_Barracks};
    /// @details @Terran defensive structure.
    constexpr UnitType Terran_Bunker{Enum::Terran_Bunker};
    /// @details @Terran resource depot.
    /// @see Race::getCenter
    constexpr UnitType Terran_Command_Center{Enum::Terran_Command_Center};
    constexpr UnitType Terran_Engineering_Bay{Enum::Terran_Engineering_Bay};
    constexpr UnitType Terran_Factory{Enum::Terran_Factory};
    /// @details @Terran defensive structure against air units.
    constexpr UnitType Terran_Missile_Turret{Enum::Terran_Missile_Turret};
    /// @details @Terran gas harvesting structure. Constructed over a @Vespene_Geyser.
    /// @note When this is constructed over a @Vespene_Geyser, the unit type of the geyser is replaced
    /// with this one. When it is destroyed, then this type is replaced by the @Vespene_Geyser.
    /// @see Race::getRefinery, UnitTypes::Resource_Vespene_Geyser
    constexpr UnitType Terran_Refinery{Enum::Terran_Refinery};
    constexpr UnitType Terran_Science_Facility{Enum::Terran_Science_Facility};
    constexpr UnitType Terran_Starport{Enum::Terran_Starport};
    /// @details @Terran supply provider.
    /// @see Race::getSupplyProvider
    constexpr UnitType Terran_Supply_Depot{Enum::Terran_Supply_Depot};
    /// @}
    /// @name Terran Addons
    /// @{
    constexpr UnitType Terran_Comsat_Station{Enum::Terran_Comsat_Station};
    constexpr UnitType Terran_Control_Tower{Enum::Terran_Control_Tower};
    constexpr UnitType Terran_Covert_Ops{Enum::Terran_Covert_Ops};
    constexpr UnitType Terran_Machine_Shop{Enum::Terran_Machine_Shop};
    constexpr UnitType Terran_Nuclear_Silo{Enum::Terran_Nuclear_Silo};
    constexpr UnitType Terran_Physics_Lab{Enum::Terran_Physics_Lab};
    /// @}
    /// @name Terran Special Buildings
    /// @{
    constexpr UnitType Special_Crashed_Norad_II{Enum::Special_Crashed_Norad_II};
    constexpr UnitType Special_Ion_Cannon{Enum::Special_Ion_Cannon};
    constexpr UnitType Special_Power_Generator{Enum::Special_Power_Generator};
    constexpr UnitType Special_Psi_Disrupter{Enum::Special_Psi_Disrupter};
    /// @}
    /// @name Protoss Ground Units
    /// @{
    constexpr UnitType Protoss_Archon{Enum::Protoss_Archon};
    constexpr UnitType Protoss_Dark_Archon{Enum::Protoss_Dark_Archon};
    constexpr UnitType Protoss_Dark_Templar{Enum::Protoss_Dark_Templar};
    constexpr UnitType Protoss_Dragoon{Enum::Protoss_Dragoon};
    constexpr UnitType Protoss_High_Templar{Enum::Protoss_High_Templar};
    /// @details @Protoss worker unit.
    /// @see Race::getWorker
    constexpr UnitType Protoss_Probe{Enum::Protoss_Probe};
    constexpr UnitType Protoss_Reaver{Enum::Protoss_Reaver};
    constexpr UnitType Protoss_Scarab{Enum::Protoss_Scarab};
    /// @Protoss tier 1 unit.
    constexpr UnitType Protoss_Zealot{Enum::Protoss_Zealot};
    /// @}
    /// @name Protoss Air Units
    /// @{
    constexpr UnitType Protoss_Arbiter{Enum::Protoss_Arbiter};
    constexpr UnitType Protoss_Carrier{Enum::Protoss_Carrier};
    constexpr UnitType Protoss_Corsair{Enum::Protoss_Corsair};
    constexpr UnitType Protoss_Interceptor{Enum::Protoss_Interceptor};
    constexpr UnitType Protoss_Observer{Enum::Protoss_Observer};
    constexpr UnitType Protoss_Scout{Enum::Protoss_Scout};
    /// @details @Protoss transport.
    /// @see Race::getTransport
    constexpr UnitType Protoss_Shuttle{Enum::Protoss_Shuttle};
    /// @}
    /// @name Protoss Heroes Units
    /// @{
    constexpr UnitType Hero_Aldaris{Enum::Hero_Aldaris};
    constexpr UnitType Hero_Artanis{Enum::Hero_Artanis};
    constexpr UnitType Hero_Danimoth{Enum::Hero_Danimoth};
    constexpr UnitType Hero_Dark_Templar{Enum::Hero_Dark_Templar};
    constexpr UnitType Hero_Fenix_Dragoon{Enum::Hero_Fenix_Dragoon};
    constexpr UnitType Hero_Fenix_Zealot{Enum::Hero_Fenix_Zealot};
    constexpr UnitType Hero_Gantrithor{Enum::Hero_Gantrithor};
    constexpr UnitType Hero_Mojo{Enum::Hero_Mojo};
    constexpr UnitType Hero_Raszagal{Enum::Hero_Raszagal};
    constexpr UnitType Hero_Tassadar{Enum::Hero_Tassadar};
    constexpr UnitType Hero_Tassadar_Zeratul_Archon{Enum::Hero_Tassadar_Zeratul_Archon};
    constexpr UnitType Hero_Warbringer{Enum::Hero_Warbringer};
    constexpr UnitType Hero_Zeratul{Enum::Hero_Zeratul};
    /// @}
    /// @name Protoss Buildings
    /// @{
    constexpr UnitType Protoss_Arbiter_Tribunal{Enum::Protoss_Arbiter_Tribunal};
    /// @details @Protoss gas harvesting structure. Must be placed on top of a @Vespene_Geyser.
    /// @note When this is constructed over a @Vespene_Geyser, the unit type of the geyser is replaced
    /// with this one. When it is destroyed, then this type is replaced by the @Vespene_Geyser.
    /// @see Race::getRefinery, UnitTypes::Resource_Vespene_Geyser
    constexpr UnitType Protoss_Assimilator{Enum::Protoss_Assimilator};
    constexpr UnitType Protoss_Citadel_of_Adun{Enum::Protoss_Citadel_of_Adun};
    constexpr UnitType Protoss_Cybernetics_Core{Enum::Protoss_Cybernetics_Core};
    constexpr UnitType Protoss_Fleet_Beacon{Enum::Protoss_Fleet_Beacon};
    constexpr UnitType Protoss_Forge{Enum::Protoss_Forge};
    constexpr UnitType Protoss_Gateway{Enum::Protoss_Gateway};
    /// @details @Protoss resource depot.
    /// @see Race::getCenter
    constexpr UnitType Protoss_Nexus{Enum::Protoss_Nexus};
    constexpr UnitType Protoss_Observatory{Enum::Protoss_Observatory};
    /// @details @Protoss defensive structure.
    constexpr UnitType Protoss_Photon_Cannon{Enum::Protoss_Photon_Cannon};
    /// @details @Protoss supply provider. Required to place other @Protoss structures.
    /// @see Race::getSupplyProvider
    constexpr UnitType Protoss_Pylon{Enum::Protoss_Pylon};
    constexpr UnitType Protoss_Robotics_Facility{Enum::Protoss_Robotics_Facility};
    constexpr UnitType Protoss_Robotics_Support_Bay{Enum::Protoss_Robotics_Support_Bay};
    /// @details A structure that can recharge @Protoss_Shields.
    constexpr UnitType Protoss_Shield_Battery{Enum::Protoss_Shield_Battery};
    constexpr UnitType Protoss_Stargate{Enum::Protoss_Stargate};
    constexpr UnitType Protoss_Templar_Archives{Enum::Protoss_Templar_Archives};
    /// @}
    /// @name Protoss Special Buildings
    /// @{
    constexpr UnitType Special_Khaydarin_Crystal_Form{Enum::Special_Khaydarin_Crystal_Form};
    constexpr UnitType Special_Protoss_Temple{Enum::Special_Protoss_Temple};
    constexpr UnitType Special_Stasis_Cell_Prison{Enum::Special_Stasis_Cell_Prison};
    constexpr UnitType Special_Warp_Gate{Enum::Special_Warp_Gate};
    constexpr UnitType Special_XelNaga_Temple{Enum::Special_XelNaga_Temple};
    /// @}
    /// @name Zerg Ground Units
    /// @{
    constexpr UnitType Zerg_Broodling{Enum::Zerg_Broodling};
    constexpr UnitType Zerg_Defiler{Enum::Zerg_Defiler};
    /// @details @Zerg worker unit.
    /// @see Race::getWorker
    constexpr UnitType Zerg_Drone{Enum::Zerg_Drone};
    constexpr UnitType Zerg_Egg{Enum::Zerg_Egg};
    constexpr UnitType Zerg_Hydralisk{Enum::Zerg_Hydralisk};
    constexpr UnitType Zerg_Infested_Terran{Enum::Zerg_Infested_Terran};
    /// @details Spawned at @Hatcheries.
    constexpr UnitType Zerg_Larva{Enum::Zerg_Larva};
    constexpr UnitType Zerg_Lurker{Enum::Zerg_Lurker};
    constexpr UnitType Zerg_Lurker_Egg{Enum::Zerg_Lurker_Egg};
    constexpr UnitType Zerg_Ultralisk{Enum::Zerg_Ultralisk};
    /// @Zerg tier 1 unit.
    constexpr UnitType Zerg_Zergling{Enum::Zerg_Zergling};
    /// @}
    /// @name Zerg Air Units
    /// @{
    constexpr UnitType Zerg_Cocoon{Enum::Zerg_Cocoon};
    constexpr UnitType Zerg_Devourer{Enum::Zerg_Devourer};
    constexpr UnitType Zerg_Guardian{Enum::Zerg_Guardian};
    constexpr UnitType Zerg_Mutalisk{Enum::Zerg_Mutalisk};
    /// @details @Zerg supply provider and transport unit. Must research @Ventral_Sacs to become a transport.
    /// @see Race::getSupplyProvider, Race::getTransport
    constexpr UnitType Zerg_Overlord{Enum::Zerg_Overlord};
    constexpr UnitType Zerg_Queen{Enum::Zerg_Queen};
    constexpr UnitType Zerg_Scourge{Enum::Zerg_Scourge};
    /// @}
    /// @name Zerg Heroes
    /// @{
    constexpr UnitType Hero_Devouring_One{Enum::Hero_Devouring_One};
    constexpr UnitType Hero_Hunter_Killer{Enum::Hero_Hunter_Killer};
    constexpr UnitType Hero_Infested_Duran{Enum::Hero_Infested_Duran};
    constexpr UnitType Hero_Infested_Kerrigan{Enum::Hero_Infested_Kerrigan};
    constexpr UnitType Hero_Kukulza_Guardian{Enum::Hero_Kukulza_Guardian};
    constexpr UnitType Hero_Kukulza_Mutalisk{Enum::Hero_Kukulza_Mutalisk};
    constexpr UnitType Hero_Matriarch{Enum::Hero_Matriarch};
    constexpr UnitType Hero_Torrasque{Enum::Hero_Torrasque};
    constexpr UnitType Hero_Unclean_One{Enum::Hero_Unclean_One};
    constexpr UnitType Hero_Yggdrasill{Enum::Hero_Yggdrasill};
    /// @}
    /// @name Zerg Buildings
    /// @{
    /// @details Spreads creep and morphs to @Zerg defensive structures.
    constexpr UnitType Zerg_Creep_Colony{Enum::Zerg_Creep_Colony};
    constexpr UnitType Zerg_Defiler_Mound{Enum::Zerg_Defiler_Mound};
    constexpr UnitType Zerg_Evolution_Chamber{Enum::Zerg_Evolution_Chamber};
    /// @details @Zerg gas harvesting structure. Must be constructed on a @Vespene_Geyser.
    /// @note When this is constructed over a @Vespene_Geyser, the unit type of the geyser is replaced
    /// with this one. When it is destroyed, then this type is replaced by the @Vespene_Geyser.
    /// @see Race::getRefinery, UnitTypes::Resource_Vespene_Geyser
    constexpr UnitType Zerg_Extractor{Enum::Zerg_Extractor};
    constexpr UnitType Zerg_Greater_Spire{Enum::Zerg_Greater_Spire};
    /// @details @Zerg resource depot and larva generator. Spreads creep.
    /// @see Race::getCenter
    constexpr UnitType Zerg_Hatchery{Enum::Zerg_Hatchery};
    constexpr UnitType Zerg_Hive{Enum::Zerg_Hive};
    constexpr UnitType Zerg_Hydralisk_Den{Enum::Zerg_Hydralisk_Den};
    /// @details @note Can land on both creep and non-creep.
    constexpr UnitType Zerg_Infested_Command_Center{Enum::Zerg_Infested_Command_Center};
    constexpr UnitType Zerg_Lair{Enum::Zerg_Lair};
    /// @details Used to teleport @Zerg ground units from one Nydus Canal to the other.
    constexpr UnitType Zerg_Nydus_Canal{Enum::Zerg_Nydus_Canal};
    constexpr UnitType Zerg_Queens_Nest{Enum::Zerg_Queens_Nest};
    constexpr UnitType Zerg_Spawning_Pool{Enum::Zerg_Spawning_Pool};
    constexpr UnitType Zerg_Spire{Enum::Zerg_Spire};
    /// @details @Zerg defensive structure against air units.
    constexpr UnitType Zerg_Spore_Colony{Enum::Zerg_Spore_Colony};
    /// @details @Zerg defensive structure against ground units.
    constexpr UnitType Zerg_Sunken_Colony{Enum::Zerg_Sunken_Colony};
    constexpr UnitType Zerg_Ultralisk_Cavern{Enum::Zerg_Ultralisk_Cavern};
    /// @}
    /// @name Zerg Special Buildings
    /// @{
    constexpr UnitType Special_Cerebrate{Enum::Special_Cerebrate};
    constexpr UnitType Special_Cerebrate_Daggoth{Enum::Special_Cerebrate_Daggoth};
    constexpr UnitType Special_Mature_Chrysalis{Enum::Special_Mature_Chrysalis};
    constexpr UnitType Special_Overmind{Enum::Special_Overmind};
    constexpr UnitType Special_Overmind_Cocoon{Enum::Special_Overmind_Cocoon};
    constexpr UnitType Special_Overmind_With_Shell{Enum::Special_Overmind_With_Shell};
    /// @}
    /// @name Critters
    /// @{
    constexpr UnitType Critter_Bengalaas{Enum::Critter_Bengalaas};
    constexpr UnitType Critter_Kakaru{Enum::Critter_Kakaru};
    constexpr UnitType Critter_Ragnasaur{Enum::Critter_Ragnasaur};
    constexpr UnitType Critter_Rhynadon{Enum::Critter_Rhynadon};
    constexpr UnitType Critter_Scantid{Enum::Critter_Scantid};
    constexpr UnitType Critter_Ursadon{Enum::Critter_Ursadon};
    /// @}
    /// @name Resources
    /// @{
    constexpr UnitType Resource_Mineral_Field{Enum::Resource_Mineral_Field};
    constexpr UnitType Resource_Mineral_Field_Type_2{Enum::Resource_Mineral_Field_Type_2};
    constexpr UnitType Resource_Mineral_Field_Type_3{Enum::Resource_Mineral_Field_Type_3};
    /// @details @note When a refinery unit is constructed over this type, a new unit is NOT created.
    /// Rather, the vespene geyser is replaced with the refinery unit. Similarily, when a refinery
    /// is destroyed, the type is replaced with the vespene geyser.
    constexpr UnitType Resource_Vespene_Geyser{Enum::Resource_Vespene_Geyser};
    /// @}
    /// @name Spells
    /// @{
    constexpr UnitType Spell_Dark_Swarm{Enum::Spell_Dark_Swarm};
    constexpr UnitType Spell_Disruption_Web{Enum::Spell_Disruption_Web};
    constexpr UnitType Spell_Scanner_Sweep{Enum::Spell_Scanner_Sweep};
    /// @}
    /// @name Beacons
    /// @{
    constexpr UnitType Special_Protoss_Beacon{Enum::Special_Protoss_Beacon};
    constexpr UnitType Special_Protoss_Flag_Beacon{Enum::Special_Protoss_Flag_Beacon};
    constexpr UnitType Special_Terran_Beacon{Enum::Special_Terran_Beacon};
    constexpr UnitType Special_Terran_Flag_Beacon{Enum::Special_Terran_Flag_Beacon};
    constexpr UnitType Special_Zerg_Beacon{Enum::Special_Zerg_Beacon};
    constexpr UnitType Special_Zerg_Flag_Beacon{Enum::Special_Zerg_Flag_Beacon};
    /// @}
    /// @name Powerups
    /// @{
    constexpr UnitType Powerup_Data_Disk{Enum::Powerup_Data_Disk};
    /// @details @note Can't be carried by your own workers.
    constexpr UnitType Powerup_Flag{Enum::Powerup_Flag};
    constexpr UnitType Powerup_Khalis_Crystal{Enum::Powerup_Khalis_Crystal};
    constexpr UnitType Powerup_Khaydarin_Crystal{Enum::Powerup_Khaydarin_Crystal};
    constexpr UnitType Powerup_Mineral_Cluster_Type_1{Enum::Powerup_Mineral_Cluster_Type_1};
    constexpr UnitType Powerup_Mineral_Cluster_Type_2{Enum::Powerup_Mineral_Cluster_Type_2};
    constexpr UnitType Powerup_Protoss_Gas_Orb_Type_1{Enum::Powerup_Protoss_Gas_Orb_Type_1};
    constexpr UnitType Powerup_Protoss_Gas_Orb_Type_2{Enum::Powerup_Protoss_Gas_Orb_Type_2};
    constexpr UnitType Powerup_Psi_Emitter{Enum::Powerup_Psi_Emitter};
    constexpr UnitType Powerup_Terran_Gas_Tank_Type_1{Enum::Powerup_Terran_Gas_Tank_Type_1};
    constexpr UnitType Powerup_Terran_Gas_Tank_Type_2{Enum::Powerup_Terran_Gas_Tank_Type_2};
    constexpr UnitType Powerup_Uraj_Crystal{Enum::Powerup_Uraj_Crystal};
    constexpr UnitType Powerup_Young_Chrysalis{Enum::Powerup_Young_Chrysalis};
    constexpr UnitType Powerup_Zerg_Gas_Sac_Type_1{Enum::Powerup_Zerg_Gas_Sac_Type_1};
    constexpr UnitType Powerup_Zerg_Gas_Sac_Type_2{Enum::Powerup_Zerg_Gas_Sac_Type_2};
    /// @}
    /// @name Traps
    /// @{
    constexpr UnitType Special_Floor_Gun_Trap{Enum::Special_Floor_Gun_Trap};
    constexpr UnitType Special_Floor_Missile_Trap{Enum::Special_Floor_Missile_Trap};
    constexpr UnitType Special_Right_Wall_Flame_Trap{Enum::Special_Right_Wall_Flame_Trap};
    constexpr UnitType Special_Right_Wall_Missile_Trap{Enum::Special_Right_Wall_Missile_Trap};
    constexpr UnitType Special_Wall_Flame_Trap{Enum::Special_Wall_Flame_Trap};
    constexpr UnitType Special_Wall_Missile_Trap{Enum::Special_Wall_Missile_Trap};
    /// @}
    /// @name Doors
    /// @{
    constexpr UnitType Special_Pit_Door{Enum::Special_Pit_Door};
    constexpr UnitType Special_Right_Pit_Door{Enum::Special_Right_Pit_Door};
    constexpr UnitType Special_Right_Upper_Level_Door{Enum::Special_Right_Upper_Level_Door};
    constexpr UnitType Special_Upper_Level_Door{Enum::Special_Upper_Level_Door};
    /// @}
    /// @name Special
    /// @{
    constexpr UnitType Special_Cargo_Ship{Enum::Special_Cargo_Ship};
    constexpr UnitType Special_Floor_Hatch{Enum::Special_Floor_Hatch};
    constexpr UnitType Special_Independant_Starport{Enum::Special_Independant_Starport};
    /// @details A special unit placed by the campaign editor that reveals a large area of the map.
    constexpr UnitType Special_Map_Revealer{Enum::Special_Map_Revealer};
    constexpr UnitType Special_Mercenary_Gunship{Enum::Special_Mercenary_Gunship};
    /// @details A special unit that indicates a player's starting location. The unit is removed at the
    /// beginning of the game and replaced with the race's resource depot.
    ///
    /// A special use case with BWAPI is passing this value to Game::canBuildHere.
    /// @see Game::canBuildHere
    constexpr UnitType Special_Start_Location{Enum::Special_Start_Location};
    /// @}

    constexpr UnitType None{Enum::None};
    /// A special macro type, used for retrieving statistics for all unit types.
    constexpr UnitType AllUnits{Enum::AllUnits};
    /// A special macro type, used for retrieving statistics for all movable, non-structure units.
    constexpr UnitType Men{Enum::Men};
    /// A special macro type, used for retrieving statistics for all structures.
    constexpr UnitType Buildings{Enum::Buildings};
    /// A special macro type, used for retrieving statistics for all structures that can produce units.
    constexpr UnitType Factories{Enum::Factories};
    constexpr UnitType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(UnitType) == sizeof(int), "Expected type to resolve to primitive size.");
} // namespace BWAPI

namespace std {
  template<>
  struct hash<BWAPI::UnitType> {
    auto operator()(BWAPI::UnitType id) const {
      return BWAPI::UnitType::Hash{}(id);
    }
  };
} // namespace std

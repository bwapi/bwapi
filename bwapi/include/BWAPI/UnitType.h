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

    };
  }
  /// <summary>The UnitType is used to get information about a particular type of unit, such as its cost,
  /// build time, weapon, hit points, abilities, etc.</summary>
  ///
  /// @see UnitInterface::getType, UnitTypes
  /// @ingroup TypeClasses
  class UnitType : public Type<UnitType, UnitTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UnitType(int id = UnitTypes::Enum::None);

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
    /// @see UnitInterface::getRemainingBuildTime
    int buildTime() const;

    /// <summary>Retrieves the amount of supply that this unit type will use when created.</summary> It will use the
    /// supply pool that is appropriate for its Race.
    ///
    /// @note In Starcraft programming, the managed supply values are double than what they appear
    /// in the game. The reason for this is because @Zerglings use 0.5 visible supply.
    ///
    /// @returns Integer containing the supply required to build this unit.
    /// @see supplyProvided, PlayerInterface::supplyTotal, PlayerInterface::supplyUsed
    int supplyRequired() const;

    /// <summary>Retrieves the amount of supply that this unit type produces for its appropriate Race's
    /// supply pool.</summary>
    ///
    /// @note In Starcraft programming, the managed supply values are double than what they appear
    /// in the game. The reason for this is because @Zerglings use 0.5 visible supply.
    ///
    /// @see supplyRequired, PlayerInterface::supplyTotal, PlayerInterface::supplyUsed
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
    /// @returns How fast the unit can accelerate to its top speed.
    ///
    /// @todo Figure out the units this quantity is measured in.
    int acceleration() const;

    /// <summary>Retrieves the unit's halting distance.</summary> This determines how fast a unit
    /// can stop moving.
    ///
    /// @returns A halting distance value.
    ///
    /// @todo Figure out the units this quantity is measured in.
    int haltDistance() const;

    /// <summary>Retrieves a unit's turning radius.</summary> This determines how fast a unit can
    /// turn.
    ///
    /// @returns A turn radius value.
    ///
    /// @todo Figure out the units this quantity is measured in.
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
    /// @code
    ///   if ( BWAPI::Broodwar->self() )
    ///   {
    ///     BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///     for ( auto u : myUnits )
    ///     {
    ///       if ( u->isIdle() && u->getType().isResourceDepot() )
    ///         u->train( u->getType().getRace().getWorker() );
    ///     }
    ///   }
    /// @endcode
    /// @returns true if the unit type is a resource depot, false if it is not.
    bool isResourceDepot() const;

    /// <summary>Checks if this unit type is a refinery.</summary> A refinery is a structure that is placed on top of
    /// a @geyser . Refinery types are @refinery , @extractor , and @assimilator.
    ///
    /// Example:
    /// @code
    ///   if ( BWAPI::Broodwar->self() )
    ///   {
    ///     BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///     for ( auto u : myUnits )
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
    ///             if ( pClosestIdleWorker->gather(u) )
    ///             {
    ///               // set a back reference for when the unit is killed or re-assigned (code not provided)
    ///               pClosestIdleWorker->setClientInfo(u, 'ref');
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
    /// Flag beacons spawn a @Flag after some ARBITRARY I FORGOT AMOUNT OF FRAMES.
    ///
    /// @see isBeacon
    /// @returns true if this unit type is one of the three race flag beacons, and false otherwise.
    ///
    /// @todo specify number of frames for flag spawner
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
    /// @since 4.1.2
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
    /// @code
    ///   BWAPI::Position myBasePosition( BWAPI::Broodwar->self()->getStartLocation() );
    ///   BWAPI::UnitSet unitsAroundTheBase = BWAPI::Broodwar->getUnitsInRadius(myBasePosition, 1024, !BWAPI::Filter::IsOwned && !BWAPI::Filter::IsParasited);
    ///   for ( auto u : unitsAroundTheBase )
    ///   {
    ///     if ( u->getType().isCritter() && !u->isInvincible() )
    ///     {
    ///       BWAPI::Unit myQueen = u->getClosestUnit(BWAPI::Filter::GetType == BWAPI::UnitTypes::Zerg_Queen && BWAPI::Filter::IsOwned);
    ///       if ( myQueen )
    ///         myQueen->useTech(BWAPI::TechTypes::Parasite, u);
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
    /// normally available. Use PlayerInterface::isUnitAvailable to determine if a unit type is
    /// actually available in the current game for a specific player.
    ///
    /// @returns UnitType::set containing the units it can build.
    /// @see PlayerInterface::isUnitAvailable
    ///
    /// @since 4.1.2
    const UnitType::set& buildsWhat() const;

    /// <summary>Retrieves the set of technologies that this unit type is capable of researching.</summary>
    /// 
    /// @note Some maps have special parameters that disable certain technologies. Use
    /// PlayerInterface::isResearchAvailable to determine if a technology is actually available in the
    /// current game for a specific player.
    ///
    /// @returns TechType::set containing the technology types that can be researched.
    /// @see PlayerInterface::isResearchAvailable
    ///
    /// @since 4.1.2
    const SetContainer<TechType>& researchesWhat() const;

    /// <summary>Retrieves the set of upgrades that this unit type is capable of upgrading.</summary>
    ///
    /// @note Some maps have special upgrade limitations. Use PlayerInterface::getMaxUpgradeLevel
    /// to check if an upgrade is available.
    ///
    /// @returns UpgradeType::set containing the upgrade types that can be upgraded.
    /// @see PlayerInterface::getMaxUpgradeLevel
    ///
    /// @since 4.1.2
    const SetContainer<UpgradeType>& upgradesWhat() const;
  };

  /// <summary>The amount of shield points that a unit recovers over 256 frames.</summary>
  /// That is, 7/256 shields are regenerated per frame.
  ///
  /// @since 4.1.0 Beta
  static const int SHIELD_REGEN_RATE = 7;

  /// <summary>The amount of energy that a unit with special abilities recovers over 256 frames.</summary>
  /// That is, 8/256 energy is generated per frame.
  ///
  /// @since 4.1.0 Beta
  static const int ENERGY_REGEN_RATE = 8;

  /// <summary>The amount of life that a zerg unit recovers over 256 frames.</summary>
  /// That is, 4/256 life is regenerated per frame.
  ///
  /// @since 4.1.0 Beta
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
    extern const UnitType Terran_Firebat;
    extern const UnitType Terran_Ghost;
    extern const UnitType Terran_Goliath;
    extern const UnitType Terran_Marine;
    extern const UnitType Terran_Medic;
    extern const UnitType Terran_SCV;
    extern const UnitType Terran_Siege_Tank_Siege_Mode;
    extern const UnitType Terran_Siege_Tank_Tank_Mode;
    extern const UnitType Terran_Vulture;
    extern const UnitType Terran_Vulture_Spider_Mine;
    /// @}
    /// @name Terran Air Units
    /// @{
    extern const UnitType Terran_Battlecruiser;
    extern const UnitType Terran_Dropship;
    extern const UnitType Terran_Nuclear_Missile;
    extern const UnitType Terran_Science_Vessel;
    extern const UnitType Terran_Valkyrie;
    extern const UnitType Terran_Wraith;
    /// @}
    /// @name Terran Heroes
    /// @{
    extern const UnitType Hero_Alan_Schezar;
    extern const UnitType Hero_Alexei_Stukov;
    extern const UnitType Hero_Arcturus_Mengsk;
    extern const UnitType Hero_Edmund_Duke_Tank_Mode;
    extern const UnitType Hero_Edmund_Duke_Siege_Mode;
    extern const UnitType Hero_Gerard_DuGalle;
    extern const UnitType Hero_Gui_Montag;
    extern const UnitType Hero_Hyperion;
    extern const UnitType Hero_Jim_Raynor_Marine;
    extern const UnitType Hero_Jim_Raynor_Vulture;
    extern const UnitType Hero_Magellan;
    extern const UnitType Hero_Norad_II;
    extern const UnitType Hero_Samir_Duran;
    extern const UnitType Hero_Sarah_Kerrigan;
    extern const UnitType Hero_Tom_Kazansky;
    extern const UnitType Terran_Civilian;
    /// @}
    /// @name Terran Buildings
    /// @{
    extern const UnitType Terran_Academy;
    extern const UnitType Terran_Armory;
    extern const UnitType Terran_Barracks;
    extern const UnitType Terran_Bunker;
    extern const UnitType Terran_Command_Center;
    extern const UnitType Terran_Engineering_Bay;
    extern const UnitType Terran_Factory;
    extern const UnitType Terran_Missile_Turret;
    extern const UnitType Terran_Refinery;
    extern const UnitType Terran_Science_Facility;
    extern const UnitType Terran_Starport;
    extern const UnitType Terran_Supply_Depot;
    /// @}
    /// @name Terran Addons
    /// @{
    extern const UnitType Terran_Comsat_Station;
    extern const UnitType Terran_Control_Tower;
    extern const UnitType Terran_Covert_Ops;
    extern const UnitType Terran_Machine_Shop;
    extern const UnitType Terran_Nuclear_Silo;
    extern const UnitType Terran_Physics_Lab;
    /// @}
    /// @name Terran Special Buildings
    /// @{
    extern const UnitType Special_Crashed_Norad_II;
    extern const UnitType Special_Ion_Cannon;
    extern const UnitType Special_Power_Generator;
    extern const UnitType Special_Psi_Disrupter;
    /// @}
    /// @name Protoss Ground Units
    /// @{
    extern const UnitType Protoss_Archon;
    extern const UnitType Protoss_Dark_Archon;
    extern const UnitType Protoss_Dark_Templar;
    extern const UnitType Protoss_Dragoon;
    extern const UnitType Protoss_High_Templar;
    extern const UnitType Protoss_Probe;
    extern const UnitType Protoss_Reaver;
    extern const UnitType Protoss_Scarab;
    extern const UnitType Protoss_Zealot;
    /// @}
    /// @name Protoss Air Units
    /// @{
    extern const UnitType Protoss_Arbiter;
    extern const UnitType Protoss_Carrier;
    extern const UnitType Protoss_Corsair;
    extern const UnitType Protoss_Interceptor;
    extern const UnitType Protoss_Observer;
    extern const UnitType Protoss_Scout;
    extern const UnitType Protoss_Shuttle;
    /// @}
    /// @name Protoss Heroes Units
    /// @{
    extern const UnitType Hero_Aldaris;
    extern const UnitType Hero_Artanis;
    extern const UnitType Hero_Danimoth;
    extern const UnitType Hero_Dark_Templar;
    extern const UnitType Hero_Fenix_Dragoon;
    extern const UnitType Hero_Fenix_Zealot;
    extern const UnitType Hero_Gantrithor;
    extern const UnitType Hero_Mojo;
    extern const UnitType Hero_Raszagal;
    extern const UnitType Hero_Tassadar;
    extern const UnitType Hero_Tassadar_Zeratul_Archon;
    extern const UnitType Hero_Warbringer;
    extern const UnitType Hero_Zeratul;
    /// @}
    /// @name Protoss Buildings
    /// @{
    extern const UnitType Protoss_Arbiter_Tribunal;
    extern const UnitType Protoss_Assimilator;
    extern const UnitType Protoss_Citadel_of_Adun;
    extern const UnitType Protoss_Cybernetics_Core;
    extern const UnitType Protoss_Fleet_Beacon;
    extern const UnitType Protoss_Forge;
    extern const UnitType Protoss_Gateway;
    extern const UnitType Protoss_Nexus;
    extern const UnitType Protoss_Observatory;
    extern const UnitType Protoss_Photon_Cannon;
    extern const UnitType Protoss_Pylon;
    extern const UnitType Protoss_Robotics_Facility;
    extern const UnitType Protoss_Robotics_Support_Bay;
    extern const UnitType Protoss_Shield_Battery;
    extern const UnitType Protoss_Stargate;
    extern const UnitType Protoss_Templar_Archives;
    /// @}
    /// @name Protoss Special Buildings
    /// @{
    extern const UnitType Special_Khaydarin_Crystal_Form;
    extern const UnitType Special_Protoss_Temple;
    extern const UnitType Special_Stasis_Cell_Prison;
    extern const UnitType Special_Warp_Gate;
    extern const UnitType Special_XelNaga_Temple;
    /// @}
    /// @name Zerg Ground Units
    /// @{
    extern const UnitType Zerg_Broodling;
    extern const UnitType Zerg_Defiler;
    extern const UnitType Zerg_Drone;
    extern const UnitType Zerg_Egg;
    extern const UnitType Zerg_Hydralisk;
    extern const UnitType Zerg_Infested_Terran;
    extern const UnitType Zerg_Larva;
    extern const UnitType Zerg_Lurker;
    extern const UnitType Zerg_Lurker_Egg;
    extern const UnitType Zerg_Ultralisk;
    extern const UnitType Zerg_Zergling;
    /// @}
    /// @name Zerg Air Units
    /// @{
    extern const UnitType Zerg_Cocoon;
    extern const UnitType Zerg_Devourer;
    extern const UnitType Zerg_Guardian;
    extern const UnitType Zerg_Mutalisk;
    extern const UnitType Zerg_Overlord;
    extern const UnitType Zerg_Queen;
    extern const UnitType Zerg_Scourge;
    /// @}
    /// @name Zerg Heroes
    /// @{
    extern const UnitType Hero_Devouring_One;
    extern const UnitType Hero_Hunter_Killer;
    extern const UnitType Hero_Infested_Duran;
    extern const UnitType Hero_Infested_Kerrigan;
    extern const UnitType Hero_Kukulza_Guardian;
    extern const UnitType Hero_Kukulza_Mutalisk;
    extern const UnitType Hero_Matriarch;
    extern const UnitType Hero_Torrasque;
    extern const UnitType Hero_Unclean_One;
    extern const UnitType Hero_Yggdrasill;
    /// @}
    /// @name Zerg Buildings
    /// @{
    extern const UnitType Zerg_Creep_Colony;
    extern const UnitType Zerg_Defiler_Mound;
    extern const UnitType Zerg_Evolution_Chamber;
    extern const UnitType Zerg_Extractor;
    extern const UnitType Zerg_Greater_Spire;
    extern const UnitType Zerg_Hatchery;
    extern const UnitType Zerg_Hive;
    extern const UnitType Zerg_Hydralisk_Den;
    extern const UnitType Zerg_Infested_Command_Center;
    extern const UnitType Zerg_Lair;
    extern const UnitType Zerg_Nydus_Canal;
    extern const UnitType Zerg_Queens_Nest;
    extern const UnitType Zerg_Spawning_Pool;
    extern const UnitType Zerg_Spire;
    extern const UnitType Zerg_Spore_Colony;
    extern const UnitType Zerg_Sunken_Colony;
    extern const UnitType Zerg_Ultralisk_Cavern;
    /// @}
    /// @name Zerg Special Buildings
    /// @{
    extern const UnitType Special_Cerebrate;
    extern const UnitType Special_Cerebrate_Daggoth;
    extern const UnitType Special_Mature_Chrysalis;
    extern const UnitType Special_Overmind;
    extern const UnitType Special_Overmind_Cocoon;
    extern const UnitType Special_Overmind_With_Shell;
    /// @}
    /// @name Critters
    /// @{
    extern const UnitType Critter_Bengalaas;
    extern const UnitType Critter_Kakaru;
    extern const UnitType Critter_Ragnasaur;
    extern const UnitType Critter_Rhynadon;
    extern const UnitType Critter_Scantid;
    extern const UnitType Critter_Ursadon;
    /// @}
    /// @name Resources
    /// @{
    extern const UnitType Resource_Mineral_Field;
    extern const UnitType Resource_Mineral_Field_Type_2;
    extern const UnitType Resource_Mineral_Field_Type_3;
    extern const UnitType Resource_Vespene_Geyser;
    /// @}
    /// @name Spells
    /// @{
    extern const UnitType Spell_Dark_Swarm;
    extern const UnitType Spell_Disruption_Web;
    extern const UnitType Spell_Scanner_Sweep;
    /// @}
    /// @name Beacons
    /// @{
    extern const UnitType Special_Protoss_Beacon;
    extern const UnitType Special_Protoss_Flag_Beacon;
    extern const UnitType Special_Terran_Beacon;
    extern const UnitType Special_Terran_Flag_Beacon;
    extern const UnitType Special_Zerg_Beacon;
    extern const UnitType Special_Zerg_Flag_Beacon;
    /// @}
    /// @name Powerups
    /// @{
    extern const UnitType Powerup_Data_Disk;
    extern const UnitType Powerup_Flag;
    extern const UnitType Powerup_Khalis_Crystal;
    extern const UnitType Powerup_Khaydarin_Crystal;
    extern const UnitType Powerup_Mineral_Cluster_Type_1;
    extern const UnitType Powerup_Mineral_Cluster_Type_2;
    extern const UnitType Powerup_Protoss_Gas_Orb_Type_1;
    extern const UnitType Powerup_Protoss_Gas_Orb_Type_2;
    extern const UnitType Powerup_Psi_Emitter;
    extern const UnitType Powerup_Terran_Gas_Tank_Type_1;
    extern const UnitType Powerup_Terran_Gas_Tank_Type_2;
    extern const UnitType Powerup_Uraj_Crystal;
    extern const UnitType Powerup_Young_Chrysalis;
    extern const UnitType Powerup_Zerg_Gas_Sac_Type_1;
    extern const UnitType Powerup_Zerg_Gas_Sac_Type_2;
    /// @}
    /// @name Traps
    /// @{
    extern const UnitType Special_Floor_Gun_Trap;
    extern const UnitType Special_Floor_Missile_Trap;
    extern const UnitType Special_Right_Wall_Flame_Trap;
    extern const UnitType Special_Right_Wall_Missile_Trap;
    extern const UnitType Special_Wall_Flame_Trap;
    extern const UnitType Special_Wall_Missile_Trap;
    /// @}
    /// @name Doors
    /// @{
    extern const UnitType Special_Pit_Door;
    extern const UnitType Special_Right_Pit_Door;
    extern const UnitType Special_Right_Upper_Level_Door;
    extern const UnitType Special_Upper_Level_Door;
    /// @}
    /// @name Special
    /// @{
    extern const UnitType Special_Cargo_Ship;
    extern const UnitType Special_Floor_Hatch;
    extern const UnitType Special_Independant_Starport;
    extern const UnitType Special_Map_Revealer;
    extern const UnitType Special_Mercenary_Gunship;
    extern const UnitType Special_Start_Location;
    /// @}

    extern const UnitType None;
    extern const UnitType AllUnits;
    extern const UnitType Men;
    extern const UnitType Buildings;
    extern const UnitType Factories;
    extern const UnitType Unknown;
  }

  static_assert(sizeof(UnitType) == sizeof(int), "Expected type to resolve to primitive size.");
}

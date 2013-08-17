#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/PositionUnit.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Filters.h>
#include <BWAPI/UnaryFilter.h>
#include <BWAPI/Interface.h>

namespace BWAPI
{
  // Forwards
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class Order;
  class TechType;
  class UpgradeType;

  class RegionInterface;
  typedef RegionInterface *Region;

  class UnitCommand;
  class UnitCommandType;
  class Unitset;
  class WeaponType;

  class UnitInterface;
  typedef UnitInterface *Unit;
  
  /// The Unit class is used to get information about individual units as well as issue orders to units. Each
  /// unit in the game has a unique Unit object, and Unit objects are not deleted until the end of the match
  /// (so you don't need to worry about unit pointers becoming invalid).
  ///
  /// Every Unit in the game is either accessible or inaccessible. To determine if an AI can access a
  /// particular unit, BWAPI checks to see if Flag::CompleteMapInformation is enabled. So there are two cases
  /// to consider - either the flag is enabled, or it is disabled:
  ///
  /// If Flag::CompleteMapInformation is disabled, then a unit is accessible if and only if it is visible.
  /// @note Some properties of visible enemy units will not be made available to the AI (such as the
  /// contents of visible enemy dropships). If a unit is not visible, UnitInterface::exists will return false,
  /// regardless of whether or not the unit exists. This is because absolutely no state information on
  /// invisible enemy units is made available to the AI. To determine if an enemy unit has been destroyed, the
  /// AI must watch for AIModule::onUnitDestroy messages from BWAPI, which is only called for visible units
  /// which get destroyed.
  ///
  /// If Flag::CompleteMapInformation is enabled, then all units that exist in the game are accessible, and
  /// UnitInterface::exists is accurate for all units. Similarly AIModule::onUnitDestroy messages are generated for all
  /// units that get destroyed, not just visible ones.
  ///
  /// If a Unit is not accessible, then only the getInitial__ functions will be available to the AI.
  /// However for units that were owned by the player, getPlayer and getType will continue to work for units
  /// that have been destroyed.
  class UnitInterface : public Interface<UnitInterface>
  {
  protected:
    virtual ~UnitInterface() {};
  public:
    /// Retrieves a unique identifier for this unit.
    ///
    /// @returns An integer containing the unit's identifier.
    ///
    /// @see getReplayID
    virtual int getID() const = 0;

    /// Checks if the Unit exists in the view of the BWAPI player.
    ///
    /// This is used primarily to check if BWAPI has access to a specific unit, or if the
    /// unit is alive. This function is more general and would be synonymous to an isAlive
    /// function if such a function were necessary.
    ///
    /// @retval true If the unit exists on the map and is visible according to BWAPI.
    /// @retval false If the unit is not accessible or the unit is dead.
    ///
    /// In the event that this function returns false, there are two cases to consider:
    ///   1. You own the unit. This means the unit is dead.
    ///   2. Another player owns the unit. This could either mean that you don't have access
    ///      to the unit or that the unit has died. You can specifically identify dead units
    ///      by polling onUnitDestroy.
    ///
    /// @see isVisible, isCompleted
    virtual bool exists() const = 0;

    /// Retrieves the unit identifier for this unit as seen in replay data.
    ///
    /// @note This is only available if Flag::CompleteMapInformation is enabled.
    ///
    /// @returns An integer containing the replay unit identifier.
    ///
    /// @see getID
    virtual int getReplayID() const = 0;

    /// Retrieves the player that owns this unit.
    ///
    /// @retval Game::neutral() If the unit is a neutral unit or inaccessible.
    ///
    /// @returns The owning Player interface object.
    virtual Player getPlayer() const = 0;

    /// Retrieves the unit's type.
    ///
    /// @retval UnitTypes::Unknown if this unit is inaccessible or cannot be determined.
    /// @returns A UnitType objects representing the unit's type.
    ///
    /// @see getInitialType
    virtual UnitType getType() const = 0;

    /// Retrieves the unit's position from the upper left corner of the map in pixels.
    ///
    /// @retval Positions::Unknown if this unit is inaccessible.
    ///
    /// @returns Position object representing the unit's current position.
    ///
    /// @see getTilePosition, getInitialPosition
    virtual Position getPosition() const = 0;

    /// Retrieves the unit's build position from the upper left corner of the map in tiles.
    ///
    /// @note: This tile position is the tile that is at the top left corner of the structure.
    ///
    /// @retval TilePositions::Unknown if this unit is inaccessible.
    ///
    /// @returns TilePosition object representing the unit's current tile position.
    ///
    /// @see getPosition, getInitialTilePosition
    TilePosition getTilePosition() const;

    /// Retrieves the unit's facing direction in radians.
    ///
    /// @note A value of 0.0 means the unit is facing east.
    ///
    /// @returns A double with the angle measure in radians.
    virtual double getAngle() const = 0;

    /// Retrieves the x component of the unit's velocity, measured in pixels per frame.
    ///
    /// @returns A double that represents the velocity's x component.
    ///
    /// @see getVelocityY
    virtual double getVelocityX() const = 0;

    /// Retrieves the y component of the unit's velocity, measured in pixels per frame.
    ///
    /// @returns A double that represents the velocity's y component.
    ///
    /// @see getVelocityX
    virtual double getVelocityY() const = 0;

    /// Retrieves the Region that the center of the unit is in.
    ///
    /// @retval nullptr If the unit is inaccessible.
    ///
    /// @returns The Region object that contains this unit.
    ///
    /// Example
    /// @code
    ///   Unitset myUnits = Broodwar->self()->getUnits();
    ///   for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///   {
    ///     if ( u->isFlying() && u->isUnderAttack() ) // implies exists and isCompleted
    ///     {
    ///       Region r = u->getRegion();
    ///       if ( r )
    ///         u->move(r->getClosestInaccessibleRegion()); // Retreat to inaccessible region
    ///     }
    ///   }
    /// @endcode
    /// @implies exists
    BWAPI::Region getRegion() const;

    /// Retrieves the X coordinate of the unit's left boundary, measured in pixels from the left
    /// side of the map.
    ///
    /// @returns An integer representing the position of the left side of the unit.
    ///
    /// @see getTop, getRight, getBottom
    int getLeft() const;

    /// Retrieves the Y coordinate of the unit's top boundary, measured in pixels from the top of
    /// the map.
    ///
    /// @returns An integer representing the position of the top side of the unit.
    ///
    /// @see getLeft, getRight, getBottom
    int getTop() const;

    /// Retrieves the X coordinate of the unit's right boundary, measured in pixels from the left
    /// side of the map.
    ///
    /// @returns An integer representing the position of the right side of the unit.
    ///
    /// @see getLeft, getTop, getBottom
    int getRight() const;

    /// Retrieves the Y coordinate of the unit's bottom boundary, measured in pixels from the top
    /// of the map.
    ///
    /// @returns An integer representing the position of the bottom side of the unit.
    ///
    /// @see getLeft, getTop, getRight
    int getBottom() const;

    /// Retrieves the unit's current Hit Points (HP) as seen in the game.
    ///
    /// @returns An integer representing the amount of hit points a unit currently has.
    ///
    /// @note In Starcraft, a unit usually dies when its HP reaches 0. It is possible however, to
    /// have abnormal HP values in the Use Map Settings game type and as the result of a hack over
    /// Battle.net. Such values include units that have 0 HP (can't be killed conventionally)
    /// or even negative HP (death in one hit).
    ///
    /// @see UnitType::maxHitPoints, getShields, getInitialHitPoints
    virtual int getHitPoints() const = 0;

    /// Retrieves the unit's current Shield Points (Shields) as seen in the game.
    ///
    /// @returns An integer representing the amount of shield points a unit currently has.
    ///
    /// @see UnitType::maxShields, getHitPoints
    virtual int getShields() const = 0;

    /// Retrieves the unit's current Energy Points (Energy) as seen in the game.
    ///
    /// @returns An integer representing the amount of energy points a unit currently has.
    ///
    /// @note Energy is required in order for units to use abilities.
    ///
    /// @see UnitType::maxEnergy
    virtual int getEnergy() const = 0;

    /// Retrieves the resource amount from a resource container, such as a Mineral Field and
    /// Vespene Geyser. If the unit is inaccessible, then the last known resource amount is
    /// returned.
    ///
    /// @returns An integer representing the last known amount of resources remaining in this
    /// resource.
    ///
    /// @see getInitialResources
    virtual int getResources() const = 0;

    /// Retrieves a grouping index from a resource container. Other resource containers of the
    /// same value are considered part of one expansion location (group of resources that are
    /// close together).
    ///
    /// @note This grouping method is explicitly determined by Starcraft itself and is used only
    /// by the internal AI.
    ///
    /// @returns An integer with an identifier between 0 and 250 that determine which resources
    /// are grouped together to form an expansion.
    virtual int getResourceGroup() const = 0;

    /// Retrieves the distance between this unit and a target.
    ///
    /// @note Distance is calculated from the edge of this unit, using Starcraft's own distance
    /// algorithm.
    ///
    /// @param target
    ///   A Position or a Unit to calculate the distance to. If it is a unit, then it will
    ///   calculate the distance to the edge of the target unit.
    ///
    /// @returns An integer representation of the number of pixels between this unit and the
    /// \p target.
    int getDistance(PositionOrUnit target) const;

    /// Using data provided by Starcraft, checks if there is a path available from this unit to
    /// the given target.
    ///
    /// @note This function only takes into account the terrain data, and does not include
    /// buildings when determining if a path is available. However, the complexity of this
    /// function is constant ( O(1) ), and no extensive calculations are necessary.
    ///
    /// @note If the current unit is an air unit, then this function will always return true.
    ///
    /// @param target
    ///   A Position or a Unit that is used to determine if this unit has a path to the target.
    ///
    /// @retval true If there is a path between this unit and the target.
    /// @retval false If the target is on a different piece of land than this one (such as an
    /// island).
    bool hasPath(PositionOrUnit target) const;

    /// Retrieves the frame number that sent the last successful command.
    /// 
    /// @note This value is comparable to Game::getFrameCount.
    ///
    /// @returns The frame number that sent the last successfully processed command to BWAPI.
    /// @see Game::getFrameCount, getLastCommand
    virtual int getLastCommandFrame() const = 0;

    /// Retrieves the last successful command that was sent to BWAPI.
    ///
    /// @returns A UnitCommand object containing information about the command that was processed.
    /// @see getLastCommandFrame
    virtual UnitCommand getLastCommand() const = 0;

    /// Retrieves the Player that last attacked this unit.
    /// 
    /// @returns Player interface object representing the player that last attacked this unit.
    /// @retval nullptr If this unit was not attacked.
    /// @implies exists()
    virtual BWAPI::Player getLastAttackingPlayer() const = 0;

    /// Retrieves the initial type of the unit. This is the type that the unit starts as in the
    /// beginning of the game. This is used to access the types of static neutral units such as
    /// mineral fields when they are not visible.
    ///
    /// @returns UnitType of this unit as it was when it was created.
    /// @retval UnitTypes::Unknown if this unit was not a static neutral unit in the beginning of
    /// the game.
    virtual UnitType getInitialType() const = 0;

    /// Retrieves the initial position of this unit. This is the position that the unit starts at
    /// in the beginning of the game. This is used to access the positions of static neutral units
    /// such as mineral fields when they are not visible.
    ///
    /// @returns Position indicating the unit's initial position when it was created.
    /// @retval Positions::Unknown if this unit was not a static neutral unit in the beginning of
    /// the game.
    virtual Position getInitialPosition() const = 0;

    /// Retrieves the initial build tile position of this unit. This is the tile position that the
    /// unit starts at in the beginning of the game. This is used to access the tile positions of
    /// static neutral units such as mineral fields when they are not visible. The build tile
    /// position corresponds to the upper left corner of the unit.
    ///
    /// @returns TilePosition indicating the unit's initial tile position when it was created.
    /// @retval TilePositions::Unknown if this unit was not a static neutral unit in the beginning of
    /// the game.
    virtual TilePosition getInitialTilePosition() const = 0;

    /// Retrieves the amount of hit points that this unit started off with at the beginning of the
    /// game. The unit must be neutral.
    ///
    /// @returns Number of hit points that this unit started with.
    /// @retval 0 if this unit was not a neutral unit at the beginning of the game.
    ///
    /// @note: It is possible for the unit's initial hit points to differ from the maximum hit
    /// points.
    ///
    /// @see Game::getStaticNeutralUnits
    virtual int getInitialHitPoints() const = 0;

    /// Retrieves the amount of resources contained in the unit at the beginning of the game. The
    /// unit must be a neutral resource container.
    ///
    /// @returns Amount of resources that this unit started with.
    /// @retval 0 if this unit was not a neutral unit at the beginning of the game, or if this
    /// unit does not contain resources. It is possible that the unit simply contains 0 resources.
    ///
    /// @see Game::getStaticNeutralUnits
    virtual int getInitialResources() const = 0;

    /// Retrieves the number of units that this unit has killed in total.
    ///
    /// @note The maximum amount of recorded kills per unit is 255.
    ///
    /// @returns integer indicating this unit's kill count.
    virtual int getKillCount() const = 0;

    /// Retrieves the number of acid spores that this unit is inflicted with.
    ///
    /// @returns Number of acid spores on this unit.
    virtual int getAcidSporeCount() const = 0;

    /// Retrieves the number of interceptors that this unit manages. This function is only for the
    /// @Carrier.
    ///
    /// @returns Number of interceptors in this unit.
    virtual int getInterceptorCount() const = 0;

    /// Retrieves the number of scarabs that this unit has for use. This function is only for the
    /// @Reaver.
    ///
    /// @returns Number of scarabs this unit has ready.
    virtual int getScarabCount() const = 0;

    /// Retrieves the amount of @mines this unit has available. This function is only for the
    /// @Vulture.
    ///
    /// @returns Number of spider mines available for placement.
    virtual int getSpiderMineCount() const = 0;

    /// Retrieves the unit's ground weapon cooldown. This value decreases every frame, until it
    /// reaches 0. When the value is 0, this indicates that the unit is capable of using its ground
    /// weapon, otherwise it must wait until it reaches 0.
    ///
    /// @note This value will vary, because Starcraft adds an additional random value between
    /// (-1) and (+2) to the unit's weapon cooldown.
    ///
    /// @returns Number of frames needed for the unit's ground weapon to become available again.
    virtual int getGroundWeaponCooldown() const = 0;

    /// Retrieves the unit's air weapon cooldown. This value decreases every frame, until it
    /// reaches 0. When the value is 0, this indicates that the unit is capable of using its air
    /// weapon, otherwise it must wait until it reaches 0.
    ///
    /// @note This value will vary, because Starcraft adds an additional random value between
    /// (-1) and (+2) to the unit's weapon cooldown.
    ///
    /// @returns Number of frames needed for the unit's air weapon to become available again.
    virtual int getAirWeaponCooldown() const = 0;

    /// Retrieves the unit's ability cooldown. This value decreases every frame, until it
    /// reaches 0. When the value is 0, this indicates that the unit is capable of using one of
    /// its special abilities, otherwise it must wait until it reaches 0.
    ///
    /// @note This value will vary, because Starcraft adds an additional random value between
    /// (-1) and (+2) to the unit's ability cooldown.
    ///
    /// @returns Number of frames needed for the unit's abilities to become available again.
    virtual int getSpellCooldown() const = 0;

    /// Retrieves the amount of hit points remaining on the @matrix created by a @Science_Vessel.
    /// The @matrix ability starts with 250 hit points when it is used.
    ///
    /// @returns Number of hit points remaining on this unit's @matrix.
    ///
    /// @see getDefenseMatrixTimer, isDefenseMatrixed
    virtual int getDefenseMatrixPoints() const = 0;

    /// Retrieves the time, in frames, that the @matrix will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see getDefenseMatrixPoints, isDefenseMatrixed
    virtual int getDefenseMatrixTimer() const = 0;

    /// Retrieves the time, in frames, that @ensnare will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isEnsnared
    virtual int getEnsnareTimer() const = 0;

    /// Retrieves the time, in frames, that @irradiate will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isIrradiated
    virtual int getIrradiateTimer() const = 0;

    /// Retrieves the time, in frames, that @lockdown will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isLockdowned
    virtual int getLockdownTimer() const = 0;

    /// Retrieves the time, in frames, that @maelstrom will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isMaelstrommed
    virtual int getMaelstromTimer() const = 0;

    // TODO: add doc (I think I forget what this is for exactly?)
    virtual int getOrderTimer() const = 0;

    /// Retrieves the time, in frames, that @plague will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isPlagued
    virtual int getPlagueTimer() const = 0;

    /// Retrieves the time, in frames, until this temporary unit is destroyed or removed. This is
    /// used to determine the remaining time for the following units that were created by abilities:
    ///   - @hallucination
    ///   - @broodling
    ///   - @swarm
    ///   - @dweb
    ///   - @scanner
    ///   .
    /// Once this value reaches 0, the unit is destroyed.
    virtual int getRemoveTimer() const = 0;

    /// Retrieves the time, in frames, that @stasis will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isPlagued
    virtual int getStasisTimer() const = 0;

    /// Retrieves the time, in frames, that @stim will remain active on the current unit.
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isPlagued
    virtual int getStimTimer() const = 0;

    /** Returns the building type a worker is about to construct. If the unit is a morphing Zerg unit or an
     * incomplete building, this returns the UnitType the unit is about to become upon completion.*/
    virtual UnitType getBuildType() const = 0;

    /** Returns the list of units queued up to be trained.
     * \see UnitInterface::train, UnitInterface::cancelTrain, UnitInterface::isTraining. */
    virtual UnitType::set getTrainingQueue() const = 0;

    /** Returns the tech that the unit is currently researching. If the unit is not researching anything,
     * TechTypes::None is returned.
     * \see UnitInterface::research, UnitInterface::cancelResearch, UnitInterface::isResearching, UnitInterface::getRemainingResearchTime. */
    virtual TechType getTech() const = 0;

    /** Returns the upgrade that the unit is currently upgrading. If the unit is not upgrading anything,
     * UpgradeTypes::None is returned.
     * \see UnitInterface::upgrade, UnitInterface::cancelUpgrade, UnitInterface::isUpgrading, UnitInterface::getRemainingUpgradeTime. */
    virtual UpgradeType getUpgrade() const = 0;

    /** Returns the remaining build time of a unit/building that is being constructed. */
    virtual int getRemainingBuildTime() const = 0;

    /** Returns the remaining time of the unit that is currently being trained. If the unit is a Hatchery,
     * Lair, or Hive, this returns the amount of time until the next larva spawns, or 0 if the unit already
     * has 3 larva. */
    virtual int getRemainingTrainTime() const = 0;

    /** Returns the amount of time until the unit is done researching its current tech. If the unit is not
     * researching anything, 0 is returned.
     * \see UnitInterface::research, UnitInterface::cancelResearch, UnitInterface::isResearching, UnitInterface::getTech. */
    virtual int getRemainingResearchTime() const = 0;

    /** Returns the amount of time until the unit is done upgrading its current upgrade. If the unit is not
     * upgrading anything, 0 is returned.
     * \see UnitInterface::upgrade, UnitInterface::cancelUpgrade, UnitInterface::isUpgrading, UnitInterface::getUpgrade. */
    virtual int getRemainingUpgradeTime() const = 0;

    /** If the unit is an SCV that is constructing a building, this will return the building it is
     * constructing. If the unit is a Terran building that is being constructed, this will return the SCV
     * that is constructing it. */
    virtual Unit getBuildUnit() const = 0;

    /** Generally returns the appropriate target unit after issuing an order that accepts a target unit
     * (i.e. attack, repair, gather, follow, etc.). To check for a target that has been acquired
     * automatically (without issuing an order) see getOrderTarget. */
    virtual Unit getTarget() const = 0;

    /** Returns the target position the unit is moving to (provided a valid path to the target position
     * exists). */
    virtual Position getTargetPosition() const = 0;

    // TODO: add doc
    virtual Order getOrder() const = 0;
    virtual Order getSecondaryOrder() const = 0;

    /** This is usually set when the low level unit AI acquires a new target automatically. For example if
     * an enemy probe comes in range of your marine, the marine will start attacking it, and getOrderTarget
     * will be set in this case, but not getTarget. */
    virtual Unit getOrderTarget() const = 0;

  /** Returns the target position for the units order. For example for the move order getTargetPosition
   * returns the end of the units path but this returns the location the unit is trying to move to.  */
    virtual Position getOrderTargetPosition() const = 0;

    /** Returns the position the building is rallied to. If the building does not produce units,
     * Positions::None is returned.
     * \see UnitInterface::setRallyPoint, UnitInterface::getRallyUnit. */
    virtual Position getRallyPosition() const = 0;

    /** Returns the unit the building is rallied to. If the building is not rallied to any unit, NULL is
     * returned.
     * \see UnitInterface::setRallyPoint, UnitInterface::getRallyPosition. */
    virtual Unit getRallyUnit() const = 0;

    /** Returns the add-on of this unit, or NULL if the unit doesn't have an add-on. */
    virtual Unit getAddon() const = 0;

    /// Retrieves the @Nydus_Canal that is attached to this one. Every @Nydus_Canal can place a
    /// "Nydus Exit" which, when connected, can be travelled through by @Zerg units.
    ///
    /// @returns Unit interface representing the @Nydus_Canal connected to this one.
    /// @retval nullptr if the unit is not a @Nydus_Canal, is not owned, or has not placed a Nydus
    /// Exit.
    virtual Unit getNydusExit() const = 0;

    /// Retrieves the power-up that the worker unit is holding. Power-ups are special units such
    /// as the @Flag in the @CTF game type, which can be picked up by worker units.
    ///
    /// @note If your bot is strictly melee/1v1, then this method is not necessary.
    ///
    /// @returns The Unit interface object that represents the power-up.
    /// @retval nullptr If the unit is not carrying anything.
    ///
    /// Example
    /// @code
    ///   BWAPI::Unitset myUnits = BWAPI::Broodwar->self()getUnits();
    ///   for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///   {
    ///     // If we are carrying a flag
    ///     if ( u->getPowerUp() && u->getPowerUp()->getType() == BWAPI::UnitTypes::Powerup_Flag )
    ///       u->move( u->getClosestUnit(BWAPI::Filter::IsFlagBeacon && BWAPI::Filter::IsOwned) );  // return it to our flag beacon to score
    ///   }
    /// @endcode
    /// @implies getType().isWorker(), isCompleted()
    virtual Unit getPowerUp() const = 0;

    /// Retrieves the @Transport or @Bunker unit that has this unit loaded inside of it.
    ///
    /// @returns Unit interface object representing the @Transport containing this unit.
    /// @retval nullptr if this unit is not in a @Transport.
    virtual Unit getTransport() const = 0;

    /// Retrieves the set of units that are contained within this @Bunker or @Transport .
    ///
    /// @returns A Unitset object containing all of the units that are loaded inside of the
    /// current unit.
    virtual Unitset getLoadedUnits() const = 0;

    /// Retrieves the remaining unit-space available for @Bunker s and transports.
    ///
    /// @returns The number of spots available to transport a unit.
    ///
    /// @see getLoadedUnits
    int getSpaceRemaining() const;

    /// Retrieves the parent @Carrier that owns this @Interceptor.
    ///
    /// @returns The parent @Carrier unit that has ownership of this one.
    /// @retval nullptr if the current unit is not an @Interceptor.
    virtual Unit getCarrier() const = 0;

    /// Retrieves the set of @Interceptors controlled by this unit. This is intended for @Carriers.
    ///
    /// @returns Unitset containing @Interceptor units owned by this one.
    virtual Unitset getInterceptors() const = 0;

    /// Retrieves the parent @Hatchery, @Lair, or @Hive that owns this particular unit. This is
    /// intended for @Larvae.
    ///
    /// @returns Hatchery unit that has ownership of this larva.
    /// @retval nullptr if the current unit is not a @Larva or has no parent.
    virtual Unit getHatchery() const = 0;

    /// Retrieves the set of @Larvae that were spawned by this unit. Only @Hatcheries, @Lairs, and
    /// @Hives are capable of spawning @Larvae. This is like clicking the "Select Larva" button
    /// and getting the selection of @Larvae.
    ///
    /// @returns Unitset containing @Larva units owned by this unit. The set will be empty if
    /// there are none.
    virtual Unitset getLarva() const = 0;

    /// Retrieves the set of all units in a given radius of the current unit.
    ///
    /// Takes into account this unit's dimensions. Can optionally specify a filter that is composed using
    /// BWAPI Filter semantics to include only specific units (such as only ground units, etc.)
    ///
    /// @param radius
    ///   The radius, in pixels, to search for units.
    /// @param pred (optional)
    ///   The composed function predicate to include only specific (desired) units in the set. Defaults to
    ///   nullptr, which means no filter.
    ///
    /// @returns A Unitset containing the set of units that match the given criteria.
    ///
    /// Example usage:
    /// @code
    ///   // Get main building closest to start location.
    ///   BWAPI::Unit pMain = BWAPI::Broodwar->getClosestUnit( BWAPI::Broodwar->self()->getStartLocation(), BWAPI::Filter::IsResourceDepot );
    ///   if ( pMain ) // check if pMain is valid
    ///   {
    ///     // Get sets of resources and workers
    ///     BWAPI::Unitset myResources = pMain->getUnitsInRadius(1024, BWAPI::Filter::IsMineralField);
    ///     if ( !myResources.empty() ) // check if we have resources nearby
    ///     {
    ///       BWAPI::Unitset myWorkers = pMain->getUnitsInRadius(512, BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle && BWAPI::Filter::IsOwned );
    ///       while ( !myWorkers.empty() ) // make sure we command all nearby idle workers, if any
    ///       {
    ///         for ( auto u = myResources.begin(); u != myResources.end() && !myWorkers.empty(); ++u )
    ///         {
    ///           myWorkers.back()->gather(*u);
    ///           myWorkers.pop_back();
    ///         }
    ///       }
    ///     } // myResources not empty
    ///   } // pMain != nullptr
    /// @endcode
    ///
    /// @see getClosestUnit, getUnitsInWeaponRange, Game::getUnitsInRadius, Game::getUnitsInRectangle
    Unitset getUnitsInRadius(int radius, const UnitFilter &pred = nullptr) const;

    /** Returns the set of units within weapon range of this unit. */
    Unitset getUnitsInWeaponRange(WeaponType weapon, const UnitFilter &pred = nullptr) const;

    // @TODO
    Unit getClosestUnit(const UnitFilter &pred = nullptr, int radius = 999999) const;

    /// Checks if the current unit is housing a @Nuke. This is only available for @Silos.
    ///
    /// @returns true if this unit has a @Nuke ready, and false if there is no @Nuke.
    virtual bool hasNuke() const = 0;

    /// Checks if the current unit is accelerating.
    ///
    /// @returns true if this unit is accelerating, and false otherwise
    virtual bool isAccelerating() const = 0;

    /// Checks if this unit is currently attacking something.
    ///
    /// @returns true if this unit is attacking another unit, and false if it is not.
    virtual bool isAttacking() const = 0;

    /// Checks if this unit is currently playing an attack animation. Issuing commands while this
    /// returns true may interrupt the unit's next attack sequence.
    ///
    /// @returns true if this unit is currently running an attack frame, and false if interrupting
    /// the unit is feasible.
    ///
    /// @note This function is only available to some unit types, specifically those that play
    /// special animations when they attack.
    virtual bool isAttackFrame() const = 0;

    /// Checks if the current unit is being constructed. This is mostly applicable to Terran
    /// structures which require an SCV to be constructing a structure.
    ///
    /// @retval true if this is either a Protoss structure, Zerg structure, or Terran structure
    /// being constructed by an attached SCV.
    /// @retval false if this is either completed, not a structure, or has no SCV constructing it
    ///
    /// @see UnitInterface::build, UnitInterface::cancelConstruction, UnitInterface::haltConstruction, UnitInterface::isConstructing
    bool isBeingConstructed() const;

    /// Checks this @Mineral_Field or @Refinery is currently being gathered from.
    ///
    /// @returns true if this unit is a resource container and being harvested by a worker, and
    /// false otherwise
    virtual bool isBeingGathered() const = 0;

    /// Checks if this unit is currently being healed by a @Medic or repaired by a @SCV.
    ///
    /// @returns true if this unit is being healed, and false otherwise.
    virtual bool isBeingHealed() const = 0;

    /// Checks if this unit is currently blinded by a @Medic 's @Optical_Flare ability. Blinded
    /// units have reduced sight range and cannot detect other units.
    ///
    /// @returns true if this unit is blind, and false otherwise
    virtual bool isBlind() const = 0;

    /// Checks if the current unit is slowing down to come to a stop.
    ///
    /// @returns true if this unit is breaking, false if it has stopped or is still moving at full
    /// speed.
    virtual bool isBraking() const = 0;

    /// Checks if the current unit is burrowed, either using the @Burrow ability, or is an armed
    /// @Spider_Mine .
    ///
    /// @returns true if this unit is burrowed, and false otherwise
    /// @see burrow, unburrow
    virtual bool isBurrowed() const = 0;

    /// Checks if this worker unit is carrying some vespene gas.
    ///
    /// @returns true if this is a worker unit carrying vespene gas, and false if it is either
    /// not a worker, or not carrying gas.
    ///
    /// Example
    /// @code
    ///   BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///   for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///   {
    ///     if ( u->isIdle() && (u->isCarryingGas() || u->isCarryingMinerals()) )
    ///       u->returnCargo();
    ///   }
    /// @endcode
    /// @implies isCompleted(), getType().isWorker()
    /// @see returnCargo, isGatheringGas, isCarryingMinerals
    virtual bool isCarryingGas() const = 0;

    /// Checks if this worker unit is carrying some minerals.
    ///
    /// @returns true if this is a worker unit carrying minerals, and false if it is either
    /// not a worker, or not carrying minerals.
    ///
    /// Example
    /// @code
    ///   BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///   for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///   {
    ///     if ( u->isIdle() && (u->isCarryingGas() || u->isCarryingMinerals()) )
    ///       u->returnCargo();
    ///   }
    /// @endcode
    /// @implies isCompleted(), getType().isWorker()
    /// @see returnCargo, isGatheringMinerals, isCarryingMinerals
    virtual bool isCarryingMinerals() const = 0;

    /// Checks if this unit is currently @cloaked.
    ///
    /// @returns true if this unit is cloaked, and false if it is visible.
    /// @see cloak, decloak
    virtual bool isCloaked() const = 0;

    /// Checks if this unit has finished being constructed, trained, morphed, or warped in, and can
    /// now receive orders.
    ///
    /// @returns true if this unit is completed, and false if it is under construction or inaccessible.
    virtual bool isCompleted() const = 0;

    /** Returns true when a unit has been issued an order to build a structure and is moving to the build
     * location. Also returns true for Terran SCVs while they construct a building.
     * \see UnitInterface::build, UnitInterface::cancelConstruction, UnitInterface::haltConstruction, UnitInterface::isBeingConstructed. */
    virtual bool isConstructing() const = 0;

    /// Checks if this unit has the @matrix effect.
    ///
    /// @returns true if the @matrix ability was used on this unit, and false otherwise.
    bool isDefenseMatrixed() const;

    /// Checks if this unit is visible or revealed by a detector unit. If this is false and
    /// #isVisible is true, then the unit is only partially visible and requires a detector in
    /// order to be targetted.
    ///
    /// @returns true if this unit is detected, and false if it needs a detector unit nearby in
    /// order to see it.
    /// @implies isVisible
    virtual bool isDetected() const = 0;

    /// Checks if the @Queen ability @Ensnare has been used on this unit.
    ///
    /// @returns true if the unit is ensnared, and false if it is not
    bool isEnsnared() const;

    /// This macro function checks if this unit is in the air. That is, the unit is either a flyer
    /// or a flying building.
    ///
    /// @returns true if this unit is in the air, and false if it is on the ground
    /// @see UnitType::isFlyer, UnitInterface::isLifted
    bool isFlying() const;

    /// Checks if this unit is following another unit. When a unit is following another unit, it
    /// simply moves where the other unit does, and does not attack enemies when it is following.
    ///
    /// @returns true if this unit is following another unit, and false if it is not
    /// @implies isCompleted
    /// @see follow, getTarget
    bool isFollowing() const;

    /// Checks if this unit is currently gathering gas. That is, the unit is either moving to a
    /// refinery, waiting to enter a refinery, harvesting from the refinery, or returning gas to a
    /// resource depot.
    ///
    /// @returns true if this unit is harvesting gas, and false if it is not
    /// @implies isCompleted, getType().isWorker()
    /// @see isCarryingGas
    virtual bool isGatheringGas() const = 0;

    /// Checks if this unit is currently harvesting minerals. That is, the unit is either moving
    /// to a @mineral_field, waiting to mine, mining minerals, or returning minerals to a resource
    /// depot.
    ///
    /// @returns true if this unit is gathering minerals, and false if it is not
    /// @implies isCompleted, getType().isWorker()
    /// @see isCarryingMinerals
    virtual bool isGatheringMinerals() const = 0;

    /// Checks if this unit is a hallucination. Hallucinations are created by the @High_Templar
    /// using the @Hallucination ability. Enemy hallucinations are unknown if
    /// Flag::CompleteMapInformation is disabled. Hallucinations have a time limit until they are
    /// destroyed (see UnitInterface::getRemoveTimer). 
    ///
    /// @returns true if the unit is a hallucination and false otherwise.
    /// @see getRemoveTimer
    virtual bool isHallucination() const = 0;

    /** Returns true if the unit is holding position
     * \see UnitInterface::holdPosition. */
    /// @todo may currently be wrong
    bool isHoldingPosition() const;

    /// Checks if this unit is not doing anything. This function is particularly useful when
    /// checking for units that aren't doing any tasks.
    ///
    /// @code
    ///   BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///   for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///   {
    ///     // Order idle worker to gather from closest mineral field
    ///     if ( u->getType().isWorker() && u->isIdle() )
    ///       u->gather( u->getClosestUnit( BWAPI::Filter::IsMineralField ) );
    ///   }
    /// @endcode
    ///
    /// @returns true if this unit is idle, and false if this unit is performing any action, such
    /// as moving or attacking
    /// @implies isCompleted
    /// @see UnitInterface::stop
    virtual bool isIdle() const = 0;

    /** Returns true if the unit can be interrupted. */
    virtual bool isInterruptible() const = 0;

    /// Checks the invincibility state for this unit.
    ///
    /// @returns true if this unit is currently invulnerable, and false if it is vulnerable
    virtual bool isInvincible() const = 0;

    /// Checks if the target unit can immediately be attacked by this unit in the current frame.
    ///
    /// @param target
    ///   The target unit to use in this check.
    ///
    /// @returns true if \p target is within weapon range of this unit's appropriate weapon, and 
    /// false otherwise.
    /// @retval false if \p target is invalid, inaccessible, too close, too far, or this unit does
    /// not have a weapon that can attack \p target.
    bool isInWeaponRange(Unit target) const;

    /// Checks if this unit is irradiated by a @Science_Vessel 's @Irradiate ability.
    ///
    /// @returns true if this unit is irradiated, and false otherwise
    ///
    /// Example usage:
    /// @code
    ///   BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///   for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///   {
    ///     if ( u->isIrradiated() && u->getIrradiateTimer > 50 && BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Restoration) )
    ///     {
    ///       BWAPI::Unit medic = u->getClosestUnit( BWAPI::Filter::GetType == BWAPI::UnitTypes::Terran_Medic &&
    ///                                              BWAPI::Filter::Energy >= BWAPI::TechTypes::Restoration.energyCost() );
    ///       if ( medic )
    ///         medic->useTech(BWAPI::TechTypes::Restoration, *u);
    ///     }
    ///   }
    /// @endcode
    /// @see getIrradiateTimer
    bool isIrradiated() const;

    /// Checks if this unit is a @Terran building and lifted off the ground. This function
    /// generally implies this->getType().isBuilding() and this->isCompleted() both return true.
    ///
    /// @returns true if this unit is a @Terran structure lifted off the ground.
    /// @implies isCompleted, getType().isFlyingBuilding()
    /// @see isFlying
    virtual bool isLifted() const = 0;

    /// Checks if this unit is currently loaded into another unit such as a @Transport.
    /// 
    /// @returns true if this unit is loaded in another one, and false otherwise
    /// @implies isCompleted
    /// @see load, unload, unloadAll
    bool isLoaded() const;

    /// Checks if this unit is currently @locked by a @Ghost.
    ///
    /// @returns true if this unit is locked down, and false otherwise
    /// @see getLockdownTimer
    bool isLockedDown() const;

    /// Checks if this unit has been @Maelstrommed by a @Dark_Archon.
    ///
    /// @returns true if this unit is maelstrommed, and false otherwise
    /// @see getMaelstromTimer
    bool isMaelstrommed() const;

    /// Finds out if the current unit is morphing or not. Zerg units and structures often have
    /// the ability to morph into different types of units. This function allows you to identify
    /// when this process is occurring.
    ///
    /// @retval true if the unit is currently morphing.
    /// @retval false if the unit is not morphing
    ///
    /// @see UnitInterface::morph, UnitInterface::cancelMorph, UnitInterface::getBuildType, UnitInterface::getRemainingBuildTime
    virtual bool isMorphing() const = 0;

    /// Checks if this unit is currently moving.
    ///
    /// @returns true if this unit is moving, and false if it is not
    /// @see stop
    virtual bool isMoving() const = 0;

    /// Checks if this unit has been parasited by some other player.
    /// 
    /// @returns true if this unit is inflicted with @parasite, and false if it is clean
    virtual bool isParasited() const = 0;

    /// Checks if this unit is patrolling between two positions.
    /// 
    /// @returns true if this unit is patrolling and false if it is not
    /// @see patrol
    bool isPatrolling() const;

    /// Checks if this unit has been been @plagued by a @defiler.
    ///
    /// @returns true if this unit is inflicted with @plague and is taking damage, and false if it
    /// is clean
    /// @see getPlagueTimer
    bool isPlagued() const;

    /// Checks if this unit is repairing or moving to @repair another unit. This is only applicable
    /// to @SCVs.
    ///
    /// @returns true if this unit is currently repairing or moving to @repair another unit, and
    /// false if it is not
    bool isRepairing() const;

    /** Returns true if the unit is a building that is researching tech. See TechTypes for the complete list
     * of available techs in Broodwar.
     * \see UnitInterface::research, UnitInterface::cancelResearch, UnitInterface::getTech, UnitInterface::getRemainingResearchTime. */
    bool isResearching() const;

    /** Returns true if the unit has been selected by the user via the starcraft GUI. Only available if you
     * enable Flag::UserInput during AIModule::onStart.
     * \see Game::getSelectedUnits. */
    virtual bool isSelected() const = 0;

    /// Checks if this unit is currently @sieged. This is only applicable to @Siege_Tanks.
    ///
    /// @returns true if the unit is in siege mode, and false if it is either not in siege mode or
    /// not a @Siege_Tank
    /// @see siege, unsiege
    bool isSieged() const;

    /** Returns true if the unit is starting to attack.
     * \see UnitInterface::attack, UnitInterface::getGroundWeaponCooldown, UnitInterface::getAirWeaponCooldown. */
    virtual bool isStartingAttack() const = 0;

    /** Returns true if the unit has been stasised by a Protoss Arbiter.
     * \see UnitInterface::getStasisTimer. */
    /// Checks if this unit is inflicted with @Stasis by an @Arbiter.
    ///
    /// @returns true if this unit is locked in a @Stasis and is unable to move, and false if it
    /// is free.
    ///
    /// @note This function does not necessarily imply that the unit is invincible, since there
    /// is a feature in the @UMS game type that allows stasised units to be vulnerable.
    ///
    /// @see getStasisTimer
    bool isStasised() const;

    /** Returns true if the unit is currently stimmed.
     * \see UnitInterface::getStimTimer. */
    bool isStimmed() const;

    /// Checks if this unit is currently trying to resolve a collision by randomly moving around.
    ///
    /// @returns true if this unit is currently stuck and trying to resolve a collision, and false
    /// if this unit is free
    virtual bool isStuck() const = 0;

    /** Returns true if the unit is training units (i.e. a Barracks training Marines).
     * \see UnitInterface::train, UnitInterface::getTrainingQueue, UnitInterface::cancelTrain, UnitInterface::getRemainingTrainTime. */
    virtual bool isTraining() const = 0;

    /** Returns true if the unit was recently attacked. */
    virtual bool isUnderAttack() const = 0;

    /** Returns true if the unit is under a Dark Swarm. */
    virtual bool isUnderDarkSwarm() const = 0;

    /** Returns true if the unit is under a Disruption Web. */
    virtual bool isUnderDisruptionWeb() const = 0;

    /** Returns true if the unit is under a Protoss Psionic Storm. */
    virtual bool isUnderStorm() const = 0;

    /** Returns true if the unit is a Protoss building that is unpowered because no pylons are in range. */
    virtual bool isUnpowered() const = 0;

    /** Returns true if the unit is a building that is upgrading. See UpgradeTypes for the complete list
     * of available upgrades in Broodwar.
     * \see UnitInterface::upgrade, UnitInterface::cancelUpgrade, UnitInterface::getUpgrade, UnitInterface::getRemainingUpgradeTime. */
    bool isUpgrading() const;

    /** Returns true if the unit is visible. If the CompleteMapInformation?  cheat flag is enabled, existing
     * units hidden by the fog of war will be accessible, but isVisible will still return false.
     * \see UnitInterface::exists. */
    virtual bool isVisible(Player player = nullptr) const = 0;

    /// Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// be targetted as the target unit of an unspecified command.
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can be a target.
    /// @retval false if an error occurred and the unit can not be a target.
    ///
    /// @see Game::getLastError, UnitInterface::canTargetUnit
    virtual bool isTargetable() const = 0;

    /// This function issues a command to the unit(s), however it is used for interfacing only,
    /// and is recommended to use one of the more specific command functions when writing an AI.
    ///
    /// @param command
    ///   A UnitCommand containing command parameters such as the type, position, target, etc.
    ///
    /// @retval true if BWAPI determined that the command was valid and passed it to Starcraft.
    /// @retval false if an error occured and the command could not be executed.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canIssueCommand
    virtual bool issueCommand(UnitCommand command) = 0;

    /// Orders the unit(s) to attack move to the specified position or attack the specified unit.
    ///
    /// @param target
    ///   A Position or a Unit to designate as the target. If a Position is used, the unit will
    ///   perform an Attack Move command.
    /// @param shiftQueueCommand
    ///   If this value is true, then the order will be queued instead of immediately executed.
    ///
    /// @retval true if BWAPI determined that the command was valid and passed it to Starcraft.
    /// @retval false if an error occured and the command could not be executed.
    ///
    /// @note A @Medic will use Heal Move instead of attack.
    ///
    /// @see Game::getLastError, UnitInterface::canAttack
    bool attack(PositionOrUnit target, bool shiftQueueCommand = false);

    /// Orders the worker unit(s) to construct a structure at a target position.
    ///
    /// @param type
    ///   The UnitType to build.
    /// @param target
    ///   A TilePosition to specify the build location, specifically the upper-left corner of the
    ///   location. If the target is not specified, then the function call will be redirected to
    ///   the train command.
    ///
    /// @retval true if BWAPI determined that the command was valid and passed it to Starcraft.
    /// @retval false if an error occured and the command could not be executed.
    ///
    /// @note You must have sufficient resources and meet the necessary requirements in order to
    /// build a structure.
    ///
    /// @see Game::getLastError, UnitInterface::train, UnitInterface::cancelConstruction, UnitInterface::canBuild
    bool build(UnitType type, TilePosition target = TilePositions::None);

    /// Orders the @Terran structure(s) to construct an add-on.
    ///
    /// @param type
    ///   The add-on UnitType to construct.
    ///
    /// @retval true if BWAPI determined that the command was valid and passed it to Starcraft.
    /// @retval false if an error occured and the command could not be executed.
    ///
    /// @note You must have sufficient resources and meet the necessary requirements in order to
    /// build a structure.
    ///
    /// @see Game::getLastError, UnitInterface::build, UnitInterface::cancelAddon, UnitInterface::canBuildAddon
    bool buildAddon(UnitType type);

    /// Orders the unit(s) to add a UnitType to its training queue, or morphs into the UnitType if
    /// it is @Zerg.
    ///
    /// @param type
    ///   The UnitType to train.
    ///
    /// @retval true if BWAPI determined that the command was valid and passed it to Starcraft.
    /// @retval false if an error occured and the command could not be executed.
    ///
    /// @note You must have sufficient resources, supply, and meet the necessary requirements in
    /// order to train a unit.
    /// @note This command is also used for training @Interceptors and @Scarabs.
    /// @note If you call this using a @Hatchery, @Lair, or @Hive, then it will automatically
    /// pass the command to one of its @Larvae.
    ///
    /// @see Game::getLastError, UnitInterface::build, UnitInterface::morph, UnitInterface::cancelTrain, UnitInterface::isTraining,
    /// UnitInterface::canTrain
    bool train(UnitType type = UnitTypes::None);

    /// Orders the unit(s) to morph into a different UnitType.
    ///
    /// @param type
    ///   The UnitType to morph into.
    ///
    /// @retval true if BWAPI determined that the command was valid and passed it to Starcraft.
    /// @retval false if an error occured and the command could not be executed.
    ///
    /// @see Game::getLastError, UnitInterface::build, UnitInterface::morph, UnitInterface::canMorph
    bool morph(UnitType type);

    /** Orders the unit to research the given tech type.
     * \see UnitInterface::cancelResearch, UnitInterface::Unit#isResearching, UnitInterface::getRemainingResearchTime, UnitInterface::getTech,
     * UnitInterface::canResearch. */
    bool research(TechType tech);

    /** Orders the unit to upgrade the given upgrade type.
     * \see UnitInterface::cancelUpgrade, UnitInterface::Unit#isUpgrading, UnitInterface::getRemainingUpgradeTime, UnitInterface::getUpgrade,
     * UnitInterface::canUpgrade. */
    bool upgrade(UpgradeType upgrade);

    /** Orders the unit to set its rally position to the specified position or unit.
     * \see UnitInterface::getRallyPosition, UnitInterface::getRallyUnit, UnitInterface::canSetRallyPoint, UnitInterface::canSetRallyPosition,
     * UnitInterface::canSetRallyUnit. */
    bool setRallyPoint(PositionOrUnit target);

    /** Orders the unit to move from its current position to the specified position.
     * \see UnitInterface::isMoving, UnitInterface::canMove. */
    bool move(Position target, bool shiftQueueCommand = false);

    /** Orders the unit to patrol between its current position and the specified position.
     * \see UnitInterface::isPatrolling, UnitInterface::canPatrol. */
    bool patrol(Position target, bool shiftQueueCommand = false);

    /** Orders the unit to hold its position.
     * \see UnitInterface::canHoldPosition. */
    bool holdPosition(bool shiftQueueCommand = false);

    /** Orders the unit to stop.
     * \see UnitInterface::canStop. */
    bool stop(bool shiftQueueCommand = false);

    /** Orders the unit to follow the specified unit.
     * \see UnitInterface::isFollowing, UnitInterface::canFollow. */
    bool follow(Unit target, bool shiftQueueCommand = false);

    /** Orders the unit to gather the specified unit (must be mineral or refinery type).
     * \see UnitInterface::isGatheringGas, UnitInterface::isGatheringMinerals, UnitInterface::canGather. */
    bool gather(Unit target, bool shiftQueueCommand = false);

    /** Orders the unit to return its cargo to a nearby resource depot such as a Command Center. Only
     * workers that are carrying minerals or gas can be ordered to return cargo.
     * \see UnitInterface::isCarryingGas, UnitInterface::isCarryingMinerals, UnitInterface::canReturnCargo. */
    bool returnCargo(bool shiftQueueCommand = false);

    /** Orders the unit to repair the specified unit. Only Terran SCVs can be ordered to repair, and the
     * target must be a mechanical Terran unit or building.
     * \see UnitInterface::isRepairing, UnitInterface::canRepair. */
    bool repair(Unit target, bool shiftQueueCommand = false);

    /** Orders the unit to burrow. Either the unit must be a Zerg Lurker, or the unit must be a Zerg ground
     * unit and burrow tech must be researched.
     * \see: UnitInterface::unburrow, UnitInterface::isBurrowed, UnitInterface::canBurrow. */
    bool burrow();

    /** Orders the burrowed unit to unburrow.
     * \see: UnitInterface::burrow, UnitInterface::isBurrowed, UnitInterface::canUnburrow. */
    bool unburrow();

    /** Orders the unit to cloak.
     * \see: UnitInterface::decloak, UnitInterface::isCloaked, UnitInterface::canCloak. */
    bool cloak();

    /** Orders the unit to decloak.
     * \see: UnitInterface::cloak, UnitInterface::isCloaked, UnitInterface::canDecloak. */
    bool decloak();

    /** Orders the unit to siege. Note: unit must be a Terran siege tank.
     * \see UnitInterface::unsiege, UnitInterface::isSieged, UnitInterface::canSiege. */
    bool siege();

    /** Orders the unit to unsiege. Note: unit must be a Terran siege tank.
     * \see: UnitInterface::unsiege, UnitInterface::isSieged, UnitInterface::canUnsiege. */
    bool unsiege();

    /** Orders the unit to lift. Note: unit must be a Terran building that can be lifted.
     * \see UnitInterface::land, UnitInterface::isLifted, UnitInterface::canLift. */
    bool lift();

    /** Orders the unit to land. Note: unit must be a Terran building that is currently lifted.
     * \see UnitInterface::lift, UnitInterface::isLifted, UnitInterface::canLand. */
    bool land(TilePosition target);

    /** Orders the unit to load the target unit.
     * \see UnitInterface::unload, UnitInterface::unloadAll, UnitInterface::getLoadedUnits, Unit:isLoaded. */
    bool load(Unit target, bool shiftQueueCommand = false);

    /** Orders the unit to unload the target unit. Unit should be a Terran Dropship, Protoss Shuttle,
     *  Zerg Overlord, or a Terran Bunker.
     * \see UnitInterface::load, UnitInterface::unloadAll, UnitInterface::getLoadedUnits, Unit:isLoaded, UnitInterface::canUnload,
     * UnitInterface::canUnloadAtPosition. */
    bool unload(Unit target);

    /** Orders the unit to unload all loaded units at the unit's current position. Unit should be a
     * Terran Dropship, Protoss Shuttle, or Zerg Overlord, or a Terran Bunker.
     * \see UnitInterface::load, UnitInterface::unload, UnitInterface::unloadAll, UnitInterface::getLoadedUnits, Unit:isLoaded,
     * UnitInterface::canUnloadAll, UnitInterface::canUnloadAtPosition. */
    bool unloadAll(bool shiftQueueCommand = false);

    /** Orders the unit to unload all loaded units at the specified location. Unit should be a Terran
     * Dropship, Protoss Shuttle, or Zerg Overlord (but not a Terran Bunker).
     * \see UnitInterface::load, UnitInterface::unload, UnitInterface::unloadAll, UnitInterface::getLoadedUnits, Unit:isLoaded,
     * Unit:canUnloadAllPosition, UnitInterface::canUnloadAtPosition. */
    bool unloadAll(Position target, bool shiftQueueCommand = false);

    /** Works like the right click in the GUI.
     * \see UnitInterface::canRightClick, UnitInterface::canRightClickPosition, UnitInterface::canRightClickUnit. */
    bool rightClick(PositionOrUnit target, bool shiftQueueCommand = false);

    /** Orders the SCV to stop constructing the building, and the building is left in a partially complete
     * state until it is canceled, destroyed, or completed.
     * \see UnitInterface::isConstructing, UnitInterface::canHaltConstruction. */
    bool haltConstruction();

    /** Orders the building to stop being constructed.
     * \see UnitInterface::beingConstructed, UnitInterface::canCancelConstruction. */
    bool cancelConstruction();

    /** Orders the unit to stop making the addon.
     * \see UnitInterface::canCancelAddon. */
    bool cancelAddon();

    /** Orders the unit to remove the specified unit from its training queue.
     * \see UnitInterface::train, UnitInterface::cancelTrain, UnitInterface::isTraining, UnitInterface::getTrainingQueue.
     * UnitInterface::canCancelTrain, UnitInterface::canCancelTrainSlot. */
    bool cancelTrain(int slot = -2);

    /** Orders the unit to stop morphing.
     * \see UnitInterface::morph, UnitInterface::isMorphing, UnitInterface::canCancelMorph. */
    bool cancelMorph();

    /** Orders the unit to cancel a research in progress.
     * \see UnitInterface::research, UnitInterface::isResearching, UnitInterface::getTech, UnitInterface::canCancelResearch. */
    bool cancelResearch();

    /** Orders the unit to cancel an upgrade in progress.
     * \see UnitInterface::upgrade, UnitInterface::isUpgrading, UnitInterface::getUpgrade, UnitInterface::canCancelUpgrade. */
    bool cancelUpgrade();
    
    /** Orders the unit to use a tech. The target may be a position, or a (non-null) unit, or
     * nullptr (which means the tech does not target another position/unit, e.g. @Stim_Packs ).
     * Returns true if it is a valid tech.
     * \see UnitInterface::canUseTechWithOrWithoutTarget, UnitInterface::canUseTech,
     * UnitInterface::canUseTechWithoutTarget, UnitInterface::canUseTechUnit, UnitInterface::canUseTechPosition. */
    bool useTech(TechType tech, PositionOrUnit target = nullptr);

    /** Moves a Flag Beacon to the target location.
     * \see UnitInterface::canPlaceCOP. */
    bool placeCOP(TilePosition target);

    /// Checks whether the unit is able to execute the given command. If you are calling this
    /// function repeatedly (e.g. to generate a collection of valid commands), you can avoid
    /// repeating the same kinds of checks by specifying false for some of the optional boolean
    /// arguments. Make sure that the state hasn't changed since the check was done though
    /// (eg a new frame/event, or a command issued). Also see the more specific functions.
    ///
    /// @param command
    ///   A UnitCommand to check.
    /// @param checkCanUseTechPositionOnPositions
    ///   Only used if the command type is UnitCommandTypes::Enum::Use_Tech_Position. A boolean
    ///   for whether to perform cheap checks for whether the unit is unable to target any
    ///   positions using the command's TechType (i.e. regardless of what the other command
    ///   parameters are). You can set this to false if you know this check has already just been
    ///   performed.
    /// @param checkCanUseTechUnitOnUnits
    ///   Only used if the command type is UnitCommandTypes::Enum::Use_Tech_Unit. A boolean for
    ///   whether to perform cheap checks for whether the unit is unable to target any units using
    ///   the command's TechType (i.e. regardless of what the other command parameters are). You
    ///   can set this to false if you know this check has already just been performed.
    /// @param checkCanBuildUnitType
    ///   Only used if the command type is UnitCommandTypes::Build. A boolean for whether to
    ///   perform cheap checks for whether the unit is unable to build the specified UnitType
    ///   (i.e. regardless of what the other command parameters are). You can set this to false if
    ///   you know this check has already just been performed.
    /// @param checkCanTargetUnit
    ///   Only used for command types that can target a unit. A boolean for whether to perform
    ///   UnitInterface::canTargetUnit as a check. You can set this to false if you know this check has
    ///   already just been performed.
    /// @param checkCanIssueCommandType
    ///   A boolean for whether to perform UnitInterface::canIssueCommandType as a check. You can set this
    ///   to false if you know this check has already just been performed.
    /// @param checkCommandibility
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    ///
    /// @retval true if BWAPI determined that the command is valid.
    /// @retval false if an error occurred and the command is invalid.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canCommand, UnitInterface::canIssueCommandType,
    /// UnitInterface::canTargetUnit
    virtual bool canIssueCommand(UnitCommand command, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute the given command as part of a Unitset
    /// (even if none of the units in the Unitset are able to execute the command individually).
    /// The reason this function exists is because some commands are valid for an individual unit
    /// but not for those individuals as a group (e.g. buildings, critters) and some commands are
    /// only valid for a unit if it is commanded as part of a unit group, e.g.:
    ///   1. attackMove/attackUnit for a Unitset, some of which can't attack, e.g. @High_Templar.
    ///      This is supported simply for consistency with BW's behaviour - you
    ///      could issue move command(s) individually instead.
    ///   2. attackMove/move/patrol/rightClickPosition for air unit(s) + e.g. @Larva, as part of
    ///      the air stacking technique. This is supported simply for consistency with BW's
    ///      behaviour - you could issue move/patrol/rightClickPosition command(s) for them
    ///      individually instead.
    ///
    /// @note BWAPI allows the following special cases to command a unit individually (rather than
    /// only allowing it to be commanded as part of a Unitset). These commands are not available
    /// to a user in BW when commanding units individually, but BWAPI allows them for convenience:
    ///   - attackMove for @Medic, which is equivalent to Heal Move.
    ///   - holdPosition for burrowed @Lurker, for ambushes.
    ///   - stop for @Larva, to move it to a different side of the @Hatchery / @Lair / @Hive (e.g.
    ///     so that @Drones morphed later morph nearer to minerals/gas).
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canIssueCommand,
    /// UnitInterface::canCommandGrouped, UnitInterface::canIssueCommandTypeGrouped, UnitInterface::canTargetUnit
    virtual bool canIssueCommandGrouped(UnitCommand command, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute any commands (eg the unit is stasised).
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can be commanded.
    /// @retval false if an error occurred and the unit can not be commanded.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand
    virtual bool canCommand() const = 0;

    /// Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute any commands as part of a Unitset (eg buildings, critters).
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can be commanded grouped.
    /// @retval false if an error occurred and the unit can not be commanded grouped.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canIssueCommand
    virtual bool canCommandGrouped(bool checkCommandibility = true) const = 0;

    /// Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute the given command type (i.e. regardless of what other possible command parameters
    /// could be).
    ///
    /// @param ct
    ///   A UnitCommandType.
    /// @param checkCommandibility
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    ///
    /// @retval true if BWAPI was unable to determine whether the command type is invalid.
    /// @retval false if an error occurred and the command type is invalid.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canIssueCommand
    virtual bool canIssueCommandType(UnitCommandType ct, bool checkCommandibility = true) const = 0;

    /// Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute the given command type (i.e. regardless of what other possible command parameters
    /// could be) as part of a Unitset.
    ///
    /// @param ct
    ///   A UnitCommandType.
    /// @param checkCommandibilityGrouped
    ///   A boolean for whether to perform UnitInterface::canCommandGrouped as a check. You can set this
    ///   to false if you know this check has already just been performed.
    /// @param checkCommandibility
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    ///
    /// @retval true if BWAPI was unable to determine whether the command type is invalid.
    /// @retval false if an error occurred and the command type is invalid.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canIssueCommandGrouped
    virtual bool canIssueCommandTypeGrouped(UnitCommandType ct, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// use the given unit as the target unit of an unspecified command.
    ///
    /// @param targetUnit
    ///   A target unit for an unspecified command.
    /// @param checkCommandibility
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can target the given unit.
    /// @retval false if an error occurred and the unit can not target the given unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::isTargetable
    virtual bool canTargetUnit(Unit targetUnit, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an attack command to attack-move or attack a unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack,
    /// UnitInterface::canAttackMove, UnitInterface::canAttackUnit
    virtual bool canAttack(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an attack command to attack-move or attack a (non-null)
    /// unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack,
    /// UnitInterface::canAttackMove, UnitInterface::canAttackUnit
    virtual bool canAttack(PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an attack command to attack-move or attack a unit,
    /// as part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttack
    virtual bool canAttackGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an attack command to attack-move or attack a
    /// (non-null) unit, as part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttack
    virtual bool canAttackGrouped(PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an attack command to attack-move.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack
    virtual bool canAttackMove(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an attack command to attack-move, as part of a
    /// Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttackMove
    virtual bool canAttackMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an attack command to attack a unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack
    virtual bool canAttackUnit(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an attack command to attack a unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack
    virtual bool canAttackUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an attack command to attack a unit,
    /// as part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttackUnit
    virtual bool canAttackUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an attack command to attack a unit, as part of
    /// a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttackUnit
    virtual bool canAttackUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a build command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::build
    virtual bool canBuild(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a build command for the given
    /// UnitType.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::build
    virtual bool canBuild(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a build command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::build
    virtual bool canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a buildAddon command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::buildAddon
    virtual bool canBuildAddon(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a buildAddon command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::buildAddon
    virtual bool canBuildAddon(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a train command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::train
    virtual bool canTrain(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a train command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::train
    virtual bool canTrain(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a morph command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::morph
    virtual bool canMorph(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a morph command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::morph
    virtual bool canMorph(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a research command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::research
    virtual bool canResearch(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a research command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::research
    virtual bool canResearch(TechType type, bool checkCanIssueCommandType = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an upgrade command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::upgrade
    virtual bool canUpgrade(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an upgrade command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::upgrade
    virtual bool canUpgrade(UpgradeType type, bool checkCanIssueCommandType = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a setRallyPoint command to a
    /// position or unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint,
    /// UnitInterface::canSetRallyPosition, UnitInterface::canSetRallyUnit.
    virtual bool canSetRallyPoint(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a setRallyPoint command to a position
    /// or (non-null) unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint,
    /// UnitInterface::canSetRallyPosition, UnitInterface::canSetRallyUnit.
    virtual bool canSetRallyPoint(PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a setRallyPoint command to a position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint
    virtual bool canSetRallyPosition(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a setRallyPoint command to a unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint
    virtual bool canSetRallyUnit(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a setRallyPoint command to a unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint
    virtual bool canSetRallyUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a move command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::move
    virtual bool canMove(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a move command, as part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canMove
    virtual bool canMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a patrol command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::patrol
    virtual bool canPatrol(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a patrol command, as part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canPatrol
    virtual bool canPatrolGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a follow command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::follow
    virtual bool canFollow(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a follow command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::follow
    virtual bool canFollow(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a gather command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::gather
    virtual bool canGather(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a gather command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::gather
    virtual bool canGather(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a returnCargo command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::returnCargo
    virtual bool canReturnCargo(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a holdPosition command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::holdPosition
    virtual bool canHoldPosition(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a stop command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::stop
    virtual bool canStop(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a repair command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::repair
    virtual bool canRepair(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a repair command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::repair
    virtual bool canRepair(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a burrow command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::burrow
    virtual bool canBurrow(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an unburrow command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unburrow
    virtual bool canUnburrow(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cloak command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cloak
    virtual bool canCloak(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a decloak command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::decloak
    virtual bool canDecloak(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a siege command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::siege
    virtual bool canSiege(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an unsiege command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unsiege
    virtual bool canUnsiege(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a lift command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::lift
    virtual bool canLift(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a land command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::land
    virtual bool canLand(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a land command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::land
    virtual bool canLand(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a load command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::load
    virtual bool canLoad(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a load command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::load
    virtual bool canLoad(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an unload command or unloadAll at
    /// current position command or unloadAll at a different position command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload, UnitInterface::unloadAll
    virtual bool canUnloadWithOrWithoutTarget(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an unload command or unloadAll at
    /// current position command or unloadAll at a different position command, for a given
    /// position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload, UnitInterface::unloadAll
    virtual bool canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an unload command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload
    virtual bool canUnload(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an unload command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload, UnitInterface::canUnloadAtPosition
    virtual bool canUnload(Unit targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an unloadAll command for the current position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unloadAll
    virtual bool canUnloadAll(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute an unloadAll command for a different
    /// position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unloadAll
    virtual bool canUnloadAllPosition(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute an unloadAll command for a different position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unloadAll
    virtual bool canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a rightClick command to a position
    /// or unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick,
    /// UnitInterface::canRightClickPosition, UnitInterface::canRightClickUnit.
    virtual bool canRightClick(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a rightClick command to a position or (non-null)
    /// unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick,
    /// UnitInterface::canRightClickPosition, UnitInterface::canRightClickUnit.
    virtual bool canRightClick(PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a rightClick command to a position
    /// or unit, as part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a rightClick command to a position or (non-null)
    /// unit, as part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickGrouped(PositionOrUnit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a rightClick command for a position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick
    virtual bool canRightClickPosition(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a rightClick command for a position, as part of
    /// a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClick
    virtual bool canRightClickPositionGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a rightClick command to a unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick
    virtual bool canRightClickUnit(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a rightClick command to a unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick
    virtual bool canRightClickUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a rightClick command to a unit, as
    /// part of a Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a rightClick command to a unit, as part of a
    /// Unitset.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a haltConstruction command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::haltConstruction
    virtual bool canHaltConstruction(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cancelConstruction command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelConstruction
    virtual bool canCancelConstruction(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cancelAddon command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelAddon
    virtual bool canCancelAddon(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cancelTrain command for any slot.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelTrain
    virtual bool canCancelTrain(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a cancelTrain command for an
    /// unspecified slot.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelTrain
    virtual bool canCancelTrainSlot(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cancelTrain command for a specified slot.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelTrain
    virtual bool canCancelTrainSlot(int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cancelMorph command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelMorph
    virtual bool canCancelMorph(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cancelResearch command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelResearch
    virtual bool canCancelResearch(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a cancelUpgrade command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelUpgrade
    virtual bool canCancelUpgrade(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a useTech command without a target or
    /// or a useTech command with a target position or a useTech command with a target unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechWithOrWithoutTarget(bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a useTech command without a target or
    /// or a useTech command with a target position or a useTech command with a target unit, for a
    /// given TechType.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechWithOrWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a useTech command for a specified position or
    /// unit (only specify nullptr if the TechType does not target another position/unit).
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech,
    /// UnitInterface::canUseTechWithoutTarget, UnitInterface::canUseTechUnit, UnitInterface::canUseTechPosition
    virtual bool canUseTech(BWAPI::TechType tech, PositionOrUnit target = nullptr, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a useTech command without a target.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a useTech command with an unspecified
    /// target unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechUnit(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a useTech command with a target unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechUnit(BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnits = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a useTech command with an unspecified target
    /// position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechPosition(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a useTech command with a target position.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechPosition(BWAPI::TechType tech, Position target, bool checkTargetsPositions = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// Cheap checks for whether the unit is able to execute a placeCOP command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::placeCOP
    virtual bool canPlaceCOP(bool checkCommandibility = true) const = 0;

    /// Checks whether the unit is able to execute a placeCOP command.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::placeCOP
    virtual bool canPlaceCOP(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
  };
}

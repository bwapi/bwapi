#pragma once
#include <BWAPI/Position.h>
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
  
  /// <summary>The Unit class is used to get information about individual units as well as issue
  /// orders to units.</summary> Each unit in the game has a unique Unit object, and Unit objects
  /// are not deleted until the end of the match (so you don't need to worry about unit pointers
  /// becoming invalid).
  ///
  /// Every Unit in the game is either accessible or inaccessible. To determine if an AI can access
  /// a particular unit, BWAPI checks to see if Flag::CompleteMapInformation is enabled. So there
  /// are two cases to consider - either the flag is enabled, or it is disabled:
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
  ///
  /// @ingroup Interface
  class UnitInterface : public Interface<UnitInterface>
  {
  protected:
    virtual ~UnitInterface() {};
  public:
    /// <summary>Retrieves a unique identifier for this unit.</summary>
    ///
    /// @returns An integer containing the unit's identifier.
    ///
    /// @see getReplayID
    virtual int getID() const = 0;

    /// <summary>Checks if the Unit exists in the view of the BWAPI player.</summary>
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

    /// <summary>Retrieves the unit identifier for this unit as seen in replay data.</summary>
    ///
    /// @note This is only available if Flag::CompleteMapInformation is enabled.
    ///
    /// @returns An integer containing the replay unit identifier.
    ///
    /// @see getID
    virtual int getReplayID() const = 0;

    /// <summary>Retrieves the player that owns this unit.</summary>
    ///
    /// @retval Game::neutral() If the unit is a neutral unit or inaccessible.
    ///
    /// @returns The owning Player interface object.
    virtual Player getPlayer() const = 0;

    /// <summary>Retrieves the unit's type.</summary>
    ///
    /// @retval UnitTypes::Unknown if this unit is inaccessible or cannot be determined.
    /// @returns A UnitType objects representing the unit's type.
    ///
    /// @see getInitialType
    virtual UnitType getType() const = 0;

    /// <summary>Retrieves the unit's position from the upper left corner of the map in pixels.
    /// </summary> The position returned is roughly the center if the unit.
    ///
    /// @note The unit bounds are defined as this value plus/minus the values of
    /// UnitType::dimensionLeft, UnitType::dimensionUp, UnitType::dimensionRight,
    /// and UnitType::dimensionDown, which is conveniently expressed in UnitInterface::getLeft,
    /// UnitInterface::getTop, UnitInterface::getRight, and UnitInterface::getBottom respectively.
    ///
    /// @retval Positions::Unknown if this unit is inaccessible.
    ///
    /// @returns Position object representing the unit's current position.
    ///
    /// @see getTilePosition, getInitialPosition, getLeft, getTop
    virtual Position getPosition() const = 0;

    /// <summary>Retrieves the unit's build position from the upper left corner of the map in
    /// tiles.</summary>
    ///
    /// @note: This tile position is the tile that is at the top left corner of the structure.
    ///
    /// @retval TilePositions::Unknown if this unit is inaccessible.
    ///
    /// @returns TilePosition object representing the unit's current tile position.
    ///
    /// @see getPosition, getInitialTilePosition
    TilePosition getTilePosition() const;

    /// <summary>Retrieves the unit's facing direction in radians.</summary>
    ///
    /// @note A value of 0.0 means the unit is facing east.
    ///
    /// @returns A double with the angle measure in radians.
    virtual double getAngle() const = 0;

    /// <summary>Retrieves the x component of the unit's velocity, measured in pixels per frame.</summary>
    ///
    /// @returns A double that represents the velocity's x component.
    ///
    /// @see getVelocityY
    virtual double getVelocityX() const = 0;

    /// <summary>Retrieves the y component of the unit's velocity, measured in pixels per frame.</summary>
    ///
    /// @returns A double that represents the velocity's y component.
    ///
    /// @see getVelocityX
    virtual double getVelocityY() const = 0;

    /// <summary>Retrieves the Region that the center of the unit is in.</summary>
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

    /// <summary>Retrieves the X coordinate of the unit's left boundary, measured in pixels from
    /// the left side of the map.</summary>
    ///
    /// @returns An integer representing the position of the left side of the unit.
    ///
    /// @see getTop, getRight, getBottom
    int getLeft() const;

    /// <summary>Retrieves the Y coordinate of the unit's top boundary, measured in pixels from
    /// the top of the map.</summary>
    ///
    /// @returns An integer representing the position of the top side of the unit.
    ///
    /// @see getLeft, getRight, getBottom
    int getTop() const;

    /// <summary>Retrieves the X coordinate of the unit's right boundary, measured in pixels from
    /// the left side of the map.</summary>
    ///
    /// @returns An integer representing the position of the right side of the unit.
    ///
    /// @see getLeft, getTop, getBottom
    int getRight() const;

    /// <summary>Retrieves the Y coordinate of the unit's bottom boundary, measured in pixels from
    /// the top of the map.</summary>
    ///
    /// @returns An integer representing the position of the bottom side of the unit.
    ///
    /// @see getLeft, getTop, getRight
    int getBottom() const;

    /// <summary>Retrieves the unit's current Hit Points (HP) as seen in the game.</summary>
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

    /// <summary>Retrieves the unit's current Shield Points (Shields) as seen in the game.</summary>
    ///
    /// @returns An integer representing the amount of shield points a unit currently has.
    ///
    /// @see UnitType::maxShields, getHitPoints
    virtual int getShields() const = 0;

    /// <summary>Retrieves the unit's current Energy Points (Energy) as seen in the game.</summary>
    ///
    /// @returns An integer representing the amount of energy points a unit currently has.
    ///
    /// @note Energy is required in order for units to use abilities.
    ///
    /// @see UnitType::maxEnergy
    virtual int getEnergy() const = 0;

    /// <summary>Retrieves the resource amount from a resource container, such as a Mineral Field
    /// and Vespene Geyser.</summary> If the unit is inaccessible, then the last known resource
    /// amount is returned.
    ///
    /// @returns An integer representing the last known amount of resources remaining in this
    /// resource.
    ///
    /// @see getInitialResources
    virtual int getResources() const = 0;

    /// <summary>Retrieves a grouping index from a resource container.</summary> Other resource
    /// containers of the same value are considered part of one expansion location (group of
    /// resources that are close together).
    ///
    /// @note This grouping method is explicitly determined by Starcraft itself and is used only
    /// by the internal AI.
    ///
    /// @returns An integer with an identifier between 0 and 250 that determine which resources
    /// are grouped together to form an expansion.
    virtual int getResourceGroup() const = 0;

    /// <summary>Retrieves the distance between this unit and a target position.</summary>
    ///
    /// @note Distance is calculated from the edge of this unit, using Starcraft's own distance
    /// algorithm. Ignores collisions.
    ///
    /// <param name="target">
    ///   A Position to calculate the distance to.
    /// </param>
    ///
    /// @returns An integer representation of the number of pixels between this unit and the
    /// \p target.
    int getDistance(Position target) const;

    /// <summary>Retrieves the distance between this unit and a target unit.</summary>
    ///
    /// @note Distance is calculated from the edge of this unit, using Starcraft's own distance
    /// algorithm. Ignores collisions.
    ///
    /// <param name="target">
    ///   A Unit to calculate the distance to. Calculate the distance to the edge of the target unit.
    /// </param>
    ///
    /// @returns An integer representation of the number of pixels between this unit and the
    /// \p target.
    int getDistance(Unit target) const;

    /// <summary>Using data provided by Starcraft, checks if there is a path available from this
    /// unit to the given target.</summary>
    ///
    /// @note This function only takes into account the terrain data, and does not include
    /// buildings when determining if a path is available. However, the complexity of this
    /// function is constant ( O(1) ), and no extensive calculations are necessary.
    ///
    /// @note If the current unit is an air unit, then this function will always return true.
    ///
    /// @note If the unit somehow gets stuck in unwalkable terrain, then this function may still
    /// return true if one of the unit's corners is on walkable terrain (i.e. if the unit is expected
    /// to return to the walkable terrain).
    ///
    /// <param name="target">
    ///   A Position or a Unit that is used to determine if this unit has a path to the target.
    /// </param>
    ///
    /// @returns true If there is a path between this unit and the target position, otherwise it will return false.
    /// @see Game::hasPath
    bool hasPath(Position target) const;

    /// <summary>Using data provided by Starcraft, checks if there is a path available from this
    /// unit to the given target.</summary>
    ///
    /// @note This function only takes into account the terrain data, and does not include
    /// buildings when determining if a path is available. However, the complexity of this
    /// function is constant ( O(1) ), and no extensive calculations are necessary.
    ///
    /// @note If the current unit is an air unit, then this function will always return true.
    ///
    /// <param name="target">
    ///   A Position or a Unit that is used to determine if this unit has a path to the target.
    /// </param>
    ///
    /// @retval true If there is a path between this unit and the target.
    /// @retval false If the target is on a different piece of land than this one (such as an
    /// island).
    bool hasPath(Unit target) const;

    /// <summary>Retrieves the frame number that sent the last successful command.</summary>
    /// 
    /// @note This value is comparable to Game::getFrameCount.
    ///
    /// @returns The frame number that sent the last successfully processed command to BWAPI.
    /// @see Game::getFrameCount, getLastCommand
    virtual int getLastCommandFrame() const = 0;

    /// <summary>Retrieves the last successful command that was sent to BWAPI.</summary>
    ///
    /// @returns A UnitCommand object containing information about the command that was processed.
    /// @see getLastCommandFrame
    virtual UnitCommand getLastCommand() const = 0;

    /// <summary>Retrieves the Player that last attacked this unit.</summary>
    /// 
    /// @returns Player interface object representing the player that last attacked this unit.
    /// @retval nullptr If this unit was not attacked.
    /// @implies exists()
    virtual BWAPI::Player getLastAttackingPlayer() const = 0;

    /// <summary>Retrieves the initial type of the unit.</summary> This is the type that the unit
    /// starts as in the beginning of the game. This is used to access the types of static neutral
    /// units such as mineral fields when they are not visible.
    ///
    /// @returns UnitType of this unit as it was when it was created.
    /// @retval UnitTypes::Unknown if this unit was not a static neutral unit in the beginning of
    /// the game.
    virtual UnitType getInitialType() const = 0;

    /// <summary>Retrieves the initial position of this unit.</summary> This is the position that
    /// the unit starts at in the beginning of the game. This is used to access the positions of
    /// static neutral units such as mineral fields when they are not visible.
    ///
    /// @returns Position indicating the unit's initial position when it was created.
    /// @retval Positions::Unknown if this unit was not a static neutral unit in the beginning of
    /// the game.
    virtual Position getInitialPosition() const = 0;

    /// <summary>Retrieves the initial build tile position of this unit.</summary> This is the tile
    /// position that the unit starts at in the beginning of the game. This is used to access the
    /// tile positions of static neutral units such as mineral fields when they are not visible.
    /// The build tile position corresponds to the upper left corner of the unit.
    ///
    /// @returns TilePosition indicating the unit's initial tile position when it was created.
    /// @retval TilePositions::Unknown if this unit was not a static neutral unit in the beginning of
    /// the game.
    virtual TilePosition getInitialTilePosition() const = 0;

    /// <summary>Retrieves the amount of hit points that this unit started off with at the
    /// beginning of the game.</summary> The unit must be neutral.
    ///
    /// @returns Number of hit points that this unit started with.
    /// @retval 0 if this unit was not a neutral unit at the beginning of the game.
    ///
    /// @note: It is possible for the unit's initial hit points to differ from the maximum hit
    /// points.
    ///
    /// @see Game::getStaticNeutralUnits
    virtual int getInitialHitPoints() const = 0;

    /// <summary>Retrieves the amount of resources contained in the unit at the beginning of the
    /// game.</summary> The unit must be a neutral resource container.
    ///
    /// @returns Amount of resources that this unit started with.
    /// @retval 0 if this unit was not a neutral unit at the beginning of the game, or if this
    /// unit does not contain resources. It is possible that the unit simply contains 0 resources.
    ///
    /// @see Game::getStaticNeutralUnits
    virtual int getInitialResources() const = 0;

    /// <summary>Retrieves the number of units that this unit has killed in total.</summary>
    ///
    /// @note The maximum amount of recorded kills per unit is 255.
    ///
    /// @returns integer indicating this unit's kill count.
    virtual int getKillCount() const = 0;

    /// <summary>Retrieves the number of acid spores that this unit is inflicted with.</summary>
    ///
    /// @returns Number of acid spores on this unit.
    virtual int getAcidSporeCount() const = 0;

    /// <summary>Retrieves the number of interceptors that this unit manages.</summary> This
    /// function is only for the @Carrier and its hero.
    ///
    /// @note This number may differ from the number of units returned from #getInterceptors. This
    /// occurs for cases in which you can see the number of enemy interceptors in the Carrier HUD,
    /// but don't actually have access to the individual interceptors.
    ///
    /// @returns Number of interceptors in this unit.
    /// @see getInterceptors
    virtual int getInterceptorCount() const = 0;

    /// <summary>Retrieves the number of scarabs that this unit has for use.</summary> This
    /// function is only for the @Reaver.
    ///
    /// @returns Number of scarabs this unit has ready.
    virtual int getScarabCount() const = 0;

    /// <summary>Retrieves the amount of @mines this unit has available.</summary> This function
    /// is only for the @Vulture.
    ///
    /// @returns Number of spider mines available for placement.
    virtual int getSpiderMineCount() const = 0;

    /// <summary>Retrieves the unit's ground weapon cooldown.</summary> This value decreases every
    /// frame, until it reaches 0. When the value is 0, this indicates that the unit is capable of
    /// using its ground weapon, otherwise it must wait until it reaches 0.
    ///
    /// @note This value will vary, because Starcraft adds an additional random value between
    /// (-1) and (+2) to the unit's weapon cooldown.
    ///
    /// @returns Number of frames needed for the unit's ground weapon to become available again.
    virtual int getGroundWeaponCooldown() const = 0;

    /// <summary>Retrieves the unit's air weapon cooldown.</summary> This value decreases every
    /// frame, until it reaches 0. When the value is 0, this indicates that the unit is capable of
    /// using its air weapon, otherwise it must wait until it reaches 0.
    ///
    /// @note This value will vary, because Starcraft adds an additional random value between
    /// (-1) and (+2) to the unit's weapon cooldown.
    ///
    /// @returns Number of frames needed for the unit's air weapon to become available again.
    virtual int getAirWeaponCooldown() const = 0;

    /// <summary>Retrieves the unit's ability cooldown.</summary> This value decreases every frame,
    /// until it reaches 0. When the value is 0, this indicates that the unit is capable of using
    /// one of its special abilities, otherwise it must wait until it reaches 0.
    ///
    /// @note This value will vary, because Starcraft adds an additional random value between
    /// (-1) and (+2) to the unit's ability cooldown.
    ///
    /// @returns Number of frames needed for the unit's abilities to become available again.
    virtual int getSpellCooldown() const = 0;

    /// <summary>Retrieves the amount of hit points remaining on the @matrix created by a
    /// @Science_Vessel.</summary> The @matrix ability starts with 250 hit points when it is used.
    ///
    /// @returns Number of hit points remaining on this unit's @matrix.
    ///
    /// @see getDefenseMatrixTimer, isDefenseMatrixed
    virtual int getDefenseMatrixPoints() const = 0;

    /// <summary>Retrieves the time, in frames, that the @matrix will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see getDefenseMatrixPoints, isDefenseMatrixed
    virtual int getDefenseMatrixTimer() const = 0;

    /// <summary>Retrieves the time, in frames, that @ensnare will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isEnsnared
    virtual int getEnsnareTimer() const = 0;

    /// <summary>Retrieves the time, in frames, that @irradiate will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isIrradiated
    virtual int getIrradiateTimer() const = 0;

    /// <summary>Retrieves the time, in frames, that @lockdown will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isLockdowned
    virtual int getLockdownTimer() const = 0;

    /// <summary>Retrieves the time, in frames, that @maelstrom will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isMaelstrommed
    virtual int getMaelstromTimer() const = 0;

    /// <summary>Retrieves an internal timer used for the primary order.</summary> Its use is
    /// specific to the order type that is currently assigned to the unit.
    ///
    /// @returns A value used as a timer for the primary order.
    /// @see getOrder
    virtual int getOrderTimer() const = 0;

    /// <summary>Retrieves the time, in frames, that @plague will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isPlagued
    virtual int getPlagueTimer() const = 0;

    /// <summary>Retrieves the time, in frames, until this temporary unit is destroyed or
    /// removed.</summary> This is used to determine the remaining time for the following units
    /// that were created by abilities:
    ///   - @hallucination
    ///   - @broodling
    ///   - @swarm
    ///   - @dweb
    ///   - @scanner
    ///   .
    /// Once this value reaches 0, the unit is destroyed.
    virtual int getRemoveTimer() const = 0;

    /// <summary>Retrieves the time, in frames, that @stasis will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isPlagued
    virtual int getStasisTimer() const = 0;

    /// <summary>Retrieves the time, in frames, that @stim will remain active on the current
    /// unit.</summary>
    ///
    /// @returns Number of frames remaining until the effect is removed.
    ///
    /// @see isPlagued
    virtual int getStimTimer() const = 0;

    /// <summary>Retrieves the building type that a @worker is about to construct.</summary> If
    /// the unit is morphing or is an incomplete structure, then this returns the UnitType that it
    /// will become when it has completed morphing/constructing.
    ///
    /// @returns UnitType indicating the type that a @worker is about to construct, or an
    /// incomplete unit will be when completed.
    virtual UnitType getBuildType() const = 0;

    /// <summary>Retrieves the list of units queued up to be trained.</summary>
    ///
    /// @returns a UnitType::list containing all the types that are in this factory's training
    /// queue, from oldest to most recent.
    /// @see train, cancelTrain, isTraining
    virtual UnitType::list getTrainingQueue() const = 0;

    /// <summary>Retrieves the technology that this unit is currently researching.</summary>
    ///
    /// @returns TechType indicating the technology being researched by this unit.
    /// @retval TechTypes::None if this unit is not researching anything.
    ///
    /// @see research, cancelResearch, isResearching, getRemainingResearchTime
    virtual TechType getTech() const = 0;

    /// <summary>Retrieves the upgrade that this unit is currently upgrading.</summary>
    ///
    /// @return UpgradeType indicating the upgrade in progress by this unit.
    /// @retval UpgradeTypes::None if this unit is not upgrading anything.
    ///
    /// @see upgrade, cancelUpgrade, isUpgrading, getRemainingUpgradeTime
    virtual UpgradeType getUpgrade() const = 0;

    /// <summary>Retrieves the remaining build time for a unit or structure that is being trained
    /// or constructed.</summary>
    ///
    /// @returns Number of frames remaining until the unit's completion.
    virtual int getRemainingBuildTime() const = 0;

    /// <summary>Retrieves the remaining time, in frames, of the unit that is currently being
    /// trained.</summary>
    ///
    /// @note If the unit is a @Hatchery, @Lair, or @Hive, this retrieves the amount of time until
    /// the next larva spawns.
    ///
    /// @returns Number of frames remaining until the current training unit becomes completed, or
    /// the number of frames remaining until the next larva spawns.
    /// @retval 0 If the unit is not training or has three larvae.
    /// @see train, getTrainingQueue
    virtual int getRemainingTrainTime() const = 0;

    /// <summary>Retrieves the amount of time until the unit is done researching its currently
    /// assigned TechType.</summary>
    ///
    /// @returns The remaining research time, in frames, for the current technology being
    /// researched by this unit.
    /// @retval 0 If the unit is not researching anything.
    ///
    /// @see research, cancelResearch, isResearching, getTech
    virtual int getRemainingResearchTime() const = 0;

    /// <summary>Retrieves the amount of time until the unit is done upgrading its current upgrade.</summary>
    ///
    /// @returns The remaining upgrade time, in frames, for the current upgrade.
    /// @retval 0 If the unit is not upgrading anything.
    ///
    /// @see upgrade, cancelUpgrade, isUpgrading, getUpgrade
    virtual int getRemainingUpgradeTime() const = 0;

    /// <summary>Retrieves the unit currently being trained, or the corresponding paired unit for
    /// @SCVs and @Terran structures, depending on the context.</summary>
    /// For example, if this unit is a @Factory under construction, this function will return the
    /// @SCV that is constructing it. If this unit is a @SCV, then it will return the structure it
    /// is currently constructing. If this unit is a @Nexus, and it is training a @Probe, then the
    /// probe will be returned.
    ///
    /// @bug This will return an incorrect unit when called on @Reavers.
    /// 
    /// @returns Paired build unit that is either constructing this unit, structure being constructed by
    /// this unit, or the unit that is being trained by this structure.
    /// @retval nullptr If there is no unit constructing this one, or this unit is not constructing
    /// another unit.
    virtual Unit getBuildUnit() const = 0;

    /// <summary>Generally returns the appropriate target unit after issuing an order that accepts
    /// a target unit (i.e. attack, repair, gather, etc.).</summary> To get a target that has been
    /// acquired automatically without issuing an order, use getOrderTarget.
    ///
    /// @returns Unit that is currently being targeted by this unit.
    /// @see getOrderTarget
    virtual Unit getTarget() const = 0;

    /// <summary>Retrieves the target position the unit is moving to, provided a valid path to the
    /// target position exists.</summary>
    ///
    /// @returns Target position of a movement action.
    virtual Position getTargetPosition() const = 0;

    /// <summary>Retrieves the primary Order that the unit is assigned.</summary> Primary orders
    /// are distinct actions such as Orders::AttackUnit and Orders::PlayerGuard.
    ///
    /// @returns The primary Order that the unit is executing.
    virtual Order getOrder() const = 0;

    /// <summary>Retrieves the secondary Order that the unit is assigned.</summary> Secondary
    /// orders are run in the background as a sub-order. An example would be Orders::TrainFighter,
    /// because a @Carrier can move and train fighters at the same time.
    ///
    /// @returns The secondary Order that the unit is executing.
    virtual Order getSecondaryOrder() const = 0;

    /// <summary>Retrieves the unit's primary order target.</summary> This is usually set when the
    /// low level unit AI acquires a new target automatically. For example if an enemy @Probe
    /// comes in range of your @Marine, the @Marine will start attacking it, and getOrderTarget
    /// will be set in this case, but not getTarget.
    ///
    /// @returns The Unit that this unit is currently targetting.
    /// @see getTarget, getOrder
    virtual Unit getOrderTarget() const = 0;

    /// <summary>Retrieves the target position for the unit's order.</summary> For example, when
    /// Orders::Move is assigned, getTargetPosition returns the end of the unit's path, but this
    /// returns the location that the unit is trying to move to.
    ///
    /// @returns Position that this unit is currently targetting.
    /// @see getTargetPosition, getOrder
    virtual Position getOrderTargetPosition() const = 0;

    /// <summary>Retrieves the position the structure is rallying units to once they are
    /// completed.</summary>
    ///
    /// @returns Position that a completed unit coming from this structure will travel to.
    /// @retval Positions::None If this building does not produce units.
    ///
    /// @note If getRallyUnit is valid, then this value is ignored.
    ///
    /// @see setRallyPoint, getRallyUnit
    virtual Position getRallyPosition() const = 0;

    /// <summary>Retrieves the unit the structure is rallying units to once they are completed.</summary>
    /// Units will then follow the targetted unit.
    ///
    /// @returns Unit that a completed unit coming from this structure will travel to.
    /// @retval nullptr If the structure is not rallied to a unit or it does not produce units.
    ///
    /// @note A rallied unit takes precedence over a rallied position. That is if the return value
    /// is valid(non-null), then getRallyPosition is ignored.
    ///
    /// @see setRallyPoint, getRallyPosition
    virtual Unit getRallyUnit() const = 0;

    /// <summary>Retrieves the add-on that is attached to this unit.</summary>
    ///
    /// @returns Unit interface that represents the add-on that is attached to this unit.
    /// @retval nullptr if this unit does not have an add-on.
    virtual Unit getAddon() const = 0;

    /// <summary>Retrieves the @Nydus_Canal that is attached to this one.</summary> Every
    /// @Nydus_Canal can place a "Nydus Exit" which, when connected, can be travelled through by
    /// @Zerg units.
    ///
    /// @returns Unit interface representing the @Nydus_Canal connected to this one.
    /// @retval nullptr if the unit is not a @Nydus_Canal, is not owned, or has not placed a Nydus
    /// Exit.
    virtual Unit getNydusExit() const = 0;

    /// <summary>Retrieves the power-up that the worker unit is holding.</summary> Power-ups are
    /// special units such as the @Flag in the @CTF game type, which can be picked up by worker
    /// units.
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

    /// <summary>Retrieves the @Transport or @Bunker unit that has this unit loaded inside of it.</summary>
    ///
    /// @returns Unit interface object representing the @Transport containing this unit.
    /// @retval nullptr if this unit is not in a @Transport.
    virtual Unit getTransport() const = 0;

    /// <summary>Retrieves the set of units that are contained within this @Bunker or @Transport.</summary>
    ///
    /// @returns A Unitset object containing all of the units that are loaded inside of the
    /// current unit.
    virtual Unitset getLoadedUnits() const = 0;

    /// <summary>Retrieves the remaining unit-space available for @Bunkers and @Transports.</summary>
    ///
    /// @returns The number of spots available to transport a unit.
    ///
    /// @see getLoadedUnits
    int getSpaceRemaining() const;

    /// <summary>Retrieves the parent @Carrier that owns this @Interceptor.</summary>
    ///
    /// @returns The parent @Carrier unit that has ownership of this one.
    /// @retval nullptr if the current unit is not an @Interceptor.
    virtual Unit getCarrier() const = 0;

    /// <summary>Retrieves the set of @Interceptors controlled by this unit.</summary> This is
    /// intended for @Carriers and its hero.
    ///
    /// @returns Unitset containing @Interceptor units owned by this carrier.
    /// @see getInterceptorCount
    virtual Unitset getInterceptors() const = 0;

    /// <summary>Retrieves the parent @Hatchery, @Lair, or @Hive that owns this particular unit.</summary>
    /// This is intended for @Larvae.
    ///
    /// @returns Hatchery unit that has ownership of this larva.
    /// @retval nullptr if the current unit is not a @Larva or has no parent.
    /// @see getLarva
    virtual Unit getHatchery() const = 0;

    /// <summary>Retrieves the set of @Larvae that were spawned by this unit.</summary> Only
    /// @Hatcheries, @Lairs, and @Hives are capable of spawning @Larvae. This is like clicking the
    /// "Select Larva" button and getting the selection of @Larvae.
    ///
    /// @returns Unitset containing @Larva units owned by this unit. The set will be empty if
    /// there are none.
    /// @see getHatchery
    virtual Unitset getLarva() const = 0;

    /// <summary>Retrieves the set of all units in a given radius of the current unit.</summary>
    ///
    /// Takes into account this unit's dimensions. Can optionally specify a filter that is composed
    /// using BWAPI Filter semantics to include only specific units (such as only ground units, etc.)
    ///
    /// <param name="radius">
    ///   The radius, in pixels, to search for units.
    /// </param>
    /// <param name="pred"> (optional)
    ///   The composed function predicate to include only specific (desired) units in the set. Defaults to
    ///   nullptr, which means no filter.
    /// </param>
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

    /// <summary>Obtains the set of units within weapon range of this unit.</summary>
    ///
    /// <param name="weapon">
    ///     The weapon type to use as a filter for distance and units that can be hit by it.
    /// </param>
    /// <param name="pred"> (optional)
    ///     A predicate used as an additional filter. If omitted, no additional filter is used.
    /// </param>
    ///
    /// @see getUnitsInRadius, getClosestUnit, Game::getUnitsInRadius, Game::getUnitsInRectangle
    Unitset getUnitsInWeaponRange(WeaponType weapon, const UnitFilter &pred = nullptr) const;

    /// <summary>Retrieves the closest unit to this one.</summary>
    /// 
    /// <param name="pred"> (optional)
    ///     A function predicate used to identify which conditions must be matched for a unit to
    ///     be considered. If omitted, then the closest unit owned by any player will be returned.
    /// </param>
    /// <param name="radius"> (optional)
    ///     The maximum radius to check for the closest unit. For performance reasons, a developer
    ///     can limit the radius that is checked. If omitted, then the entire map is checked.
    /// </param>
    ///
    /// @see getUnitsInRadius, Game::getUnitsInRadius, Game::getUnitsInRectangle
    Unit getClosestUnit(const UnitFilter &pred = nullptr, int radius = 999999) const;

    /// <summary>Checks if the current unit is housing a @Nuke.</summary> This is only available
    /// for @Silos.
    ///
    /// @returns true if this unit has a @Nuke ready, and false if there is no @Nuke.
    virtual bool hasNuke() const = 0;

    /// <summary>Checks if the current unit is accelerating.</summary>
    ///
    /// @returns true if this unit is accelerating, and false otherwise
    virtual bool isAccelerating() const = 0;

    /// <summary>Checks if this unit is currently attacking something.</summary>
    ///
    /// @returns true if this unit is attacking another unit, and false if it is not.
    virtual bool isAttacking() const = 0;

    /// <summary>Checks if this unit is currently playing an attack animation.</summary> Issuing
    /// commands while this returns true may interrupt the unit's next attack sequence.
    ///
    /// @returns true if this unit is currently running an attack frame, and false if interrupting
    /// the unit is feasible.
    ///
    /// @note This function is only available to some unit types, specifically those that play
    /// special animations when they attack.
    virtual bool isAttackFrame() const = 0;

    /// <summary>Checks if the current unit is being constructed.</summary> This is mostly
    /// applicable to Terran structures which require an SCV to be constructing a structure.
    ///
    /// @retval true if this is either a Protoss structure, Zerg structure, or Terran structure
    /// being constructed by an attached SCV.
    /// @retval false if this is either completed, not a structure, or has no SCV constructing it
    ///
    /// @see build, cancelConstruction, haltConstruction, isConstructing
    bool isBeingConstructed() const;

    /// <summary>Checks this @Mineral_Field or @Refinery is currently being gathered from.</summary>
    ///
    /// @returns true if this unit is a resource container and being harvested by a worker, and
    /// false otherwise
    virtual bool isBeingGathered() const = 0;

    /// <summary>Checks if this unit is currently being healed by a @Medic or repaired by a @SCV.</summary>
    ///
    /// @returns true if this unit is being healed, and false otherwise.
    virtual bool isBeingHealed() const = 0;

    /// <summary>Checks if this unit is currently blinded by a @Medic 's @Optical_Flare ability.</summary>
    /// Blinded units have reduced sight range and cannot detect other units.
    ///
    /// @returns true if this unit is blind, and false otherwise
    virtual bool isBlind() const = 0;

    /// <summary>Checks if the current unit is slowing down to come to a stop.</summary>
    ///
    /// @returns true if this unit is breaking, false if it has stopped or is still moving at full
    /// speed.
    virtual bool isBraking() const = 0;

    /// <summary>Checks if the current unit is burrowed, either using the @Burrow ability, or is
    /// an armed @Spider_Mine.</summary>
    ///
    /// @returns true if this unit is burrowed, and false otherwise
    /// @see burrow, unburrow
    virtual bool isBurrowed() const = 0;

    /// <summary>Checks if this worker unit is carrying some vespene gas.</summary>
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

    /// <summary>Checks if this worker unit is carrying some minerals.</summary>
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

    /// <summary>Checks if this unit is currently @cloaked.</summary>
    ///
    /// @returns true if this unit is cloaked, and false if it is visible.
    /// @see cloak, decloak
    virtual bool isCloaked() const = 0;

    /// <summary>Checks if this unit has finished being constructed, trained, morphed, or warped
    /// in, and can now receive orders.</summary>
    ///
    /// @returns true if this unit is completed, and false if it is under construction or inaccessible.
    virtual bool isCompleted() const = 0;

    /// <summary>Checks if a unit is either constructing something or moving to construct something.</summary>
    ///
    /// @returns true when a unit has been issued an order to build a structure and is moving to
    /// the build location, or is currently constructing something.
    ///
    /// @see isBeingConstructed, build, cancelConstruction, haltConstruction
    virtual bool isConstructing() const = 0;

    /// <summary>Checks if this unit has the @matrix effect.</summary>
    ///
    /// @returns true if the @matrix ability was used on this unit, and false otherwise.
    bool isDefenseMatrixed() const;

    /// <summary>Checks if this unit is visible or revealed by a detector unit.</summary> If this
    /// is false and #isVisible is true, then the unit is only partially visible and requires a
    /// detector in order to be targetted.
    ///
    /// @returns true if this unit is detected, and false if it needs a detector unit nearby in
    /// order to see it.
    /// @implies isVisible
    virtual bool isDetected() const = 0;

    /// <summary>Checks if the @Queen ability @Ensnare has been used on this unit.</summary>
    ///
    /// @returns true if the unit is ensnared, and false if it is not
    bool isEnsnared() const;

    /// <summary>This macro function checks if this unit is in the air. That is, the unit is
    /// either a flyer or a flying building.</summary>
    ///
    /// @returns true if this unit is in the air, and false if it is on the ground
    /// @see UnitType::isFlyer, UnitInterface::isLifted
    bool isFlying() const;

    /// <summary>Checks if this unit is following another unit.</summary> When a unit is following
    /// another unit, it simply moves where the other unit does, and does not attack enemies when
    /// it is following.
    ///
    /// @returns true if this unit is following another unit, and false if it is not
    /// @implies isCompleted
    /// @see follow, getTarget
    bool isFollowing() const;

    /// <summary>Checks if this unit is currently gathering gas.</summary> That is, the unit is
    /// either moving to a refinery, waiting to enter a refinery, harvesting from the refinery, or
    /// returning gas to a resource depot.
    ///
    /// @returns true if this unit is harvesting gas, and false if it is not
    /// @implies isCompleted, getType().isWorker()
    /// @see isCarryingGas
    virtual bool isGatheringGas() const = 0;

    /// <summary>Checks if this unit is currently harvesting minerals.</summary> That is, the unit
    /// is either moving to a @mineral_field, waiting to mine, mining minerals, or returning
    /// minerals to a resource depot.
    ///
    /// @returns true if this unit is gathering minerals, and false if it is not
    /// @implies isCompleted, getType().isWorker()
    /// @see isCarryingMinerals
    virtual bool isGatheringMinerals() const = 0;

    /// <summary>Checks if this unit is a hallucination.</summary> Hallucinations are created by
    /// the @High_Templar using the @Hallucination ability. Enemy hallucinations are unknown if
    /// Flag::CompleteMapInformation is disabled. Hallucinations have a time limit until they are
    /// destroyed (see UnitInterface::getRemoveTimer). 
    ///
    /// @returns true if the unit is a hallucination and false otherwise.
    /// @see getRemoveTimer
    virtual bool isHallucination() const = 0;

    /// <summary>Checks if the unit is currently holding position.</summary> A unit that is holding
    /// position will attack other units, but will not chase after them.
    ///
    /// @returns true if this unit is holding position, and false if it is not.
    ///
    /// @see holdPosition
    bool isHoldingPosition() const;

    /// <summary>Checks if this unit is running an idle order.</summary> This function is
    /// particularly useful when checking for units that aren't doing any tasks that you assigned.
    ///
    /// A unit is considered idle if it is <b>not</b> doing any of the following:
    ///   - Training
    ///   - Constructing
    ///   - Morphing
    ///   - Researching
    ///   - Upgrading
    ///
    /// In <b>addition</b> to running one of the following orders:
    ///   - Orders::PlayerGuard: Player unit idle.
    ///   - Orders::Guard: Generic unit idle.
    ///   - Orders::Stop
    ///   - Orders::PickupIdle
    ///   - Orders::Nothing: Structure/generic idle.
    ///   - Orders::Medic: Medic idle.
    ///   - Orders::Carrier: Carrier idle.
    ///   - Orders::Reaver: Reaver idle.
    ///   - Orders::Critter: Critter idle.
    ///   - Orders::Neutral: Neutral unit idle.
    ///   - Orders::TowerGuard: Turret structure idle.
    ///   - Orders::Burrowed: Burrowed unit idle.
    ///   - Orders::NukeTrain
    ///   - Orders::Larva: Larva idle.
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

    /// <summary>Checks if the unit can be interrupted.</summary>
    ///
    /// @returns true if this unit can be interrupted, or false if this unit is uninterruptable
    virtual bool isInterruptible() const = 0;

    /// <summary>Checks the invincibility state for this unit.</summary>
    ///
    /// @returns true if this unit is currently invulnerable, and false if it is vulnerable
    virtual bool isInvincible() const = 0;

    /// <summary>Checks if the target unit can immediately be attacked by this unit in the current
    /// frame.</summary>
    ///
    /// <param name="target">
    ///   The target unit to use in this check.
    /// </param>
    ///
    /// @returns true if \p target is within weapon range of this unit's appropriate weapon, and 
    /// false otherwise.
    /// @retval false if \p target is invalid, inaccessible, too close, too far, or this unit does
    /// not have a weapon that can attack \p target.
    bool isInWeaponRange(Unit target) const;

    /// <summary>Checks if this unit is irradiated by a @Science_Vessel 's @Irradiate ability.</summary>
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

    /// <summary>Checks if this unit is a @Terran building and lifted off the ground.</summary>
    /// This function generally implies this->getType().isBuilding() and this->isCompleted() both
    /// return true.
    ///
    /// @returns true if this unit is a @Terran structure lifted off the ground.
    /// @implies isCompleted, getType().isFlyingBuilding()
    /// @see isFlying
    virtual bool isLifted() const = 0;

    /// <summary>Checks if this unit is currently loaded into another unit such as a @Transport.</summary>
    /// 
    /// @returns true if this unit is loaded in another one, and false otherwise
    /// @implies isCompleted
    /// @see load, unload, unloadAll
    bool isLoaded() const;

    /// <summary>Checks if this unit is currently @locked by a @Ghost.</summary>
    ///
    /// @returns true if this unit is locked down, and false otherwise
    /// @see getLockdownTimer
    bool isLockedDown() const;

    /// <summary>Checks if this unit has been @Maelstrommed by a @Dark_Archon.</summary>
    ///
    /// @returns true if this unit is maelstrommed, and false otherwise
    /// @see getMaelstromTimer
    bool isMaelstrommed() const;

    /// <summary>Finds out if the current unit is morphing or not.<summary> @Zerg units and
    /// structures often have the ability to #morph into different types of units. This function
    /// allows you to identify when this process is occurring.
    ///
    /// @retval true if the unit is currently morphing.
    /// @retval false if the unit is not morphing
    ///
    /// @see morph, cancelMorph, getBuildType, getRemainingBuildTime
    virtual bool isMorphing() const = 0;

    /// <summary>Checks if this unit is currently moving.</summary>
    ///
    /// @returns true if this unit is moving, and false if it is not
    /// @see stop
    virtual bool isMoving() const = 0;

    /// <summary>Checks if this unit has been parasited by some other player.</summary>
    /// 
    /// @returns true if this unit is inflicted with @parasite, and false if it is clean
    virtual bool isParasited() const = 0;

    /// <summary>Checks if this unit is patrolling between two positions.</summary>
    /// 
    /// @returns true if this unit is patrolling and false if it is not
    /// @see patrol
    bool isPatrolling() const;

    /// <summary>Checks if this unit has been been @plagued by a @defiler.</summary>
    ///
    /// @returns true if this unit is inflicted with @plague and is taking damage, and false if it
    /// is clean
    /// @see getPlagueTimer
    bool isPlagued() const;

    /// <summary>Checks if this unit is repairing or moving to @repair another unit.</summary>
    /// This is only applicable to @SCVs.
    ///
    /// @returns true if this unit is currently repairing or moving to @repair another unit, and
    /// false if it is not
    bool isRepairing() const;

    /// <summary>Checks if this unit is a structure that is currently researching a technology.</summary>
    /// See TechTypes for a complete list of technologies in Broodwar.
    ///
    /// @returns true if this structure is researching a technology, false otherwise
    /// @see research, cancelResearch, getTech, getRemainingResearchTime, 
    /// @implies exists(), isCompleted(), getType().isBuilding()
    bool isResearching() const;

    /// <summary>Checks if this unit has been selected in the user interface.</summary> This
    /// function is only available if the flag Flag::UserInput is enabled.
    ///
    /// @returns true if this unit is currently selected, and false if this unit is not selected
    /// @see Game::getSelectedUnits
    virtual bool isSelected() const = 0;

    /// <summary>Checks if this unit is currently @sieged.</summary> This is only applicable to
    /// @Siege_Tanks.
    ///
    /// @returns true if the unit is in siege mode, and false if it is either not in siege mode or
    /// not a @Siege_Tank
    /// @see siege, unsiege
    bool isSieged() const;

    /// <summary>Checks if the unit is starting to attack.</summary>
    ///
    /// @returns true if this unit is starting an attack.
    ///
    /// @see attack, getGroundWeaponCooldown, getAirWeaponCooldown
    virtual bool isStartingAttack() const = 0;

    /// <summary>Checks if this unit is inflicted with @Stasis by an @Arbiter.</summary>
    ///
    /// @returns true if this unit is locked in a @Stasis and is unable to move, and false if it
    /// is free.
    ///
    /// @note This function does not necessarily imply that the unit is invincible, since there
    /// is a feature in the @UMS game type that allows stasised units to be vulnerable.
    ///
    /// @see getStasisTimer
    bool isStasised() const;

    /// <summary>Checks if this unit is currently under the influence of a @Stim_Pack.</summary>
    ///
    /// @returns true if this unit has used a stim pack, false otherwise
    /// @see getStimTimer
    bool isStimmed() const;

    /// <summary>Checks if this unit is currently trying to resolve a collision by randomly moving
    /// around.</summary>
    ///
    /// @returns true if this unit is currently stuck and trying to resolve a collision, and false
    /// if this unit is free
    virtual bool isStuck() const = 0;

    /// <summary>Checks if this unit is training a new unit.</summary> For example, a @Barracks
    /// training a @Marine.
    ///
    /// @note It is possible for a unit to remain in the training queue with no progress. In that
    /// case, this function will return false because of supply or unit count limitations.
    ///
    /// @returns true if this unit is currently training another unit, and false otherwise.
    ///
    /// @see train, getTrainingQueue, cancelTrain, getRemainingTrainTime
    virtual bool isTraining() const = 0;

    /// <summary>Checks if the current unit is being attacked.</summary> Has a small delay before
    /// this returns false
    /// again when the unit is no longer being attacked.
    ///
    /// @returns true if this unit has been attacked within the past few frames, and false
    /// if it has not
    virtual bool isUnderAttack() const = 0;

    /// <summary>Checks if this unit is under the cover of a @Dark_Swarm.</summary>
    ///
    /// @returns true if this unit is protected by a @Dark_Swarm, and false if it is not
    virtual bool isUnderDarkSwarm() const = 0;

    /// <summary>Checks if this unit is currently being affected by a @Disruption_Web.</summary>
    ///
    /// @returns true if this unit is under the effects of @Disruption_Web.
    virtual bool isUnderDisruptionWeb() const = 0;

    /// <summary>Checks if this unit is currently taking damage from a @Psi_Storm.</summary>
    ///
    /// @returns true if this unit is losing hit points from a @Psi_Storm, and false otherwise.
    virtual bool isUnderStorm() const = 0;

    /// <summary>Checks if this unit has power.</summary> Most structures are powered by default,
    /// but @Protoss structures require a @Pylon to be powered and functional.
    ///
    /// @returns true if this unit has power or is inaccessible, and false if this unit is
    /// unpowered.
    ///
    /// @since 4.0.1 Beta (previously isUnpowered)
    virtual bool isPowered() const = 0;

    /// <summary>Checks if this unit is a structure that is currently upgrading an upgrade.</summary>
    /// See UpgradeTypes for a full list of upgrades in Broodwar.
    ///
    /// @returns true if this structure is upgrading, false otherwise
    /// @see upgrade, cancelUpgrade, getUpgrade, getRemainingUpgradeTime
    /// @implies exists(), isCompleted(), getType().isBuilding()
    bool isUpgrading() const;

    /// <summary>Checks if this unit is visible.</summary>
    ///
    /// <param name="player"> (optional)
    ///     The player to check visibility for. If this parameter is omitted, then the BWAPI player
    ///     obtained from Game::self will be used.
    /// </param>
    ///
    /// @returns true if this unit is visible to the specified \p player, and false if it is not.
    ///
    /// @note If the Flag::CompleteMapInformation flag is enabled, existing units hidden by the
    /// fog of war will be accessible, but isVisible will still return false.
    ///
    /// @see exists
    virtual bool isVisible(Player player = nullptr) const = 0;

    /// <summary>Performs some cheap checks to attempt to quickly detect whether the unit is
    /// unable to be targetted as the target unit of an unspecified command.</summary>
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can be a target.
    /// @retval false if an error occurred and the unit can not be a target.
    ///
    /// @see Game::getLastError, UnitInterface::canTargetUnit
    virtual bool isTargetable() const = 0;

    /// @name Unit Commands
    /// @{

    /// <summary>This function issues a command to the unit(s), however it is used for interfacing
    /// only, and is recommended to use one of the more specific command functions when writing an
    /// AI.</summary>
    ///
    /// <param name="command">
    ///     A UnitCommand containing command parameters such as the type, position, target, etc.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canIssueCommand
    virtual bool issueCommand(UnitCommand command) = 0;

    /// <summary>Orders the unit(s) to attack move to the specified position.</summary>
    ///
    /// <param name="target">
    ///     A Position to designate as the target. The unit will perform an Attack Move command.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @note A @Medic will use Heal Move instead of attack.
    ///
    /// @see Game::getLastError, UnitInterface::canAttack
    bool attack(Position target, bool shiftQueueCommand = false);

    /// <summary>Orders the unit(s) to attack the specified target unit.</summary>
    ///
    /// <param name="target">
    ///     A Unit to designate as the target.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see Game::getLastError, UnitInterface::canAttack
    bool attack(Unit target, bool shiftQueueCommand = false);

    /// <summary>Orders the worker unit(s) to construct a structure at a target position.</summary>
    ///
    /// <param name="type">
    ///     The UnitType to build.
    /// </param>
    /// <param name="target">
    ///     A TilePosition to specify the build location, specifically the upper-left corner of the
    ///     location. If the target is not specified, then the function call will be redirected to
    ///     the train command.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @note You must have sufficient resources and meet the necessary requirements in order to
    /// build a structure.
    ///
    /// @see Game::getLastError, UnitInterface::train, UnitInterface::cancelConstruction, UnitInterface::canBuild
    bool build(UnitType type, TilePosition target = TilePositions::None);

    /// <summary>Orders the @Terran structure(s) to construct an add-on.</summary>
    ///
    /// <param name="type">
    ///     The add-on UnitType to construct.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @note You must have sufficient resources and meet the necessary requirements in order to
    /// build a structure.
    ///
    /// @see Game::getLastError, UnitInterface::build, UnitInterface::cancelAddon, UnitInterface::canBuildAddon
    bool buildAddon(UnitType type);

    /// <summary>Orders the unit(s) to add a UnitType to its training queue, or morphs into the
    /// UnitType if it is @Zerg.</summary>
    ///
    /// <param name="type">
    ///     The UnitType to train.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
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

    /// <summary>Orders the unit(s) to morph into a different UnitType.</summary>
    ///
    /// <param name="type">
    ///     The UnitType to morph into.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see Game::getLastError, UnitInterface::build, UnitInterface::morph, UnitInterface::canMorph
    bool morph(UnitType type);

    /// <summary>Orders the unit to research the given tech type.</summary>
    ///
    /// <param name="tech">
    ///     The TechType to research.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see cancelResearch, isResearching, getRemainingResearchTime, getTech, canResearch
    bool research(TechType tech);

    /// <summary>Orders the unit to upgrade the given upgrade type.</summary>
    ///
    /// <param name="upgrade">
    ///     The UpgradeType to upgrade.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see cancelUpgrade, isUpgrading, getRemainingUpgradeTime, getUpgrade, canUpgrade
    bool upgrade(UpgradeType upgrade);

    /// <summary>Orders the unit to set its rally position.</summary>
    ///
    /// <param name="target">
    ///     The target position that this structure will rally completed units to.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see getRallyPosition, getRallyUnit, canSetRallyPoint, canSetRallyPosition, canSetRallyUnit
    bool setRallyPoint(Position target);

    /// <summary>Orders the unit to set its rally position to the specified unit.</summary>
    ///
    /// <param name="target">
    ///     The target unit that this structure will rally completed units to.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see getRallyPosition, getRallyUnit, canSetRallyPoint, canSetRallyPosition, canSetRallyUnit
    bool setRallyPoint(Unit target);

    /// <summary>Orders the unit to move from its current position to the specified position.</summary>
    ///
    /// <param name="target">
    ///     The target position to move to.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isMoving, canMove
    bool move(Position target, bool shiftQueueCommand = false);

    /// <summary>Orders the unit to patrol between its current position and the specified position.</summary>
    /// While patrolling, units will attack and chase enemy units that they encounter, and then
    /// return to its patrol route. @Medics will automatically heal units and then return to their
    /// patrol route.
    ///
    /// <param name="target">
    ///     The position to patrol to.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isPatrolling, canPatrol
    bool patrol(Position target, bool shiftQueueCommand = false);

    /// <summary>Orders the unit to hold its position.</summary>
    ///
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see canHoldPosition, isHoldingPosition
    bool holdPosition(bool shiftQueueCommand = false);

    /// <summary>Orders the unit to stop.</summary>
    ///
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see canStop, isIdle
    bool stop(bool shiftQueueCommand = false);

    /// <summary>Orders the unit to follow the specified unit.</summary> Units that are following
    /// other units will not perform any other actions such as attacking. They will ignore attackers.
    ///
    /// <param name="target">
    ///     The target unit to start following.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isFollowing, canFollow, getOrderTarget
    bool follow(Unit target, bool shiftQueueCommand = false);

    /// <summary>Orders the unit to gather the specified unit (must be mineral or refinery type).</summary>
    ///
    /// <param name="target">
    ///     The target unit to gather from.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isGatheringGas, isGatheringMinerals, canGather
    bool gather(Unit target, bool shiftQueueCommand = false);

    /// <summary>Orders the unit to return its cargo to a nearby resource depot such as a Command
    /// Center.</summary> Only workers that are carrying minerals or gas can be ordered to return
    /// cargo.
    ///
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isCarryingGas, isCarryingMinerals, canReturnCargo
    bool returnCargo(bool shiftQueueCommand = false);

    /// <summary>Orders the unit to repair the specified unit.</summary> Only Terran SCVs can be
    /// ordered to repair, and the target must be a mechanical @Terran unit or building.
    ///
    /// <param name="target">
    ///     The unit to repair.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isRepairing, canRepair
    bool repair(Unit target, bool shiftQueueCommand = false);

    /// <summary>Orders the unit to burrow.</summary> Either the unit must be a @Lurker, or the
    /// unit must be a @Zerg ground unit that is capable of @Burrowing, and @Burrow technology
    /// must be researched.
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see unburrow, isBurrowed, canBurrow
    bool burrow();

    /// <summary>Orders a burrowed unit to unburrow.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see burrow, isBurrowed, canUnburrow
    bool unburrow();

    /// <summary>Orders the unit to cloak.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see decloak, isCloaked, canCloak
    bool cloak();

    /// <summary>Orders a cloaked unit to decloak.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see cloak, isCloaked, canDecloak
    bool decloak();

    /// <summary>Orders the unit to siege.</summary> Only works for @Siege_Tanks.
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see unsiege, isSieged, canSiege
    bool siege();

    /// <summary>Orders the unit to unsiege.</summary> Only works for sieged @Siege_Tanks.
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see siege, isSieged, canUnsiege
    bool unsiege();

    /// <summary>Orders the unit to lift.</summary> Only works for liftable @Terran structures.
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see land, isLifted, canLift
    bool lift();

    /// <summary>Orders the unit to land.</summary> Only works for @Terran structures that are
    /// currently lifted.
    ///
    /// <param name="target">
    ///     The tile position to land this structure at.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see lift, isLifted, canLand
    bool land(TilePosition target);

    /// <summary>Orders the unit to load the target unit.</summary> Only works if this unit is a
    /// @Transport or @Bunker type.
    ///
    /// <param name="target">
    ///     The target unit to load into this @Transport or @Bunker.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see unload, unloadAll, getLoadedUnits, isLoaded
    bool load(Unit target, bool shiftQueueCommand = false);

    /// <summary>Orders the unit to unload the target unit.</summary> Only works for @Transports
    /// and @Bunkers.
    ///
    /// <param name="target">
    ///     Unloads the target unit from this @Transport or @Bunker.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see load, unloadAll, getLoadedUnits, isLoaded, canUnload, canUnloadAtPosition
    bool unload(Unit target);

    /// <summary>Orders the unit to unload all loaded units at the unit's current position.</summary>
    /// Only works for @Transports and @Bunkers.
    ///
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see load, unload, getLoadedUnits, isLoaded, canUnloadAll, canUnloadAtPosition
    bool unloadAll(bool shiftQueueCommand = false);

    /// <summary>Orders the unit to unload all loaded units at the specified location.</summary>
    /// Only works for @Transports. Not applicable to @Bunkers.
    ///
    /// <param name="target">
    ///     The target position to unload the units at.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see load, unload, getLoadedUnits, isLoaded, canUnloadAllPosition, canUnloadAtPosition
    bool unloadAll(Position target, bool shiftQueueCommand = false);

    /// <summary>Performs a right click action as it would work in StarCraft.</summary>
    ///
    /// <param name="target">
    ///     The target position to right click.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see canRightClick, canRightClickPosition, canRightClickUnit
    bool rightClick(Position target, bool shiftQueueCommand = false);

    /// <summary>Performs a right click action as it would work in StarCraft.</summary>
    ///
    /// <param name="target">
    ///     The target unit to right click.
    /// </param>
    /// <param name="shiftQueueCommand"> (optional)
    ///     If this value is true, then the order will be queued instead of immediately executed.
    ///     If this value is omitted, then the order will be executed immediately by default.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see canRightClick, canRightClickPosition, canRightClickUnit
    bool rightClick(Unit target, bool shiftQueueCommand = false);

    /// <summary>Orders a @SCV to stop constructing a structure.</summary> This leaves the
    /// structure in an incomplete state until it is either cancelled, razed, or completed by
    /// another @SCV.
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isConstructing, canHaltConstruction
    bool haltConstruction();

    /// <summary>Orders this unit to cancel and refund itself from begin constructed.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see isBeingConstructed, build, canCancelConstruction
    bool cancelConstruction();

    /// <summary>Orders this unit to cancel and refund an add-on that is being constructed.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see canCancelAddon, buildAddon
    bool cancelAddon();

    /// <summary>Orders the unit to remove the specified unit from its training queue.</summary>
    ///
    /// <param name="slot"> (optional)
    ///     Identifies the slot that will be cancelled. If the specified value is at least 0, then
    ///     the unit in the corresponding slot from the list provided by getTrainingQueue will be
    ///     cancelled. If the value is either omitted or -2, then the last slot is cancelled.
    /// </param>
    ///
    /// @note The value of slot is passed directly to Broodwar. Other negative values have no
    /// effect.
    ///
    /// @see train, cancelTrain, isTraining, getTrainingQueue, canCancelTrain, canCancelTrainSlot
    bool cancelTrain(int slot = -2);

    /// <summary>Orders this unit to cancel and refund a unit that is morphing.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see morph, isMorphing, canCancelMorph
    bool cancelMorph();

    /// <summary>Orders this unit to cancel and refund a research that is in progress.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @see research, isResearching, getTech, canCancelResearch
    bool cancelResearch();

    /// <summary>Orders this unit to cancel and refund an upgrade that is in progress.</summary>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    /// @see upgrade, isUpgrading, getUpgrade, canCancelUpgrade
    bool cancelUpgrade();

    /// <summary>Orders the unit to use a technology.</summary>
    ///
    /// <param name="tech">
    ///     The technology type to use.
    /// </param>
    /// <param name="target">
    ///     If specified, indicates the target location to use the tech on.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that
    /// the command would fail.
    ///
    /// @see canUseTechWithOrWithoutTarget, canUseTech, canUseTechWithoutTarget, canUseTechUnit,
    ///      canUseTechPosition, TechTypes
    bool useTech(TechType tech, Position target);

    /// <summary>Orders the unit to use a technology.</summary>
    ///
    /// <param name="tech">
    ///     The technology type to use.
    /// </param>
    /// <param name="target"> (optional)
    ///     If specified, indicates the target unit to use the tech on. If unspecified,
    ///     causes the \p tech to be used without a target (i.e. @Stim_Packs).
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that
    /// the command would fail.
    ///
    /// @see canUseTechWithOrWithoutTarget, canUseTech, canUseTechWithoutTarget, canUseTechUnit,
    ///      canUseTechPosition, TechTypes
    bool useTech(TechType tech, Unit target = nullptr);

    /// <summary>Moves a @Flag_Beacon to a different location.</summary> This is only used for @CTF
    /// or @UMS game types.
    ///
    /// <param name="target">
    ///     The target tile position to place the @Flag_Beacon.
    /// </param>
    ///
    /// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
    /// the command would fail.
    /// @note There is a small chance for a command to fail after it has been passed to Broodwar.
    ///
    /// @note This command is only available for the first 10 minutes of the game, as in Broodwar.
    ///
    /// @see canPlaceCOP
    bool placeCOP(TilePosition target);

    /// @}
    /// @name Command Verifiers
    /// @{

    /// <summary>Checks whether the unit is able to execute the given command.</summary> If you
    /// are calling this function repeatedly (e.g. to generate a collection of valid commands),
    /// you can avoid repeating the same kinds of checks by specifying false for some of the
    /// optional boolean arguments. Make sure that the state hasn't changed since the check was
    /// done though (eg a new frame/event, or a command issued). Also see the more specific functions.
    ///
    /// <param name="command">
    ///   A UnitCommand to check.
    /// </param>
    /// <param name="checkCanUseTechPositionOnPositions">
    ///   Only used if the command type is UnitCommandTypes::Enum::Use_Tech_Position. A boolean
    ///   for whether to perform cheap checks for whether the unit is unable to target any
    ///   positions using the command's TechType (i.e. regardless of what the other command
    ///   parameters are). You can set this to false if you know this check has already just been
    ///   performed.
    /// </param>
    /// <param name="checkCanUseTechUnitOnUnits">
    ///   Only used if the command type is UnitCommandTypes::Enum::Use_Tech_Unit. A boolean for
    ///   whether to perform cheap checks for whether the unit is unable to target any units using
    ///   the command's TechType (i.e. regardless of what the other command parameters are). You
    ///   can set this to false if you know this check has already just been performed.
    /// </param>
    /// <param name="checkCanBuildUnitType">
    ///   Only used if the command type is UnitCommandTypes::Build. A boolean for whether to
    ///   perform cheap checks for whether the unit is unable to build the specified UnitType
    ///   (i.e. regardless of what the other command parameters are). You can set this to false if
    ///   you know this check has already just been performed.
    /// </param>
    /// <param name="checkCanTargetUnit">
    ///   Only used for command types that can target a unit. A boolean for whether to perform
    ///   UnitInterface::canTargetUnit as a check. You can set this to false if you know this check has
    ///   already just been performed.
    /// </param>
    /// <param name="checkCanIssueCommandType">
    ///   A boolean for whether to perform UnitInterface::canIssueCommandType as a check. You can set this
    ///   to false if you know this check has already just been performed.
    /// </param>
    /// <param name="checkCommandibility">
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    /// </param>
    ///
    /// @retval true if BWAPI determined that the command is valid.
    /// @retval false if an error occurred and the command is invalid.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canCommand, UnitInterface::canIssueCommandType,
    /// UnitInterface::canTargetUnit
    virtual bool canIssueCommand(UnitCommand command, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute the given command as part of a Unitset
    /// (even if none of the units in the Unitset are able to execute the command individually).</summary>
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

    /// <summary>Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute any commands (eg the unit is stasised).</summary>
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can be commanded.
    /// @retval false if an error occurred and the unit can not be commanded.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand
    virtual bool canCommand() const = 0;

    /// <summary>Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute any commands as part of a Unitset (eg buildings, critters).</summary>
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can be commanded grouped.
    /// @retval false if an error occurred and the unit can not be commanded grouped.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canIssueCommand
    virtual bool canCommandGrouped(bool checkCommandibility = true) const = 0;

    /// <summary>Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute the given command type (i.e. regardless of what other possible command parameters
    /// could be).</summary>
    ///
    /// <param name="ct">
    ///   A UnitCommandType.
    /// </param>
    /// <param name="checkCommandibility">
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    /// </param>
    ///
    /// @retval true if BWAPI was unable to determine whether the command type is invalid.
    /// @retval false if an error occurred and the command type is invalid.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canIssueCommand
    virtual bool canIssueCommandType(UnitCommandType ct, bool checkCommandibility = true) const = 0;

    /// <summary>Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// execute the given command type (i.e. regardless of what other possible command parameters
    /// could be) as part of a Unitset.</summary>
    ///
    /// <param name="ct">
    ///   A UnitCommandType.
    /// </param>
    /// <param name="checkCommandibilityGrouped">
    ///   A boolean for whether to perform UnitInterface::canCommandGrouped as a check. You can set this
    ///   to false if you know this check has already just been performed.
    /// </param>
    /// <param name="checkCommandibility">
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    /// </param>
    ///
    /// @retval true if BWAPI was unable to determine whether the command type is invalid.
    /// @retval false if an error occurred and the command type is invalid.
    ///
    /// @see UnitCommandTypes, Game::getLastError, UnitInterface::canIssueCommandGrouped
    virtual bool canIssueCommandTypeGrouped(UnitCommandType ct, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Performs some cheap checks to attempt to quickly detect whether the unit is unable to
    /// use the given unit as the target unit of an unspecified command.</summary>
    ///
    /// <param name="targetUnit">
    ///   A target unit for an unspecified command.
    /// </param>
    /// <param name="checkCommandibility">
    ///   A boolean for whether to perform UnitInterface::canCommand as a check. You can set this to false
    ///   if you know this check has already just been performed.
    /// </param>
    ///
    /// @retval true if BWAPI was unable to determine whether the unit can target the given unit.
    /// @retval false if an error occurred and the unit can not target the given unit.
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::isTargetable
    virtual bool canTargetUnit(Unit targetUnit, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an attack command to attack-move or attack a unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack,
    /// UnitInterface::canAttackMove, UnitInterface::canAttackUnit
    virtual bool canAttack(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an attack command to attack-move or attack a (non-null)
    /// unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack,
    /// UnitInterface::canAttackMove, UnitInterface::canAttackUnit
    virtual bool canAttack(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    /// @overload
    virtual bool canAttack(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an attack command to attack-move or attack a unit,
    /// as part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttack
    virtual bool canAttackGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an attack command to attack-move or attack a
    /// (non-null) unit, as part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttack
    virtual bool canAttackGrouped(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;
    /// @overload
    virtual bool canAttackGrouped(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an attack command to attack-move.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack
    virtual bool canAttackMove(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an attack command to attack-move, as part of a
    /// Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttackMove
    virtual bool canAttackMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an attack command to attack a unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack
    virtual bool canAttackUnit(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an attack command to attack a unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::attack
    virtual bool canAttackUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an attack command to attack a unit,
    /// as part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttackUnit
    virtual bool canAttackUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an attack command to attack a unit, as part of
    /// a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canAttackUnit
    virtual bool canAttackUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a build command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::build
    virtual bool canBuild(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a build command for the given
    /// UnitType.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::build
    virtual bool canBuild(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a build command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::build
    virtual bool canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a buildAddon command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::buildAddon
    virtual bool canBuildAddon(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a buildAddon command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::buildAddon
    virtual bool canBuildAddon(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a train command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::train
    virtual bool canTrain(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a train command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::train
    virtual bool canTrain(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a morph command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::morph
    virtual bool canMorph(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a morph command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::morph
    virtual bool canMorph(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a research command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::research
    virtual bool canResearch(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a research command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::research
    virtual bool canResearch(TechType type, bool checkCanIssueCommandType = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an upgrade command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::upgrade
    virtual bool canUpgrade(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an upgrade command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::upgrade
    virtual bool canUpgrade(UpgradeType type, bool checkCanIssueCommandType = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a setRallyPoint command to a
    /// position or unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint,
    /// UnitInterface::canSetRallyPosition, UnitInterface::canSetRallyUnit.
    virtual bool canSetRallyPoint(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a setRallyPoint command to a position
    /// or (non-null) unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint,
    /// UnitInterface::canSetRallyPosition, UnitInterface::canSetRallyUnit.
    virtual bool canSetRallyPoint(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    /// @overload
    virtual bool canSetRallyPoint(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a setRallyPoint command to a position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint
    virtual bool canSetRallyPosition(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a setRallyPoint command to a unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint
    virtual bool canSetRallyUnit(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a setRallyPoint command to a unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::setRallyPoint
    virtual bool canSetRallyUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a move command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::move
    virtual bool canMove(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a move command, as part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canMove
    virtual bool canMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a patrol command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::patrol
    virtual bool canPatrol(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a patrol command, as part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canPatrol
    virtual bool canPatrolGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a follow command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::follow
    virtual bool canFollow(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a follow command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::follow
    virtual bool canFollow(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a gather command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::gather
    virtual bool canGather(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a gather command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::gather
    virtual bool canGather(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a returnCargo command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::returnCargo
    virtual bool canReturnCargo(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a holdPosition command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::holdPosition
    virtual bool canHoldPosition(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a stop command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::stop
    virtual bool canStop(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a repair command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::repair
    virtual bool canRepair(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a repair command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::repair
    virtual bool canRepair(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a burrow command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::burrow
    virtual bool canBurrow(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an unburrow command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unburrow
    virtual bool canUnburrow(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cloak command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cloak
    virtual bool canCloak(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a decloak command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::decloak
    virtual bool canDecloak(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a siege command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::siege
    virtual bool canSiege(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an unsiege command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unsiege
    virtual bool canUnsiege(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a lift command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::lift
    virtual bool canLift(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a land command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::land
    virtual bool canLand(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a land command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::land
    virtual bool canLand(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a load command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::load
    virtual bool canLoad(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a load command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::load
    virtual bool canLoad(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an unload command or unloadAll at
    /// current position command or unloadAll at a different position command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload, UnitInterface::unloadAll
    virtual bool canUnloadWithOrWithoutTarget(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an unload command or unloadAll at
    /// current position command or unloadAll at a different position command, for a given
    /// position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload, UnitInterface::unloadAll
    virtual bool canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an unload command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload
    virtual bool canUnload(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an unload command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unload, UnitInterface::canUnloadAtPosition
    virtual bool canUnload(Unit targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an unloadAll command for the current position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unloadAll
    virtual bool canUnloadAll(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute an unloadAll command for a different
    /// position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unloadAll
    virtual bool canUnloadAllPosition(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute an unloadAll command for a different position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::unloadAll
    virtual bool canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a rightClick command to a position
    /// or unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick,
    /// UnitInterface::canRightClickPosition, UnitInterface::canRightClickUnit.
    virtual bool canRightClick(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a rightClick command to a position or (non-null)
    /// unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick,
    /// UnitInterface::canRightClickPosition, UnitInterface::canRightClickUnit.
    virtual bool canRightClick(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    // @overload
    virtual bool canRightClick(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a rightClick command to a position
    /// or unit, as part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a rightClick command to a position or (non-null)
    /// unit, as part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickGrouped(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;
    /// @overload
    virtual bool canRightClickGrouped(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a rightClick command for a position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick
    virtual bool canRightClickPosition(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a rightClick command for a position, as part of
    /// a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClick
    virtual bool canRightClickPositionGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a rightClick command to a unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick
    virtual bool canRightClickUnit(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a rightClick command to a unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::rightClick
    virtual bool canRightClickUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a rightClick command to a unit, as
    /// part of a Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a rightClick command to a unit, as part of a
    /// Unitset.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommandGrouped, UnitInterface::canRightClickUnit
    virtual bool canRightClickUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a haltConstruction command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::haltConstruction
    virtual bool canHaltConstruction(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cancelConstruction command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelConstruction
    virtual bool canCancelConstruction(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cancelAddon command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelAddon
    virtual bool canCancelAddon(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cancelTrain command for any slot.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelTrain
    virtual bool canCancelTrain(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a cancelTrain command for an
    /// unspecified slot.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelTrain
    virtual bool canCancelTrainSlot(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cancelTrain command for a specified slot.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelTrain
    virtual bool canCancelTrainSlot(int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cancelMorph command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelMorph
    virtual bool canCancelMorph(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cancelResearch command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelResearch
    virtual bool canCancelResearch(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a cancelUpgrade command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::cancelUpgrade
    virtual bool canCancelUpgrade(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a useTech command without a target or
    /// or a useTech command with a target position or a useTech command with a target unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechWithOrWithoutTarget(bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a useTech command without a target or
    /// or a useTech command with a target position or a useTech command with a target unit, for a
    /// given TechType.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechWithOrWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a useTech command for a specified position or
    /// unit (only specify nullptr if the TechType does not target another position/unit).</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech,
    /// UnitInterface::canUseTechWithoutTarget, UnitInterface::canUseTechUnit, UnitInterface::canUseTechPosition
    virtual bool canUseTech(BWAPI::TechType tech, Position target, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    /// @overload
    virtual bool canUseTech(BWAPI::TechType tech, Unit target = nullptr, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a useTech command without a target.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a useTech command with an unspecified
    /// target unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechUnit(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a useTech command with a target unit.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechUnit(BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnits = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a useTech command with an unspecified target
    /// position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechPosition(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a useTech command with a target position.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::useTech
    virtual bool canUseTechPosition(BWAPI::TechType tech, Position target, bool checkTargetsPositions = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    /// <summary>Cheap checks for whether the unit is able to execute a placeCOP command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::placeCOP
    virtual bool canPlaceCOP(bool checkCommandibility = true) const = 0;

    /// <summary>Checks whether the unit is able to execute a placeCOP command.</summary>
    ///
    /// @see Game::getLastError, UnitInterface::canIssueCommand, UnitInterface::placeCOP
    virtual bool canPlaceCOP(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    ///@}
  };
}

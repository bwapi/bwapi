#pragma once

#include <list>

#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/UnitCommand.h>

namespace BWAPI
{
  class Player;

  /** The Unit class is used to get information about individual units as well as issue orders to units. Each
   * unit in the game has a unique Unit object, and Unit objects are not deleted until the end of the match
   * (so you don't need to worry about unit pointers becoming invalid).
   *
   * Every Unit in the game is either accessible or inaccessible. To determine if an AI can access a
   * particular unit, BWAPI checks to see if Flag::CompleteMapInformation? is enabled. So there are two cases
   * to consider - either the flag is enabled, or it is disabled:
   *
   * If Flag::CompleteMapInformation? is disabled, then a unit is accessible if and only if it is visible.
   * Note also that some properties of visible enemy units will not be made available to the AI (such as the
   * contents of visible enemy dropships). If a unit is not visible, Unit::exists will return false,
   * regardless of whether or not the unit exists. This is because absolutely no state information on
   * invisible enemy units is made available to the AI. To determine if an enemy unit has been destroyed, the
   * AI must watch for AIModule::onUnitDestroy messages from BWAPI, which is only called for visible units
   * which get destroyed.
   *
   * If Flag::CompleteMapInformation? is enabled, then all units that exist in the game are accessible, and
   * Unit::exists is accurate for all units. Similarly AIModule::onUnitDestroy messages are generated for all
   * units that get destroyed, not just visible ones.
   *
   * If a Unit is not accessible, in general the only the getInitial__ functions will be available to the AI.
   * However for units that were owned by the player, getPlayer and getType will continue to work for units
   * that have been destroyed. */
  class Unit
  {
    public:
      /** Returns a unique ID for this unit. It simply casts the unit's address as an integer, since each unit
       * has a unique address. */
      virtual int getID() const = 0;

      /** Returns a pointer to the player that owns this unit. */
      virtual Player* getPlayer() const = 0;

      /** Returns the current type of the unit. */
      virtual UnitType getType() const = 0;

      /** Returns the initial type of the unit or Unknown if it wasn't a neutral unit at the beginning of the
       * game. */
      virtual UnitType getInitialType() const = 0;

      /** Returns the unit's current amount of hit points. */
      virtual int getHitPoints() const = 0;

      /** Returns the unit's initial amount of hit points, or 0 if it wasn't a neutral unit at the beginning
       * of the game. */
      virtual int getInitialHitPoints() const = 0;

      /** Returns the unit's current amount of shields. */
      virtual int getShields() const = 0;

      /** Returns the unit's current amount of energy. */
      virtual int getEnergy() const = 0;

      /** Returns the unit's current amount of containing resources. Useful for determining how much minerals
       * are left in a mineral patch, or how much gas is left in a geyser
       * (can also be called on a refinery/assimilator/extractor). */
      virtual int getResources() const = 0;

      /** Returns the unit's initial amount of containing resources, or 0 if the unit wasn't a neutral unit
       * at the beginning of the game. */
      virtual int getInitialResources() const = 0;

      /** Returns the unit's current kill count. */
      virtual int getKillCount() const = 0;

      /** Returns unit's ground weapon cooldown. It is 0 if the unit is ready to attack. */
      virtual int getGroundWeaponCooldown() const = 0;

      /** Returns unit's air weapon cooldown. It is 0 if the unit is ready to attack. */
      virtual int getAirWeaponCooldown() const = 0;

      /** Returns unit's ground weapon cooldown. It is 0 if the unit is ready cast a spell. */
      virtual int getSpellCooldown() const = 0;

      /** Returns the remaining hit points of the defense matrix. Initially a defense Matrix has 250 points.
       * \see Unit::getDefenseMatrixTimer, Unit::isDefenseMatrixed. */
      virtual int getDefenseMatrixPoints() const = 0;

      /** Returns the time until the defense matrix wears off. 0 -> No defense Matrix present. */
      virtual int getDefenseMatrixTimer() const = 0;

      /** Returns the time until the ensnare effect wears off. 0 -> No ensnare effect present. */
      virtual int getEnsnareTimer() const = 0;

      /** Returns the time until the radiation wears off. 0 -> No radiation present. */
      virtual int getIrradiateTimer() const = 0;

      /** Returns the time until the lockdown wears off. 0 -> No lockdown present. */
      virtual int getLockdownTimer() const = 0;

      /** Returns the time until the maelstrom wears off. 0 -> No maelstrom present. */
      virtual int getMaelstromTimer() const = 0;

      /** Returns the time until the plague wears off. 0 -> No plague present. */
      virtual int getPlagueTimer() const = 0;

      /** Returns the amount of time until the unit is removed, or 0 if the unit does not have a remove timer.
       * Used to determine how much time remains before hallucinated units, dark swarm, etc have until they
       * are removed. */
      virtual int getRemoveTimer() const = 0;

      /** Returns the time until the stasis field wears off. 0 -> No stasis field present. */
      virtual int getStasisTimer() const = 0;

      /** Returns the time until the stimpack wears off. 0 -> No stimpack boost present. */
      virtual int getStimTimer() const = 0;

      /** Returns the position of the unit on the map. */
      virtual Position getPosition() const = 0;

      /** Returns the initial position of the unit on the map, or Positions::Unknown if the unit wasn't a
       * neutral unit at the beginning of the game. */
      virtual Position getInitialPosition() const = 0;

      /** Returns the build tile position of the unit on the map. Useful if the unit is a building. The tile
       * position is of the top left corner of the building. */
      virtual TilePosition getTilePosition() const = 0;

      /** Returns the initial build tile position of the unit on the map, or TilePositions::Unknown if the
       * unit wasn't a neutral unit at the beginning of the game. The tile position is of the top left corner
       * of the building. */
      virtual TilePosition getInitialTilePosition() const = 0;

      /** Returns the edge-to-edge distance between the current unit and the target unit. */
      virtual double getDistance(Unit* target) const = 0;

      /** Returns the distance from the edge of the current unit to the target position. */
      virtual double getDistance(Position target) const = 0;

      /** Returns the direction the unit is facing, measured in radians. An angle of 0 means the unit is
       * facing east. */
      virtual double getAngle() const = 0;

      /** Returns the x component of the unit's velocity, measured in pixels per frame. */
      virtual double getVelocityX() const = 0;

      /** Returns the y component of the unit's velocity, measured in pixels per frame. */
      virtual double getVelocityY() const = 0;

      /** Generally returns the appropriate target unit after issuing an order that accepts a target unit
       * (i.e. attack, repair, gather, follow, etc.). To check for a target that has been acquired
       * automatically (without issuing an order) see getOrderTarget. */
      virtual Unit* getTarget() const = 0;

      /** Returns the target position the unit is moving to (provided a valid path to the target position
       * exists). */
      virtual Position getTargetPosition() const = 0;

      // TODO: add doc
      virtual Order getOrder() const = 0;

      /** This is usually set when the low level unit AI acquires a new target automatically. For example if
       * an enemy probe comes in range of your marine, the marine will start attacking it, and getOrderTarget
       * will be set in this case, but not getTarget. */
      virtual Unit* getOrderTarget() const = 0;
      virtual int getOrderTimer() const = 0;
      virtual Order getSecondaryOrder() const = 0;

      /** If the unit is an SCV that is constructing a building, this will return the building it is
       * constructing. If the unit is a Terran building that is being constructed, this will return the SCV
       * that is constructing it. */
      virtual Unit* getBuildUnit() const = 0;

      /** Returns the building type a worker is about to construct. If the unit is a morphing Zerg unit or an
       * incomplete building, this returns the UnitType the unit is about to become upon completion.*/
      virtual UnitType getBuildType() const = 0;

      /** Returns the remaining build time of a unit/building that is being constructed. */
      virtual int getRemainingBuildTime() const = 0;

      /** Returns the remaining time of the unit that is currently being trained. If the unit is a Hatchery,
       * Lair, or Hive, this returns the amount of time until the next larva spawns, or 0 if the unit already
       * has 3 larva. */
      virtual int getRemainingTrainTime() const = 0;

      // TODO: add doc
      virtual Unit* getChild() const = 0;

      /** Returns the list of units queued up to be trained.
       * \see Unit::train, Unit::cancelTrain, Unit::isTraining. */
      virtual std::list<UnitType > getTrainingQueue() const = 0;

      /** Returns the dropship, shuttle, overlord, or bunker that is this unit is loaded in to. */
      virtual Unit* getTransport() const = 0;

      /** Returns a list of the units loaded into a Terran Bunker, Terran Dropship, Protoss Shuttle, or Zerg
       * Overlord. */
      virtual std::list<Unit*> getLoadedUnits() const = 0;

      /** Returns the number of interceptors the Protoss Carrier has. */
      virtual int getInterceptorCount() const = 0;

      /** Returns the number of scarabs in the Protoss Reaver. */
      virtual int getScarabCount() const = 0;

      /** Returns the number of spider mines in the Terran Vulture. */
      virtual int getSpiderMineCount() const = 0;

      /** Returns the tech that the unit is currently researching. If the unit is not researching anything,
       * TechTypes::None is returned.
       * \see Unit::research, Unit::cancelResearch, Unit::isResearching, Unit::getRemainingResearchTime. */
      virtual TechType getTech() const = 0;

      /** Returns the upgrade that the unit is currently upgrading. If the unit is not upgrading anything,
       * UpgradeTypes::None is returned.
       * \see Unit::upgrade, Unit::cancelUpgrade, Unit::isUpgrading, Unit::getRemainingUpgradeTime. */
      virtual UpgradeType getUpgrade() const = 0;

      /** Returns the amount of time until the unit is done researching its current tech. If the unit is not
       * researching anything, 0 is returned.
       * \see Unit::research, Unit::cancelResearch, Unit::isResearching, Unit::getTech. */
      virtual int getRemainingResearchTime() const = 0;

      /** Returns the amount of time until the unit is done upgrading its current upgrade. If the unit is not
       * upgrading anything, 0 is returned.
       * \see Unit::upgrade, Unit::cancelUpgrade, Unit::isUpgrading, Unit::getUpgrade. */
      virtual int getRemainingUpgradeTime() const = 0;

      /** Returns the position the building is rallied to. If the building does not produce units,
       * Positions::None is returned.
       * \see Unit::setRallyPosition, Unit::setRallyUnit, Unit::getRallyUnit. */
      virtual Position getRallyPosition() const = 0;

      /** Returns the unit the building is rallied to. If the building is not rallied to any unit, NULL is
       * returned.
       * \see Unit::setRallyPosition, Unit::setRallyUnit, Unit::getRallyPosition. */
      virtual Unit* getRallyUnit() const = 0;

      /** Returns the add-on of this unit, or NULL if the unit doesn't have an add-on. */
      virtual Unit* getAddon() const = 0;

      /** For Zerg Larva, this returns the Hatchery, Lair, or Hive unit this Larva was spawned from. For all
       * other unit types this function returns NULL. */
      virtual Unit* getHatchery() const = 0;

      /** Returns the set of larva spawned by this unit. If the unit has no larva, or is not a Hatchery, Lair,
       * or Hive, this function returns an empty set. Equivalent to clicking "Select Larva" from the Starcraft
       * GUI. */
      virtual std::set<Unit*> getLarva() const = 0;

      /** Returns true if the owner of this player has upgraded the given upgrade type, and this unit is
       * affected by this upgrade. */
      virtual int getUpgradeLevel(UpgradeType upgrade) const = 0;

      /**
       * 3 cases to consider:
       *
       * - If exists() returns true, the unit exists.
       * - If exists() returns false and the unit is owned by self(), then the unit does not exist.
       * - If exists() returns false and the unit is not owned by self(), then the unit may or may not exist.
       *
       * \see Unit::isVisible.
       * */
      virtual bool exists() const = 0;

      /** Returns true if the unit is currently accelerating. */
      virtual bool isAccelerating() const = 0;

      // TODO: add doc
      virtual bool isAttacking() const = 0;

      /** Returns true if the unit is being constructed. Always true for incomplete Protoss and Zerg
       * buildings, and true for incomplete Terran buildings that have an SCV constructing them. If the SCV
       * halts construction, isBeingConstructed will return false.
       *
       * \see Unit::build, Unit::cancelConstruction, Unit::haltConstruction, Unit::isConstructing. */
      virtual bool isBeingConstructed() const = 0;

      /** Returns true if the unit is a mineral patch or refinery that is being gathered. */
      virtual bool isBeingGathered() const = 0;

      /** Returns true if the unit is currently being healed by a Terran Medic. */
      virtual bool isBeingHealed() const = 0;

      /** Returns true if the unit is currently blind from a Medic's Optical Flare. */
      virtual bool isBlind() const = 0;

      /** Returns true if the unit is currently braking/slowing down. */
      virtual bool isBraking() const = 0;

      /** Returns true if the unit is a Zerg unit that is current burrowed.
       * \see Unit::burrow, Unit::unburrow. */
      virtual bool isBurrowed() const = 0;

      /** Returns true if the unit is a worker that is carrying gas.
       * \see Unit::returnCargo, Unit::isGatheringGas. */
      virtual bool isCarryingGas() const = 0;

      /** Returns true if the unit is a worker that is carrying minerals.
       * \see Unit::returnCargo, Unit::isGatheringMinerals. */
      virtual bool isCarryingMinerals() const = 0;

      /** Returns true if the unit is cloaked.
       * \see Unit::cloak, Unit::decloak. */
      virtual bool isCloaked() const = 0;

      /** Returns true if the unit has been completed. */
      virtual bool isCompleted() const = 0;

      /** Returns true when a unit has been issued an order to build a structure and is moving to the build
       * location. Also returns true for Terran SCVs while they construct a building.
       * \see Unit::build, Unit::cancelConstruction, Unit::haltConstruction, Unit::isBeingConstructed. */
      virtual bool isConstructing() const = 0;

      /** Returns true if the unit has a defense matrix from a Terran Science Vessel. */
      virtual bool isDefenseMatrixed() const = 0;

      /** Returns true if the unit has been ensnared by a Zerg Queen. */
      virtual bool isEnsnared() const = 0;

      /** Returns true if the unit is following another unit.
       * \see Unit::follow, Unit::getTarget. */
      virtual bool isFollowing() const = 0;

      /** Returns true if the unit is in one of the four states for gathering gas (MoveToGas, WaitForGas,
       * HarvestGas, ReturnGas).
       * \see Unit::isCarryingGas. */
      virtual bool isGatheringGas() const = 0;

      /** Returns true if the unit is in one of the four states for gathering minerals (MoveToMinerals,
       * WaitForMinerals, MiningMinerals, ReturnMinerals).
       * \see Unit::isCarryingMinerals. */
      virtual bool isGatheringMinerals() const = 0;

      /** Returns true for hallucinated units, false for normal units. Returns true for hallucinated enemy
       * units only if Complete Map Information is enabled.
       * \see Unit::getRemoveTimer. */
      virtual bool isHallucination() const = 0;

      /** Returns true if the unit is not doing anything.
       * \see Unit::stop. */
      virtual bool isIdle() const = 0;

      /** Returns true if the unit is being irradiated by a Terran Science Vessel.
       * \see Unit::getIrradiateTimer. */
      virtual bool isIrradiated() const = 0;

      /** Returns true if the unit is a Terran building that is currently lifted off the ground.
       * \see Unit::lift,Unit::land. */
      virtual bool isLifted() const = 0;

      /** Return true if the unit is loaded into a Terran Bunker, Terran Dropship, Protoss Shuttle, or Zerg
       * Overlord.
       * \see Unit::load, Unit::unload, Unit::unloadAll. */
      virtual bool isLoaded() const = 0;

      /** Returns true if the unit is locked down by a Terran Ghost.
       *  \see Unit::getLockdownTimer. */
      virtual bool isLockedDown() const = 0;

      /** Returns true if the unit is being maelstrommed.
       * \see Unit::getMaelstromTimer. */
      virtual bool isMaelstrommed() const = 0;

      /** Returns true if the unit is a zerg unit that is morphing.
       * \see Unit::morph, Unit::cancelMorph, Unit::getBuildType, Unit::getRemainingBuildTime. */
      virtual bool isMorphing() const = 0;

      /** Returns true if the unit is moving.
       * \see Unit::attackMove, Unit::stop. */
      virtual bool isMoving() const = 0;

      /** Returns true if the unit has been parasited by some other player. */
      virtual bool isParasited() const = 0;

      /** Returns true if the unit is patrolling between two positions.
       * \see Unit::patrol. */
      virtual bool isPatrolling() const = 0;

      /** Returns true if the unit has been plagued by a Zerg Defiler.
       * \see Unit::getPlagueTimer. */
      virtual bool isPlagued() const = 0;

      /** Returns true if the unit is a Terran SCV that is repairing or moving to repair another unit. */
      virtual bool isRepairing() const = 0;

      /** Returns true if the unit is a building that is researching tech. See TechTypes for the complete list
       * of available techs in Broodwar.
       * \see Unit::research, Unit::cancelResearch, Unit::getTech, Unit::getRemainingResearchTime. */
      virtual bool isResearching() const = 0;

      /** Returns true if the unit has been selected by the user via the starcraft GUI. Only available if you
       * enable Flag::UserInput during AIModule::onStart.
       * \see Game::getSelectedUnits. */
      virtual bool isSelected() const = 0;

      /** Returns true if the unit is a Terran Siege Tank that is currently in Siege mode.
       * \see Unit::siege, Unit::unsiege. */
      virtual bool isSieged() const = 0;

      /** Returns true if the unit is starting to attack.
       * \see Unit::attackUnit, Unit::getGroundWeaponCooldown, Unit::getAirWeaponCooldown. */
      virtual bool isStartingAttack() const = 0;

      /** Returns true if the unit has been stasised by a Protoss Arbiter.
       * \see Unit::getStasisTimer. */
      virtual bool isStasised() const = 0;

      /** Returns true if the unit is currently stimmed.
       * \see Unit::getStimTimer. */
      virtual bool isStimmed() const = 0;

      /** Returns true if the unit is training units (i.e. a Barracks training Marines).
       * \see Unit::train, Unit::getTrainingQueue, Unit::cancelTrain, Unit::getRemainingTrainTime. */
      virtual bool isTraining() const = 0;

      /** Returns true if the unit is under a Protoss Psionic Storm. */
      virtual bool isUnderStorm() const = 0;

      /** Returns true if the unit is a Protoss building that is unpowered because no pylons are in range. */
      virtual bool isUnpowered() const = 0;

      /** Returns true if the unit is a building that is upgrading. See UpgradeTypes for the complete list
       * of available upgrades in Broodwar.
       * \see Unit::upgrade, Unit::cancelUpgrade, Unit::getUpgrade, Unit::getRemainingUpgradeTime. */
      virtual bool isUpgrading() const = 0;

      /** Returns true if the unit is visible. If the CompleteMapInformation?  cheat flag is enabled, existing
       * units hidden by the fog of war will be accessible, but isVisible will still return false.
       * \see Unit::exists. */
      virtual bool isVisible() const = 0;
      virtual bool isVisible(Player* player) const = 0;

      /** Takes any unit command and calls the corresponding order that will execute it */
      virtual bool issueCommand(UnitCommand command) = 0;

      /** Orders the unit to attack move to the specified location. */
      virtual bool attackMove(Position position) = 0;

      /** Orders the unit to attack the specified unit. */
      virtual bool attackUnit(Unit* target) = 0;

      /** Works like the right click in the GUI. */
      virtual bool rightClick(Position position) = 0;

      /** Works like the right click in the GUI. Right click on a mineral patch to order a worker to mine,
       * right click on an enemy to attack it. */
      virtual bool rightClick(Unit* target) = 0;

      /** Orders this unit to add the specified unit type to the training queue. Note that the player must
       * have sufficient resources to train. If you wish to make units from a hatchery, use getLarva to get
       * the larva associated with the hatchery and then call morph on the larva you want to morph. This
       * command can also be used to make interceptors and scarabs. */
      virtual bool train(UnitType type) = 0;

      /** Orders the unit to build the given unit type at the given position. Note that if the player does not
       * have enough resources when the unit attempts to place the building down, the order will fail. The
       * tile position specifies where the top left corner of the building will be placed. */
      virtual bool build(TilePosition position, UnitType type) = 0;

      /** Orders the unit to build the given addon. The unit must be a Terran building that can have an addon
       * and the specified unit type must be an addon unit type. */
      virtual bool buildAddon(UnitType type) = 0;

      /** Orders the unit to research the given tech type.
       * \see Unit::cancelResearch, Unit::Unit#isResearching, Unit::getRemainingResearchTime, Unit::getTech. */
      virtual bool research(TechType tech) = 0;

      /** Orders the unit to upgrade the given upgrade type.
       * \see Unit::cancelUpgrade, Unit::Unit#isUpgrading, Unit::getRemainingUpgradeTime, Unit::getUpgrade. */
      virtual bool upgrade(UpgradeType upgrade) = 0;

      /** Orders the unit to stop. */
      virtual bool stop() = 0;

      /** Orders the unit to hold its position.*/
      virtual bool holdPosition() = 0;

      /** Orders the unit to patrol between its current position and the specified position.
       * \see Unit::isPatrolling.  */
      virtual bool patrol(Position position) = 0;

      /** Orders the unit to follow the specified unit.
       * \see Unit::isFollowing. */
      virtual bool follow(Unit* target) = 0;

      /** Orders the unit to set its rally position to the specified position.
       * \see Unit::setRallyUnit, Unit::getRallyPosition, Unit::getRallyUnit. */
      virtual bool setRallyPosition(Position target) = 0;

      /** Orders the unit to set its rally unit to the specified unit.
       * \see Unit::setRallyPosition, Unit::getRallyPosition, Unit::getRallyUnit. */
      virtual bool setRallyUnit(Unit* target) = 0;

      /** Orders the unit to repair the specified unit. Only Terran SCVs can be ordered to repair, and the
       * target must be a mechanical Terran unit or building.
       * \see Unit::isRepairing. */
      virtual bool repair(Unit* target) = 0;

      /** Orders the unit to return its cargo to a nearby resource depot such as a Command Center. Only
       * workers that are carrying minerals or gas can be ordered to return cargo.
       * \see Unit::isCarryingGas, Unit::isCarryingMinerals. */
      virtual bool returnCargo() = 0;

      /** Orders the unit to morph into the specified unit type. Returns false if given a wrong type.
       * \see Unit::cancelMorph, Unit::isMorphing. */
      virtual bool morph(UnitType type) = 0;

      /** Orders the unit to burrow. Either the unit must be a Zerg Lurker, or the unit must be a Zerg ground
       * unit and burrow tech must be researched.
       * \see: Unit::unburrow, Unit::isBurrowed. */
      virtual bool burrow() = 0;

      /** Orders the burrowed unit to unburrow.
       * \see: Unit::burrow, Unit::isBurrowed.
       * */
      virtual bool unburrow() = 0;

      /** Orders the unit to siege. Note: unit must be a Terran siege tank.
       * \see Unit::unsiege, Unit::isSieged. */
      virtual bool siege() = 0;

      /** Orders the unit to unsiege. Note: unit must be a Terran siege tank.
       * \see: Unit::unsiege, Unit::isSieged. */
      virtual bool unsiege() = 0;

      /** Orders the unit to cloak.
       * \see: Unit::decloak, Unit::isCloaked. */
      virtual bool cloak() = 0;

      /** Orders the unit to decloak.
       * \see: Unit::cloak, Unit::isCloaked. */
      virtual bool decloak() = 0;

      /** Orders the unit to lift. Note: unit must be a Terran building that can be lifted.
       * \see Unit::land, Unit::isLifted.  */
      virtual bool lift() = 0;

      /** Orders the unit to land. Note: unit must be a Terran building that is currently lifted.
       * \see Unit::lift, Unit::isLifted. */
      virtual bool land(TilePosition position) = 0;

      /** Orders the unit to load the target unit.
       * \see Unit::unload, Unit::unloadAll, Unit::getLoadedUnits, Unit:isLoaded. */
      virtual bool load(Unit* target) = 0;

      /** Orders the unit to unload the target unit.
       * \see Unit::load, Unit::unloadAll, Unit::getLoadedUnits, Unit:isLoaded. */
      virtual bool unload(Unit* target) = 0;

      /** Orders the unit to unload all loaded units at the unit's current position.
       * \see Unit::load, Unit::unload, Unit::unloadAll, Unit::getLoadedUnits, Unit:isLoaded. */
      virtual bool unloadAll() = 0;

      /** Orders the unit to unload all loaded units at the specified location. Unit should be a Terran
       * Dropship, Protoss Shuttle, or Zerg Overlord. If the unit is a Terran Bunker, the units will be
       * unloaded right outside the bunker, like in the first version of unloadAll.
       * \see Unit::load, Unit::unload, Unit::unloadAll, Unit::getLoadedUnits, Unit:isLoaded. */
      virtual bool unloadAll(Position position) = 0;

      /** Orders the building to stop being constructed.
       * \see Unit::beingConstructed. */
      virtual bool cancelConstruction() = 0;

      /** Orders the SCV to stop constructing the building, and the building is left in a partially complete
       * state until it is canceled, destroyed, or completed.
       * \see Unit::isConstructing. */
      virtual bool haltConstruction() = 0;

      /** Orders the unit to stop morphing.
       * \see Unit::morph, Unit::isMorphing. */
      virtual bool cancelMorph() = 0;

      /** Orders the unit to remove the last unit from its training queue.
       * \see Unit::train, Unit::cancelTrain, Unit::isTraining, Unit::getTrainingQueue. */
      virtual bool cancelTrain() = 0;

      /** Orders the unit to remove the specified unit from its training queue.
       * \see Unit::train, Unit::cancelTrain, Unit::isTraining, Unit::getTrainingQueue. */
      virtual bool cancelTrain(int slot) = 0;

      /** Orders the unit to stop making the addon. */
      virtual bool cancelAddon() = 0;

      /** Orders the unit to cancel a research in progress.
       * \see Unit::research, Unit::isResearching, Unit::getTech. */
      virtual bool cancelResearch() = 0;

      /** Orders the unit to cancel an upgrade in progress.
       * \see Unit::upgrade, Unit::isUpgrading, Unit::getUpgrade. */
      virtual bool cancelUpgrade() = 0;

      /** Orders the unit to use a tech not requiring a target (ie Stim Pack). Returns true if it is a valid
       * tech. */
      virtual bool useTech(TechType tech) = 0;

      /** Orders the unit to use a tech requiring a position target (ie Dark Swarm). Returns true if it is a
       * valid tech.*/
      virtual bool useTech(TechType tech, Position position) = 0;

      /** Orders the unit to use a tech requiring a unit target (ie Irradiate). Returns true if it is a valid
       * tech.*/
      virtual bool useTech(TechType tech, Unit* target) = 0;

  };
}

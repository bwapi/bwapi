#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  // Forwards
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class Order;
  class TechType;
  class UpgradeType;

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
  class UnitInterface
  {
  protected:
    virtual ~UnitInterface() {};
  public:
    virtual int getID() const = 0;

    virtual bool exists() const = 0;

    virtual int getReplayID() const = 0;

    virtual Player getPlayer() const = 0;

    virtual UnitType getType() const = 0;

    virtual Position getPosition() const = 0;

    TilePosition getTilePosition() const;

    virtual double getAngle() const = 0;

    virtual double getVelocityX() const = 0;

    virtual double getVelocityY() const = 0;

    int getLeft() const;

    int getTop() const;

    int getRight() const;

    int getBottom() const;

    virtual int getHitPoints() const = 0;

    virtual int getShields() const = 0;

    virtual int getEnergy() const = 0;

    virtual int getResources() const = 0;

    virtual int getResourceGroup() const = 0;

    int getDistance(Position target) const;

    int getDistance(Unit target) const;

    bool hasPath(Position target) const;

    bool hasPath(Unit target) const;

    virtual int getLastCommandFrame() const = 0;

    virtual UnitCommand getLastCommand() const = 0;

    virtual BWAPI::Player getLastAttackingPlayer() const = 0;

    virtual UnitType getInitialType() const = 0;

    virtual Position getInitialPosition() const = 0;

    virtual TilePosition getInitialTilePosition() const = 0;

    virtual int getInitialHitPoints() const = 0;

    virtual int getInitialResources() const = 0;

    virtual int getKillCount() const = 0;

    virtual int getAcidSporeCount() const = 0;

    virtual int getInterceptorCount() const = 0;

    virtual int getScarabCount() const = 0;

    virtual int getSpiderMineCount() const = 0;

    virtual int getGroundWeaponCooldown() const = 0;

    virtual int getAirWeaponCooldown() const = 0;

    virtual int getSpellCooldown() const = 0;

    virtual int getDefenseMatrixPoints() const = 0;

    virtual int getDefenseMatrixTimer() const = 0;

    virtual int getEnsnareTimer() const = 0;

    virtual int getIrradiateTimer() const = 0;

    virtual int getLockdownTimer() const = 0;

    virtual int getMaelstromTimer() const = 0;

    virtual int getOrderTimer() const = 0;

    virtual int getPlagueTimer() const = 0;

    virtual int getRemoveTimer() const = 0;

    virtual int getStasisTimer() const = 0;

    virtual int getStimTimer() const = 0;

    virtual UnitType getBuildType() const = 0;

    virtual UnitType::list getTrainingQueue() const = 0;

    virtual TechType getTech() const = 0;

    virtual UpgradeType getUpgrade() const = 0;

    virtual int getRemainingBuildTime() const = 0;

    virtual int getRemainingTrainTime() const = 0;

    virtual int getRemainingResearchTime() const = 0;

    virtual int getRemainingUpgradeTime() const = 0;

    virtual Unit getBuildUnit() const = 0;

    virtual Unit getTarget() const = 0;

    virtual Position getTargetPosition() const = 0;

    virtual Order getOrder() const = 0;

    virtual Order getSecondaryOrder() const = 0;

    virtual Unit getOrderTarget() const = 0;

    virtual Position getOrderTargetPosition() const = 0;

    virtual Position getRallyPosition() const = 0;

    virtual Unit getRallyUnit() const = 0;

    virtual Unit getAddon() const = 0;

    virtual Unit getNydusExit() const = 0;

    virtual Unit getPowerUp() const = 0;

    virtual Unit getTransport() const = 0;

    virtual Unitset getLoadedUnits() const = 0;

    int getSpaceRemaining() const;

    virtual Unit getCarrier() const = 0;

    virtual Unitset getInterceptors() const = 0;

    virtual Unit getHatchery() const = 0;

    virtual Unitset getLarva() const = 0;

    virtual bool hasNuke() const = 0;

    virtual bool isAccelerating() const = 0;

    virtual bool isAttacking() const = 0;

    virtual bool isAttackFrame() const = 0;

    bool isBeingConstructed() const;

    virtual bool isBeingGathered() const = 0;

    virtual bool isBeingHealed() const = 0;

    virtual bool isBlind() const = 0;

    virtual bool isBraking() const = 0;

    virtual bool isBurrowed() const = 0;

    virtual bool isCarryingGas() const = 0;

    virtual bool isCarryingMinerals() const = 0;

    virtual bool isCloaked() const = 0;

    virtual bool isCompleted() const = 0;

    virtual bool isConstructing() const = 0;

    bool isDefenseMatrixed() const;

    virtual bool isDetected() const = 0;

    bool isEnsnared() const;

    bool isFlying() const;

    bool isFollowing() const;

    virtual bool isGatheringGas() const = 0;

    virtual bool isGatheringMinerals() const = 0;

    virtual bool isHallucination() const = 0;

    bool isHoldingPosition() const;

    virtual bool isIdle() const = 0;

    virtual bool isInterruptible() const = 0;

    virtual bool isInvincible() const = 0;

    bool isInWeaponRange(Unit target) const;

    bool isIrradiated() const;

    virtual bool isLifted() const = 0;

    bool isLoaded() const;

    bool isLockedDown() const;

    bool isMaelstrommed() const;

    virtual bool isMorphing() const = 0;

    virtual bool isMoving() const = 0;

    virtual bool isParasited() const = 0;

    bool isPatrolling() const;

    bool isPlagued() const;

    bool isRepairing() const;

    bool isResearching() const;

    virtual bool isSelected() const = 0;

    bool isSieged() const;

    virtual bool isStartingAttack() const = 0;

    bool isStasised() const;

    bool isStimmed() const;

    virtual bool isStuck() const = 0;

    virtual bool isTraining() const = 0;

    virtual bool isUnderAttack() const = 0;

    virtual bool isUnderDarkSwarm() const = 0;

    virtual bool isUnderDisruptionWeb() const = 0;

    virtual bool isUnderStorm() const = 0;

    virtual bool isPowered() const = 0;

    bool isUpgrading() const;

    virtual bool isVisible(Player player = nullptr) const = 0;

    virtual bool isTargetable() const = 0;


    virtual bool issueCommand(UnitCommand command) = 0;


    virtual bool canIssueCommand(UnitCommand command, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canIssueCommandGrouped(UnitCommand command, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canCommand() const = 0;

    virtual bool canCommandGrouped(bool checkCommandibility = true) const = 0;

    virtual bool canIssueCommandType(UnitCommandType ct, bool checkCommandibility = true) const = 0;

    virtual bool canIssueCommandTypeGrouped(UnitCommandType ct, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canTargetUnit(Unit targetUnit, bool checkCommandibility = true) const = 0;

    virtual bool canAttack(bool checkCommandibility = true) const = 0;

    virtual bool canAttack(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    virtual bool canAttack(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canAttackGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canAttackGrouped(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;
    virtual bool canAttackGrouped(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canAttackMove(bool checkCommandibility = true) const = 0;

    virtual bool canAttackMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canAttackUnit(bool checkCommandibility = true) const = 0;

    virtual bool canAttackUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canAttackUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canAttackUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canBuild(bool checkCommandibility = true) const = 0;

    virtual bool canBuild(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canBuildAddon(bool checkCommandibility = true) const = 0;

    virtual bool canBuildAddon(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canTrain(bool checkCommandibility = true) const = 0;

    virtual bool canTrain(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canMorph(bool checkCommandibility = true) const = 0;

    virtual bool canMorph(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canResearch(bool checkCommandibility = true) const = 0;

    virtual bool canResearch(TechType type, bool checkCanIssueCommandType = true) const = 0;

    virtual bool canUpgrade(bool checkCommandibility = true) const = 0;

    virtual bool canUpgrade(UpgradeType type, bool checkCanIssueCommandType = true) const = 0;

    virtual bool canSetRallyPoint(bool checkCommandibility = true) const = 0;

    virtual bool canSetRallyPoint(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    virtual bool canSetRallyPoint(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canSetRallyPosition(bool checkCommandibility = true) const = 0;

    virtual bool canSetRallyUnit(bool checkCommandibility = true) const = 0;

    virtual bool canSetRallyUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canMove(bool checkCommandibility = true) const = 0;

    virtual bool canMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canPatrol(bool checkCommandibility = true) const = 0;

    virtual bool canPatrolGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canFollow(bool checkCommandibility = true) const = 0;

    virtual bool canFollow(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canGather(bool checkCommandibility = true) const = 0;

    virtual bool canGather(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canReturnCargo(bool checkCommandibility = true) const = 0;

    virtual bool canHoldPosition(bool checkCommandibility = true) const = 0;

    virtual bool canStop(bool checkCommandibility = true) const = 0;

    virtual bool canRepair(bool checkCommandibility = true) const = 0;

    virtual bool canRepair(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canBurrow(bool checkCommandibility = true) const = 0;

    virtual bool canUnburrow(bool checkCommandibility = true) const = 0;

    virtual bool canCloak(bool checkCommandibility = true) const = 0;

    virtual bool canDecloak(bool checkCommandibility = true) const = 0;

    virtual bool canSiege(bool checkCommandibility = true) const = 0;

    virtual bool canUnsiege(bool checkCommandibility = true) const = 0;

    virtual bool canLift(bool checkCommandibility = true) const = 0;

    virtual bool canLand(bool checkCommandibility = true) const = 0;

    virtual bool canLand(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canLoad(bool checkCommandibility = true) const = 0;

    virtual bool canLoad(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUnloadWithOrWithoutTarget(bool checkCommandibility = true) const = 0;

    virtual bool canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUnload(bool checkCommandibility = true) const = 0;

    virtual bool canUnload(Unit targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUnloadAll(bool checkCommandibility = true) const = 0;

    virtual bool canUnloadAllPosition(bool checkCommandibility = true) const = 0;

    virtual bool canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canRightClick(bool checkCommandibility = true) const = 0;

    virtual bool canRightClick(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    // @overload
    virtual bool canRightClick(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canRightClickGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canRightClickGrouped(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;
    virtual bool canRightClickGrouped(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canRightClickPosition(bool checkCommandibility = true) const = 0;

    virtual bool canRightClickPositionGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canRightClickUnit(bool checkCommandibility = true) const = 0;

    virtual bool canRightClickUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canRightClickUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canRightClickUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const = 0;

    virtual bool canHaltConstruction(bool checkCommandibility = true) const = 0;

    virtual bool canCancelConstruction(bool checkCommandibility = true) const = 0;

    virtual bool canCancelAddon(bool checkCommandibility = true) const = 0;

    virtual bool canCancelTrain(bool checkCommandibility = true) const = 0;

    virtual bool canCancelTrainSlot(bool checkCommandibility = true) const = 0;

    virtual bool canCancelTrainSlot(int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canCancelMorph(bool checkCommandibility = true) const = 0;

    virtual bool canCancelResearch(bool checkCommandibility = true) const = 0;

    virtual bool canCancelUpgrade(bool checkCommandibility = true) const = 0;

    virtual bool canUseTechWithOrWithoutTarget(bool checkCommandibility = true) const = 0;

    virtual bool canUseTechWithOrWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUseTech(BWAPI::TechType tech, Position target, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;
    virtual bool canUseTech(BWAPI::TechType tech, Unit target = nullptr, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUseTechWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUseTechUnit(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUseTechUnit(BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnits = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUseTechPosition(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canUseTechPosition(BWAPI::TechType tech, Position target, bool checkTargetsPositions = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

    virtual bool canPlaceCOP(bool checkCommandibility = true) const = 0;

    virtual bool canPlaceCOP(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const = 0;

  };
}

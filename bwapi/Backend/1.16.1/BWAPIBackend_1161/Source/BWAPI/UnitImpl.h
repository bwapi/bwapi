#pragma once

#include <BWAPI/Unitset.h>
#include <Util/Types.h>

#include <BWAPI/UnitCommand.h>
#include <BWAPI/Client/UnitData.h>

namespace BW
{ 
  class CUnit;
};
namespace BWAPI
{
  /**
   * Interface for broodwar unit, can be used to obtain any information and
   * issue commands.
   */
  class UnitImpl
  {
    public:

      UnitType      getBuildType() const;
      UnitType::list getTrainingQueue() const;
      TechType      getTech() const;
      UpgradeType   getUpgrade() const;
      int           getRemainingBuildTime() const;
      int           getRemainingTrainTime() const;
      int           getRemainingResearchTime() const;
      int           getRemainingUpgradeTime() const;
      Unit         getBuildUnit() const;

      Unit     getTarget() const;
      Position getTargetPosition() const;
      Order    getOrder() const;
      Unit     getOrderTarget() const;
      Position getOrderTargetPosition() const;
      Order    getSecondaryOrder() const;
      Position getRallyPosition() const;
      Unit     getRallyUnit() const;
      Unit     getAddon() const;
      Unit     getNydusExit() const;
      Unit     getPowerUp() const;

      Unit     getTransport() const;
      Unitset  getLoadedUnits() const;
      Unit     getCarrier() const;
      Unitset  getInterceptors() const;
      Unit     getHatchery() const;
      Unitset  getLarva() const;

      bool hasNuke() const;
      bool isAccelerating() const;
      bool isAttackFrame() const;
      bool isAttacking() const;
      bool isBeingGathered() const;
      bool isBeingHealed() const;
      bool isBlind() const;
      bool isBraking() const;
      bool isBurrowed() const;
      bool isCarryingGas() const;
      bool isCarryingMinerals() const;
      bool isCloaked() const;
      bool isCompleted() const;
      bool isConstructing() const;
      bool isDetected() const;
      bool isGatheringGas() const;
      bool isGatheringMinerals() const;
      bool isHallucination() const;
      bool isIdle() const;
      bool isInterruptible() const;
      bool isInvincible() const;
      bool isLifted() const;
      bool isMorphing() const;
      bool isMoving() const;
      bool isParasited() const;
      bool isSelected() const;
      bool isStartingAttack() const;
      bool isStuck() const;
      bool isTraining() const;
      bool isUnderAttack() const;
      bool isUnderDarkSwarm() const;
      bool isUnderDisruptionWeb() const;
      bool isUnderStorm() const;
      bool isPowered() const;
      bool isVisible(Player player = nullptr) const;
      bool isTargetable() const;

      bool canCommand() const;
      bool canCommandGrouped(bool checkCommandibility = true) const;
      bool canTargetUnit(Unit targetUnit, bool checkCommandibility = true) const;

      bool canAttack(bool checkCommandibility = true) const;
      bool canAttack(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canAttack(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canAttackGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canAttackGrouped(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canAttackGrouped(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canAttackMove(bool checkCommandibility = true) const;
      bool canAttackMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canAttackUnit(bool checkCommandibility = true) const;
      bool canAttackUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canAttackUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canAttackUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canBuild(bool checkCommandibility = true) const;
      bool canBuild(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canBuildAddon(bool checkCommandibility = true) const;
      bool canBuildAddon(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canTrain(bool checkCommandibility = true) const;
      bool canTrain(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canMorph(bool checkCommandibility = true) const;
      bool canMorph(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canResearch(bool checkCommandibility = true) const;
      bool canResearch(TechType type, bool checkCanIssueCommandType = true) const;
      bool canUpgrade(bool checkCommandibility = true) const;
      bool canUpgrade(UpgradeType type, bool checkCanIssueCommandType = true) const;
      bool canSetRallyPoint(bool checkCommandibility = true) const;
      bool canSetRallyPoint(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canSetRallyPoint(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canSetRallyPosition(bool checkCommandibility = true) const;
      bool canSetRallyUnit(bool checkCommandibility = true) const;
      bool canSetRallyUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canMove(bool checkCommandibility = true) const;
      bool canMoveGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canPatrol(bool checkCommandibility = true) const;
      bool canPatrolGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canFollow(bool checkCommandibility = true) const;
      bool canFollow(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canGather(bool checkCommandibility = true) const;
      bool canGather(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canReturnCargo(bool checkCommandibility = true) const;
      bool canHoldPosition(bool checkCommandibility = true) const;
      bool canStop(bool checkCommandibility = true) const;
      bool canRepair(bool checkCommandibility = true) const;
      bool canRepair(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canBurrow(bool checkCommandibility = true) const;
      bool canUnburrow(bool checkCommandibility = true) const;
      bool canCloak(bool checkCommandibility = true) const;
      bool canDecloak(bool checkCommandibility = true) const;
      bool canSiege(bool checkCommandibility = true) const;
      bool canUnsiege(bool checkCommandibility = true) const;
      bool canLift(bool checkCommandibility = true) const;
      bool canLand(bool checkCommandibility = true) const;
      bool canLand(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canLoad(bool checkCommandibility = true) const;
      bool canLoad(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUnloadWithOrWithoutTarget(bool checkCommandibility = true) const;
      bool canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUnload(bool checkCommandibility = true) const;
      bool canUnload(Unit targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUnloadAll(bool checkCommandibility = true) const;
      bool canUnloadAllPosition(bool checkCommandibility = true) const;
      bool canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canRightClick(bool checkCommandibility = true) const;
      bool canRightClick(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canRightClick(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canRightClickGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canRightClickGrouped(Position target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canRightClickGrouped(Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canRightClickPosition(bool checkCommandibility = true) const;
      bool canRightClickPositionGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canRightClickUnit(bool checkCommandibility = true) const;
      bool canRightClickUnit(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canRightClickUnitGrouped(bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canRightClickUnitGrouped(Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canHaltConstruction(bool checkCommandibility = true) const;
      bool canCancelConstruction(bool checkCommandibility = true) const;
      bool canCancelAddon(bool checkCommandibility = true) const;
      bool canCancelTrain(bool checkCommandibility = true) const;
      bool canCancelTrainSlot(bool checkCommandibility = true) const;
      bool canCancelTrainSlot(int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canCancelMorph(bool checkCommandibility = true) const;
      bool canCancelResearch(bool checkCommandibility = true) const;
      bool canCancelUpgrade(bool checkCommandibility = true) const;
      bool canUseTechWithOrWithoutTarget(bool checkCommandibility = true) const;
      bool canUseTechWithOrWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUseTech(BWAPI::TechType tech, Unit target = nullptr, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUseTech(BWAPI::TechType tech, Position target, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUseTechWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUseTechUnit(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUseTechUnit(BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnits = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUseTechPosition(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canUseTechPosition(BWAPI::TechType tech, Position target, bool checkTargetsPositions = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canPlaceCOP(bool checkCommandibility = true) const;
      bool canPlaceCOP(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;

      bool canIssueCommandType(BWAPI::UnitCommandType ct, bool checkCommandibility = true) const;
      bool canIssueCommandTypeGrouped(BWAPI::UnitCommandType ct, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;
      bool canIssueCommand(UnitCommand command, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      bool canIssueCommandGrouped(UnitCommand command, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true) const;

      bool issueCommand(UnitCommand command);

      //Internal BWAPI commands:
      UnitImpl(BW::CUnit* originalUnit, u16 index);
      static UnitImpl* BWUnitToBWAPIUnit(BW::CUnit* unit);
      void die();
      void setID(int newID);
      bool canAccess() const;
      bool canAccessDetected() const;
      bool canAccessInside() const;

      /**
       * Gets index of the unit in the unit array. Note that the index is same
       * for the original unit array, BWAPI::Unit array and BWAI::Unit array, so
       * it is good way to compare units of different types and for conversion.
       * @return 0-based index of the unit in the unit array.
       */
      u16 getIndex() const;
      void setSelected(bool selectedState);
      UnitImpl* getNext() const;

      void saveInitialState();
      void updateInternalData();
      void updateData();
      
    // data members
      Player _getPlayer;
      UnitType _getType;
      Position _getPosition;
      int _getResources;
      int _getHitPoints;
      Unit _getTransport;
      
      /** Gets #bwOriginalUnit */
      BW::CUnit* getOriginalRawData;
      /** Gets #bwUnitLocal */
      u8 getBuildQueueSlot;
      /** Gets #bwUnit->BW#Unit#buildQueue */
      BWAPI::UnitType getBuildQueue[5];
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue;

      UnitData data;

      bool userSelected = false;
      bool nukeDetected = false;
      Position nukePosition;
      int lastGroundWeaponCooldown = 0;
      int lastAirWeaponCooldown = 0;
      int lastFrameSet = -1;
      UnitType lastType = UnitTypes::Unknown;
      Player lastPlayer = nullptr;
      int id = -1;
      bool isAlive = false;
      bool wasAlive = false;
      bool _isCompleted = false;
      bool wasCompleted = false;
      bool wasAccessible = false;
      bool wasVisible = false;
      
      Unitset connectedUnits;
      Unitset loadedUnits;

      int lastCommandFrame = 0;
      UnitCommand lastCommand;

      int lastImmediateCommandFrame = 0;
      UnitCommand lastImmediateCommand;

      static void setLastImmediateCommand(const UnitCommand &command);
      bool prepareIssueCommand(UnitCommand &command);

      void clear();
    private:
      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();

      u16 index; /**< Index of the unit in the array (can be computed, it's just optimisation) */
      UnitType initialType;
      BWAPI::Position initialPosition;
      int initialResources;
      int initialHitPoints;
  };
};


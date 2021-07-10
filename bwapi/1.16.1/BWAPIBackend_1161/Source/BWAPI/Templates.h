#pragma once

#include <unordered_map>

#include <BWAPI/Position.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitCommandType.h>

#include <BWAPI4/Unitset.h>

#include "GameImpl.h"

namespace BWAPI4
{
  class UnitImpl;
  typedef UnitImpl* Unit;

  namespace Templates
  {
    //--------------------------------------------- FORWARD DECLARATIONS -------------------------------------
    bool canAttackMove(Unit thisUnit, bool checkCommandibility = true);
    bool canAttackMoveGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canAttackUnit(Unit thisUnit, bool checkCommandibility = true);
    bool canAttackUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canAttackUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canAttackUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canSetRallyPosition(Unit thisUnit, bool checkCommandibility = true);
    bool canSetRallyUnit(Unit thisUnit, bool checkCommandibility = true);
    bool canSetRallyUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canMove(Unit thisUnit, bool checkCommandibility = true);
    bool canRightClickPosition(Unit thisUnit, bool checkCommandibility = true);
    bool canRightClickPositionGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canRightClickUnit(Unit thisUnit, bool checkCommandibility = true);
    bool canRightClickUnit(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canRightClickUnitGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canRightClickUnitGrouped(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canUseTechWithoutTarget(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUseTechUnit(Unit thisUnit, BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnits = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUseTechPosition(Unit thisUnit, BWAPI::TechType tech, BWAPI::Position target, bool checkTargetsPositions = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);

    bool hasPower(int x, int y, BWAPI::UnitType unitType, const Unitset& pylons);
    bool canBuildHere(Unit builder, BWAPI::TilePosition position, BWAPI::UnitType type, bool checkExplored);
    bool canMake(Unit builder, BWAPI::UnitType type);
    bool canCommand(Unit thisUnit);
    bool canCommandGrouped(Unit thisUnit, bool checkCommandibility = true);
    bool canTargetUnit(Unit targetUnit);
    bool canTargetUnit(Unit thisUnit, Unit targetUnit, bool checkCommandibility = true);
    bool canAttack(Unit thisUnit, bool checkCommandibility = true);
    bool canAttack(Unit thisUnit, BWAPI::Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canAttack(Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canAttackGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canAttackGrouped(Unit thisUnit, BWAPI::Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canAttackGrouped(Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canBuild(Unit thisUnit, bool checkCommandibility = true);
    bool canBuild(Unit thisUnit, BWAPI::UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canBuild(Unit thisUnit, BWAPI::UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canBuildAddon(Unit thisUnit, bool checkCommandibility = true);
    bool canBuildAddon(Unit thisUnit, BWAPI::UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canTrain(Unit thisUnit, bool checkCommandibility = true);
    bool canTrain(Unit thisUnit, BWAPI::UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canMorph(Unit thisUnit, bool checkCommandibility = true);
    bool canMorph(Unit thisUnit, BWAPI::UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canResearch(Unit thisUnit, bool checkCommandibility = true);
    bool canResearch(Unit thisUnit, BWAPI::TechType type, bool checkCanIssueCommandType = true);
    bool canUpgrade(Unit thisUnit, bool checkCommandibility = true);
    bool canUpgrade(Unit thisUnit, BWAPI::UpgradeType type, bool checkCanIssueCommandType = true);
    bool canSetRallyPoint(Unit thisUnit, bool checkCommandibility = true);
    bool canSetRallyPoint(Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canSetRallyPoint(Unit thisUnit, BWAPI::Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canMoveGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canPatrol(Unit thisUnit, bool checkCommandibility = true);
    bool canPatrolGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canFollow(Unit thisUnit, bool checkCommandibility = true);
    bool canFollow(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canGather(Unit thisUnit, bool checkCommandibility = true);
    bool canGather(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canReturnCargo(Unit thisUnit, bool checkCommandibility = true);
    bool canHoldPosition(Unit thisUnit, bool checkCommandibility = true);
    bool canStop(Unit thisUnit, bool checkCommandibility = true);
    bool canRepair(Unit thisUnit, bool checkCommandibility = true);
    bool canRepair(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canBurrow(Unit thisUnit, bool checkCommandibility = true);
    bool canUnburrow(Unit thisUnit, bool checkCommandibility = true);
    bool canCloak(Unit thisUnit, bool checkCommandibility = true);
    bool canDecloak(Unit thisUnit, bool checkCommandibility = true);
    bool canSiege(Unit thisUnit, bool checkCommandibility = true);
    bool canUnsiege(Unit thisUnit, bool checkCommandibility = true);
    bool canLift(Unit thisUnit, bool checkCommandibility = true);
    bool canLand(Unit thisUnit, bool checkCommandibility = true);
    bool canLand(Unit thisUnit, BWAPI::TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canLoad(Unit thisUnit, bool checkCommandibility = true);
    bool canLoad(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUnloadWithOrWithoutTarget(Unit thisUnit, bool checkCommandibility = true);
    bool canUnloadAtPosition(Unit thisUnit, BWAPI::Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUnload(Unit thisUnit, bool checkCommandibility = true);
    bool canUnload(Unit thisUnit, Unit targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUnloadAll(Unit thisUnit, bool checkCommandibility = true);
    bool canUnloadAllPosition(Unit thisUnit, bool checkCommandibility = true);
    bool canUnloadAllPosition(Unit thisUnit, BWAPI::Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canRightClick(Unit thisUnit, bool checkCommandibility = true);
    bool canRightClick(Unit thisUnit, BWAPI::Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canRightClick(Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canRightClickGrouped(Unit thisUnit, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canRightClickGrouped(Unit thisUnit, BWAPI::Position, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canRightClickGrouped(Unit thisUnit, Unit target, bool checkCanTargetUnit = true, bool checkCanIssueCommandTypeGrouped = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canHaltConstruction(Unit thisUnit, bool checkCommandibility = true);
    bool canCancelConstruction(Unit thisUnit, bool checkCommandibility = true);
    bool canCancelAddon(Unit thisUnit, bool checkCommandibility = true);
    bool canCancelTrain(Unit thisUnit, bool checkCommandibility = true);
    bool canCancelTrainSlot(Unit thisUnit, bool checkCommandibility = true);
    bool canCancelTrainSlot(Unit thisUnit, int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canCancelMorph(Unit thisUnit, bool checkCommandibility = true);
    bool canCancelResearch(Unit thisUnit, bool checkCommandibility = true);
    bool canCancelUpgrade(Unit thisUnit, bool checkCommandibility = true);
    bool canUseTechWithOrWithoutTarget(Unit thisUnit, bool checkCommandibility = true);
    bool canUseTechWithOrWithoutTarget(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUseTech(Unit thisUnit, BWAPI::TechType tech, BWAPI::Position target, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUseTech(Unit thisUnit, BWAPI::TechType tech, Unit target = nullptr, bool checkCanTargetUnit = true, bool checkTargetsType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUseTechUnit(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canUseTechPosition(Unit thisUnit, BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canPlaceCOP(Unit thisUnit, bool checkCommandibility = true);
    bool canPlaceCOP(Unit thisUnit, BWAPI::TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canIssueCommandType(Unit thisUnit, BWAPI::UnitCommandType ct, bool checkCommandibility = true);
    bool canIssueCommandTypeGrouped(Unit thisUnit, BWAPI::UnitCommandType ct, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);
    bool canIssueCommand(Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true);
    bool canIssueCommandGrouped(Unit thisUnit, UnitCommand c, bool checkCanUseTechPositionOnPositions = true, bool checkCanUseTechUnitOnUnits = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibilityGrouped = true, bool checkCommandibility = true);

    //-------------------------------------------- UNIT FINDER -----------------------------------------------
    template <class finder, typename _T>
    void iterateUnitFinder(finder* finder_x, finder* finder_y, int finderCount, int left, int top, int right, int bottom, const _T& callback)
    {
      // Note that the native finder in Broodwar uses an id between 1 and 1700, 0 being an unused entry
      // IDs provided by the client are BWAPI IDs, which are not bound
      std::unordered_map<unsigned, unsigned> finderFlags;

      // Declare some variables
      int r = right, b = bottom;
      bool isWidthExtended = right - left + 1 < BWAPI::UnitTypes::maxUnitWidth();
      bool isHeightExtended = top - bottom + 1 < BWAPI::UnitTypes::maxUnitHeight();

      // Check if the location is smaller than the largest unit
      if (isWidthExtended)
        r += BWAPI::UnitTypes::maxUnitWidth();
      if (isHeightExtended)
        b += BWAPI::UnitTypes::maxUnitHeight();

      // Obtain finder indexes for all bounds
      finder* p_xend = finder_x + finderCount;
      finder* p_yend = finder_y + finderCount;

      // Create finder elements for compatibility with stl functions
      finder finderVal;

      // Search for the values using built-in binary search algorithm and comparator
      const auto cmp = [](const finder& a, const finder& b) { return a.searchValue < b.searchValue; };

      finderVal.searchValue = left;
      finder* pLeft = std::lower_bound(finder_x, p_xend, finderVal, cmp);

      finderVal.searchValue = top;
      finder* pTop = std::lower_bound(finder_y, p_yend, finderVal, cmp);

      finderVal.searchValue = r + 1;
      finder* pRight = std::upper_bound(pLeft, p_xend, finderVal, cmp);

      finderVal.searchValue = b + 1;
      finder* pBottom = std::upper_bound(pTop, p_yend, finderVal, cmp);

      // Iterate the X entries of the finder
      for (finder* px = pLeft; px < pRight; ++px)
      {
        int iUnitIndex = px->unitIndex;
        if (finderFlags[iUnitIndex] == 0)
        {
          if (isWidthExtended)  // If width is small, check unit bounds
          {
            Unit u = BroodwarImpl._unitFromIndex(iUnitIndex);
            if (u && u->getLeft() <= right)
              finderFlags[iUnitIndex] = 1;
          }
          else
            finderFlags[iUnitIndex] = 1;
        }
      }
      // Iterate the Y entries of the finder
      for (finder* py = pTop; py < pBottom; ++py)
      {
        int iUnitIndex = py->unitIndex;
        if (finderFlags[iUnitIndex] == 1)
        {
          if (isHeightExtended) // If height is small, check unit bounds
          {
            Unit u = BroodwarImpl._unitFromIndex(iUnitIndex);
            if (u && u->getTop() <= bottom)
              finderFlags[iUnitIndex] = 2;
          }
          else
            finderFlags[iUnitIndex] = 2;
        }
      }
      // Final Iteration
      for (finder* px = pLeft; px < pRight; ++px)
      {
        int iUnitIndex = px->unitIndex;
        if (finderFlags[iUnitIndex] == 2)
        {
          Unit u = BroodwarImpl._unitFromIndex(iUnitIndex);
          if (u && u->exists())
            callback(u);
        }
        // Reset finderFlags so that callback isn't called for duplicates
        finderFlags[iUnitIndex] = 0;
      }
    }
  }
}

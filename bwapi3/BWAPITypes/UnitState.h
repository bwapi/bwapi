#pragma once

#include <BWAPITypes\Position.h>
#include <BWAPITypes\UnitTypeId.h>

namespace BWAPI
{
  // access for a unit the agent has knowledge of
  typedef int ClearanceLevel;
  namespace ClearanceLevels
  {
    enum
    {
      None = 0, // should not be used. unit gets removed
      Noticed = 1,
      Detected = 2,
      Visible = 3,
      Full = 4  // highest clearance level
    };
  }
  // Unit's state we know about, depending on clearance level
  struct UnitStateNoticed
  {
    // in the root object
    ClearanceLevel clearanceLevel;
    //replace with position object later
    Position position;
    bool isAccelerating;
    bool isMoving;
    bool isIdle;
    UnitTypeId type;
    int removeTimer;
    int id;
    bool isUnderStorm;
  };
  struct UnitStateDetected : UnitStateNoticed
  {
    int player;
    int hitPoints;
    int shields;
    int energy;
    int resources;
    int killCount;
    bool isBeingConstructed;
    bool isBeingGathered;
    bool isBeingHealed;
    bool isBlind;
    bool isBraking;
    bool isBurrowed;
    bool isCarryingGas;
    bool isCarryingMinerals;
    bool isCloaked;
    bool isCompleted;
    bool isConstructing;
    bool isDefenseMatrixed;
    bool isEnsnared;
    bool isGatheringGas;
    bool isGatheringMinerals;
    bool isIrradiated;
    bool isLifted;
    bool isLockedDown;
    bool isMaelstrommed;
    bool isMorphing;
    bool isParasited;
    bool isPlagued;
    bool isRepairing;
    bool isResearching;
    bool isSelected;
    bool isSieged;
    bool isStasised;
    bool isStimmed;
    bool isTraining;
    bool isUnpowered;
    bool isUpgrading;

    int groundWeaponCooldown;
    int airWeaponCooldown;
    int spellCooldown;
  };
  struct UnitStateVisible : UnitStateDetected
  {
    // i found no states that are accessible when visible
    // but are not accessible when detected. if this keeps up,
    // remove detected, weld things, and let agent use
    // isCloaked to see if it's just visible or detected
  };
  struct UnitStateFull : UnitStateVisible
  {
    // you could only possibly see this if you own it
    bool isHallucination;
    bool isLoaded;

    // you can't see timer from enemy units, can you?
    int defenseMatrixTimer;
    int ensnareTimer;
    int irradiateTimer;
    int lockdownTimer;
    int maelstromTimer;
    int plagueTimer;
    int stasisTimer;
    int stimTimer;
    int defenseMatrixPoints;

    // orders you can only get from your own units
    bool isFollowing;
    bool isPatrolling;
  };
  typedef UnitStateFull UnitState;      // all states unified

}

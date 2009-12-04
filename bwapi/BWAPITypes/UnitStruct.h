#pragma once
namespace BWAPI
{
  // access for a unit the agent has knowledge of
  typedef int ClearanceLevel;
  namespace ClearanceLevels
  {
    enum
    {
      None = 0,
      Basic = 1,
      Detected = 2,
      Visible = 3,
      Full = 4  // highest clearance level
    };
  }
  // Unit's state we know about, depending on clearance level
  struct StateNoticed
  {
    int getID;
    int getInitialType;
    int getInitialHitPoints;
    int getInitialResources;
    //PlayerStruct* getInitialPlayer;
  };
  struct StateDetected : StateNoticed
  {
    //replace with position object later
    int x;
    int y;
  };
  struct StateVisible : StateDetected
  {
    //todo: move some of these to fully observable
    int getPlayer;
    int getType;
    int getHitPoints;
    int getShields;
    int getEnergy;
    int getResources;
    int getKillCount;
    int getGroundWeaponCooldown;
    int getAirWeaponCooldown;
    int getSpellCooldown;
    int getDefenseMatrixPoints;

    int getDefenseMatrixTimer;
    int getEnsnareTimer;
    int getIrradiateTimer;
    int getLockdownTimer;
    int getMaelstromTimer;
    int getPlagueTimer;
    int getRemoveTimer;
    int getStasisTimer;
    int getStimTimer;

    bool exists;
    bool isAccelerating;
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
    bool isFollowing;
    bool isGatheringGas;
    bool isGatheringMinerals;
    bool isHallucination;
    bool isIdle;
    bool isIrradiated;
    bool isLifted;
    bool isLoaded;
    bool isLockedDown;
    bool isMaelstrommed;
    bool isMorphing;
    bool isMoving;
    bool isParasited;
    bool isPatrolling;
    bool isPlagued;
    bool isRepairing;
    bool isResearching;
    bool isSelected;
    bool isSieged;
    bool isStartingAttack;
    bool isStasised;
    bool isStimmed;
    bool isTraining;
    bool isUnderStorm;
    bool isUnpowered;
    bool isUpgrading;
    bool isVisible;
  };
  struct StateFullyObservable : StateVisible
  {
    bool isResearching;//can not get this from visible enemy units
  };
  typedef StateFullyObservable State;      // all states unified

}
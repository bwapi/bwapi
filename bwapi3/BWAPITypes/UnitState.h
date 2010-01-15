#pragma once

#include "Position.h"
#include "UnitTypeId.h"
#include "UnitId.h"
#include "PlayerId.h"
#include "Order.h"
#include "Target.h"

#include <Util\StaticVector.h>

namespace BWAPI
{
  // access for a unit the agent has knowledge of
  typedef int ClearanceLevel;
  namespace ClearanceLevels
  {
    enum
    {
      None = 0,     // never appearsto the AI, unit gets removed
      Noticed = 1,  // unit is cloacked but shimering
      Visible = 2,  // unit is visible/detected
      Full = 3      // unit belongs to you
    };
  }
  // Unit's state we know about, depending on clearance level
  struct UnitStateNoticed
  {
    // basic data
    ClearanceLevel clearanceLevel;
    int id;
    int removeTimer;
    
    // position is noticable
    Position position;
    bool isAccelerating;
    bool isMoving;
    bool isIdle;
    double angle;
    Util::Point<double> velocity;

    // position related stuff is noticable too
    bool isUnderStorm;

    // a human can well guess the type
    UnitTypeId type;

    // debug value
    int debug;
  };
  struct UnitStateVisible : UnitStateNoticed
  {
    PlayerId player;
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

    UnitId buildUnit;   // bidirectional

    int groundWeaponCooldown;
    int airWeaponCooldown;
    int spellCooldown;
  };
  struct UnitStateFull : UnitStateVisible
  {
    // you could only possibly see this if you own it
    bool isHallucination;
    bool isLoaded;

    // connected units
    union
    {
      UnitId  hatchery;
      UnitId  nuclearMissile;
      UnitId  ghost;
    };

    // timers
    int defenseMatrixTimer;
    int ensnareTimer;
    int irradiateTimer;
    int lockdownTimer;
    int maelstromTimer;
    int plagueTimer;
    int stasisTimer;
    int stimTimer;
    int defenseMatrixPoints;

    // unit's orders
    Order order;

    Util::StaticVector<UnitId, 5> trainingQueue;

    // Broodwar's unit movement engine
    UnitId movementTargetUnit;
    Position movementTargetPosition;

    // this is Broodwar pathfinding system's next shortstep.
    Position movementNextWaypoint;
  };
  typedef UnitStateFull UnitState;      // all states unified

}

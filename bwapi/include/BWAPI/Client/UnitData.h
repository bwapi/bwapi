#pragma once

#include <BWAPI/IDs.h>
#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitCommand.h>

#include <array>
#include "BWAPI/UnitCommandType.h"

namespace BWAPI
{
  class UnitCommand;

  struct UnitData {
    UnitData(UnitID id) {
      trainingQueue.reserve(5);
      loadedUnits.reserve(8);
    }

    int clearanceLevel;
    BWAPI::UnitID id{-1};
    BWAPI::PlayerID player{-1};
    BWAPI::UnitType type;
    BWAPI::Position position;
    double angle;
    double velocityX;
    double velocityY;
    int hitPoints;
    int lastHitPoints;
    int shields;
    int energy;
    int resources;
    int resourceGroup;

    int killCount;
    int acidSporeCount;
    int scarabCount;
    int interceptorCount;
    int spiderMineCount;
    int groundWeaponCooldown;
    int airWeaponCooldown;
    int spellCooldown;
    int defenseMatrixPoints;

    int defenseMatrixTimer;
    int ensnareTimer;
    int irradiateTimer;
    int lockdownTimer;
    int maelstromTimer;
    int orderTimer;
    int plagueTimer;
    int removeTimer;
    int stasisTimer;
    int stimTimer;

    BWAPI::UnitType buildType;
    BWAPI::UnitType::list trainingQueue;
    BWAPI::UnitID::list loadedUnits;
    BWAPI::TechType tech;
    BWAPI::UpgradeType upgrade;
    int remainingBuildTime;
    int remainingTrainTime;
    int remainingResearchTime;
    int remainingUpgradeTime;
    BWAPI::UnitID buildUnit{-1};

    BWAPI::UnitID target{-1};
    BWAPI::Position targetPosition;
    BWAPI::Order order;
    BWAPI::UnitID orderTarget{-1};
    BWAPI::Position orderTargetPosition;
    BWAPI::Order secondaryOrder;
    BWAPI::Position rallyPosition;
    BWAPI::UnitID rallyUnit{-1};
    BWAPI::UnitID addon{-1};
    BWAPI::UnitID nydusExit{-1};
    BWAPI::UnitID powerUp{-1};

    BWAPI::UnitID transport{-1};
    BWAPI::UnitID carrier{-1};
    BWAPI::UnitID hatchery{-1};
    
    bool exists;
    bool hasNuke;
    bool isAccelerating;
    bool isAttacking;
    bool isAttackFrame;
    bool isBeingGathered;
    bool isBeingHealed;
    bool isBlind;
    bool isBraking;
    bool isBurrowed;
    int  carryResourceType;
    bool isCloaked;
    bool isCompleted;
    bool isConstructing;
    bool isDetected;
    bool isGathering;
    bool isHallucination;
    bool isIdle;
    bool isInterruptible;
    bool isInvincible;
    bool isLifted;
    bool isMorphing;
    bool isMoving;
    bool isParasited;
    bool isSelected;
    bool isStartingAttack;
    bool isStuck;
    bool isTraining;
    bool isUnderStorm;
    bool isUnderDarkSwarm;
    bool isUnderDWeb;
    bool isPowered;
    bool isVisible[9];
    int  buttonset;

    int lastCommandFrame;
    BWAPI::UnitCommand lastCommand{};

    BWAPI::PlayerID lastAttackerPlayer{-1};
    BWAPI::UnitID replayID{-1};
    bool recentlyAttacked;
  };
}

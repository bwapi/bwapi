#pragma once

namespace BWAPI4
{
  struct UnitData
  {
    int id;
    int player;
    int type;
    int positionX;
    int positionY;
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

    int buildType;
    int trainingQueueCount;
    int trainingQueue[5];
    int tech;
    int upgrade;
    int remainingBuildTime;
    int remainingTrainTime;
    int remainingResearchTime;
    int remainingUpgradeTime;
    int buildUnit;

    int target;
    int targetPositionX;
    int targetPositionY;
    int order;
    int orderTarget;
    int orderTargetPositionX;
    int orderTargetPositionY;
    int secondaryOrder;
    int rallyPositionX;
    int rallyPositionY;
    int rallyUnit;
    int addon;
    int nydusExit;
    int powerUp;

    int transport;
    int carrier;
    int hatchery;
    
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

    bool recentlyAttacked;
    int  replayID;
  };
}

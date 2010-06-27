#pragma once

namespace BWAPIC
{
  struct UnitData
  {
    int clearanceLevel;
    int id;
    int player;
    int type;
    int positionX;
    int positionY;
    double angle;
    double velocityX;
    double velocityY;
    int hitPoints;
    int shields;
    int energy;
    int resources;

    int killCount;
    //int interceptorCount; connectedUnits.size()
    int scarabCount;
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
    int secondaryOrder;
    int child;
    int rallyPositionX;
    int rallyPositionY;
    int rallyUnit;
    int addon;
    int nydusExit;

    int transport;
    int carrier;
    int hatchery;
    
    bool exists;
    bool hasNuke;
    bool isAccelerating;
    bool isAttacking;
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
    //bool isDefenseMatrixed; defenseMatrixTimer > 0
    //bool isEnsnared; ensnareTimer > 0
    //bool isFollowing; order == Orders::Follow.getID()
    bool isGatheringGas;
    bool isGatheringMinerals;
    bool isHallucination;
    bool isIdle;
    //bool isIrradiated; irradiateTimer > 0
    bool isLifted;
    //bool isLoaded; transport != -1
    //bool isLockedDown; lockdownTimer > 0
    //bool isMaelstrommed; maelstromTimer > 0
    bool isMorphing;
    bool isMoving;
    bool isParasited;
    //bool isPatrolling; order == Orders::Patrol.getID()
    //bool isPlagued; plagueTimer > 0
    //bool isRepairing; order == Orders::Repair1.getID() || order == Orders::Repair2.getID()
    //bool isResearching; order == Orders::ResearchTech.getID()
    bool isSelected;
    //bool isSieged; type == UnitTypes::Terran_Siege_Tank_Siege_Mode
    bool isStartingAttack;
    //bool isStasised; stasisTimer > 0
    //bool isStimmed; stimTimer > 0
    bool isTraining;
    bool isUnderStorm;
    bool isUnpowered;
    //bool isUpgrading; order == Orders::Upgrade.getID()
    bool isVisible[9];
  };
}

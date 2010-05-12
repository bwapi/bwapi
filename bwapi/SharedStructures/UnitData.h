#pragma once

namespace BWAPIC
{
  struct UnitData
  {
    int clearanceLevel;
    int id;
    int player;
    int type;
    int hitPoints;
    int shields;
    int energy;
    int resources;
    int killCount;
    int groundWeaponCooldown;
    int airWeaponCooldown;
    int spellCooldown;
    int defenseMatrixPoints;

    int defenseMatrixTimer;
    int ensnareTimer;
    int irradiateTimer;
    int lockdownTimer;
    int maelstromTimer;
    int plagueTimer;
    int removeTimer;
    int stasisTimer;
    int stimTimer;
    int positionX;
    int positionY;
    double angle;
    double velocityX;
    double velocityY;
    int target;
    int targetPositionX;
    int targetPositionY;
    int order;
    int orderTarget;
    int orderTimer;
    int secondaryOrder;
    int buildUnit;
    int buildType;
    int remainingBuildTime;
    int remainingTrainTime;
    int child;
    int trainingQueueCount;
    int trainingQueue[5];
    int transport;
    int loadedUnitCount;
    int loadedUnits[8];
    int interceptorCount;
    int scarabCount;
    int spiderMineCount;
    int tech;
    int upgrade;
    int remainingResearchTime;
    int remainingUpgradeTime;
    int rallyPositionX;
    int rallyPositionY;
    int rallyUnit;
    int addon;
    int hatchery;
    int larva;
    
    bool exists;
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
    bool isLoaded;
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

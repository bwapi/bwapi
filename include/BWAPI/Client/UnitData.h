#pragma once

#include <BWAPI/IDs.h>
#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class Game;

  struct UnitData {
    UnitData(Game &g, UnitID id): game{g}, id { id } {
      trainingQueue.reserve(5);
      loadedUnits.reserve(8);
    }

    Game &game;
    BWAPI::UnitID id{ UnitID::None };
    BWAPI::PlayerID player{ PlayerID::None };
    BWAPI::UnitType type{ UnitTypes::None };
    BWAPI::Position position{ Positions::None };
    double angle = 0;
    double velocityX = 0;
    double velocityY = 0;
    int hitPoints = 0;
    int lastHitPoints = 0;
    int shields = 0;
    int energy = 0;
    int resources = 0;
    int resourceGroup = 0;

    int killCount = 0;
    int acidSporeCount = 0;
    int scarabCount = 0;
    int interceptorCount = 0;
    int spiderMineCount = 0;
    int groundWeaponCooldown = 0;
    int airWeaponCooldown = 0;
    int spellCooldown = 0;
    int defenseMatrixPoints = 0;

    int defenseMatrixTimer = 0;
    int ensnareTimer = 0;
    int irradiateTimer = 0;
    int lockdownTimer = 0;
    int maelstromTimer = 0;
    // @TODO REMOVE orderTimer
    int orderTimer = 0;
    int plagueTimer = 0;
    int removeTimer = 0;
    int stasisTimer = 0;
    int stimTimer = 0;

    BWAPI::UnitType buildType{ UnitTypes::None };
    BWAPI::UnitType::list trainingQueue;
    BWAPI::UnitID::list loadedUnits;
    BWAPI::UnitID::list interceptors;
    BWAPI::UnitID::list larva;
    BWAPI::TechType tech{ TechTypes::None };
    BWAPI::UpgradeType upgrade{ UpgradeTypes::None };
    int remainingBuildTime = 0;
    int remainingTrainTime = 0;
    int remainingResearchTime = 0;
    int remainingUpgradeTime = 0;
    BWAPI::UnitID buildUnit{ UnitID::None };

    BWAPI::UnitID target{ UnitID::None };
    BWAPI::Position targetPosition{ Positions::None };
    BWAPI::Order order{ Orders::None };
    BWAPI::UnitID orderTarget{ UnitID::None };
    BWAPI::Position orderTargetPosition{ Positions::None };
    BWAPI::Order secondaryOrder{ Orders::None };
    BWAPI::Position rallyPosition{ Positions::None };
    BWAPI::UnitID rallyUnit{ UnitID::None };
    BWAPI::UnitID addon{ UnitID::None };
    BWAPI::UnitID nydusExit{ UnitID::None };
    BWAPI::UnitID powerUp{ UnitID::None };

    BWAPI::UnitID transport{ UnitID::None };
    BWAPI::UnitID carrier{ UnitID::None };
    BWAPI::UnitID hatchery{ UnitID::None };
    
    bool exists = false;
    bool hasNuke = false;
    bool isAccelerating = false;
    bool isAttacking = false;
    bool isAttackFrame = false;
    bool isBeingGathered = false;
    bool isBeingHealed = false;
    bool isBlind = false;
    bool isBraking = false;
    bool isBurrowed = false;
    int  carryResourceType = 0;
    bool isCloaked = false;
    bool isCompleted = false;
    bool isConstructing = false;
    bool isDetected = false;
    bool isGathering = false;
    bool isHallucination = false;
    bool isIdle = false;
    bool isInterruptible = false;
    bool isInvincible = false;
    bool isLifted = false;
    bool isMorphing = false;
    bool isMoving = false;
    bool isParasited = false;
    bool isSelected = false;
    bool isStartingAttack = false;
    bool isStuck = false;
    bool isTraining = false;
    bool isUnderStorm = false;
    bool isUnderDarkSwarm = false;
    bool isUnderDWeb = false;
    bool isPowered = false;
    bool isVisible[9] = {};
    // @TODO REMOVE buttonset
    int  buttonset = 0;

    int lastCommandFrame = 0;
    BWAPI::UnitCommand lastCommand{};

    BWAPI::UnitID replayID{ UnitID::None };
    bool recentlyAttacked = false;
  };
}

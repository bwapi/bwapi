#pragma once

#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/TechType.h>

namespace BWAPI4
{
  struct PlayerData
  {
    char name[25];
    int  race;
    int  type;
    int  force;
    bool isAlly[12];
    bool isEnemy[12];
    bool isNeutral;
    int  startLocationX;
    int  startLocationY;
    bool isVictorious;
    bool isDefeated;
    bool leftGame;
    bool isParticipating;

    int minerals;
    int gas;
    int gatheredMinerals;
    int gatheredGas;
    int repairedMinerals;
    int repairedGas;
    int refundedMinerals;
    int refundedGas;
    int supplyTotal[3];
    int supplyUsed[3];

    int allUnitCount[BWAPI::UnitTypes::Enum::MAX];
    int visibleUnitCount[BWAPI::UnitTypes::Enum::MAX];
    int completedUnitCount[BWAPI::UnitTypes::Enum::MAX];
    int deadUnitCount[BWAPI::UnitTypes::Enum::MAX];
    int killedUnitCount[BWAPI::UnitTypes::Enum::MAX];

    int  upgradeLevel[BWAPI::UpgradeTypes::Enum::MAX];
    bool hasResearched[BWAPI::TechTypes::Enum::MAX];
    bool isResearching[BWAPI::TechTypes::Enum::MAX];
    bool isUpgrading[BWAPI::UpgradeTypes::Enum::MAX];

    int color;

    int totalUnitScore;
    int totalKillScore;
    int totalBuildingScore;
    int totalRazingScore;
    int customScore;

    int   maxUpgradeLevel[BWAPI::UpgradeTypes::Enum::MAX];
    bool  isResearchAvailable[BWAPI::TechTypes::Enum::MAX];
    bool  isUnitAvailable[BWAPI::UnitTypes::Enum::MAX];
  };
}

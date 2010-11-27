#pragma once

namespace BWAPI
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

    int minerals;
    int gas;
    int cumulativeMinerals;
    int cumulativeGas;
    int supplyTotal[3];
    int supplyUsed[3];

    int allUnitCount[BWAPI_UNIT_TYPE_MAX_COUNT];
    int visibleUnitCount[BWAPI_UNIT_TYPE_MAX_COUNT];
    int completedUnitCount[BWAPI_UNIT_TYPE_MAX_COUNT];
    int deadUnitCount[BWAPI_UNIT_TYPE_MAX_COUNT];
    int killedUnitCount[BWAPI_UNIT_TYPE_MAX_COUNT];

    int  upgradeLevel[63];
    bool hasResearched[47];
    bool isResearching[47];
    bool isUpgrading[63];

    int colorByte;
    int color;

    int totalUnitScore;
    int totalKillScore;
    int totalBuildingScore;
    int totalRazingScore;
    int customScore;
  };
}

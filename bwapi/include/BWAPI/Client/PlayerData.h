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

    int allUnitCount[230];
    int completedUnitCount[230];
    int deadUnitCount[230];
    int killedUnitCount[230];

    int  upgradeLevel[63];
    bool hasResearched[47];
    bool isResearching[47];
    bool isUpgrading[63];


  };
}

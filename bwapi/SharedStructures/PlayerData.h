#pragma once

namespace BWAPIC
{
  struct PlayerData
  {
    char name[25];  // size as BW memory
    int race;
    int type;
    int force;
    bool isAlly[12];       // index correlated with players
    bool isEnemy[12];       // index correlated with players
    bool isNeutral;
    int startLocationX;
    int startLocationY;
    bool isVictorious;
    bool isDefeated;
    bool leftGame;

    int minerals;
    int gas;
    int cumulativeMinerals;
    int cumulativeGas;
    int supplyTotal[3];
    int supplyUsed[3];

    int allUnitCount[228];
    int completedUnitCount[228];
    int deadUnitCount[228];
    int killedUnitCount[228];

    int upgradeLevel[66];
    bool hasResearched[44];
    bool isResearching[44];
    bool isUpgrading[66];


  };
}

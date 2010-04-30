#pragma once

namespace BWAPIC
{
  struct PlayerData
  {
    bool exists;
    int force;
    int type;
    int race;
    int minerals;
    int gas;
    int cumulativeMinerals;
    int cumulativeGas;
    int suppliesAvailable[3];
    int suppliesUsed[3];

    int allUnitCount[228];
    int completedUnitCount[228];
    int deadUnitCount[228];
    int killedUnitCount[228];

    int upgradeLevel[66];
    bool isUpgrading[66];
    bool hasResearched[44];
    bool isResearching[44];

    char name[25];  // size as BW memory

    bool alliance[12];       // index correlated with players
    bool isVictorious;
    bool isDefeated;
    int startLocationX;
    int startLocationY;
  };
}

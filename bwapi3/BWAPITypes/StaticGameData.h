#pragma once

#include "UnitCommand.h"
#include "UnitState.h"
#include "Player.h"
#include "Force.h"

#include <Util\LimitedArray.h>

namespace BWAPI
{
  typedef Util::LimitedArray<Player, 12> PlayerArray;
  typedef Util::LimitedArray<Force, 4> ForceArray;

  struct StaticGameData
  {
    int frameCount;
    int getLatency;
    int mouseX;
    int mouseY;
    int screenX;
    int screenY;
    int mapWidth;
    int mapHeight;
    char mapFilename[260];//size based on broodwar memory
    char mapName[32];     //size based on broodwar memory
    char userInput[256];  // size based on experiments. 78 in bw, 119 in mp chat room, 255 in bnet
    int mapHash;
    int getGroundHeight[1024][1024];
    bool isWalkable[1024][1024]; 
    bool isBuildable[256][256];
    bool isVisible[256][256];
    bool isExplored[256][256];
    bool hasCreep[256][256];
    bool isMultiplayer;
    bool isReplay;
    bool isPaused;
    int unitCount;
    PlayerArray players;
    ForceArray forces;
  };
}

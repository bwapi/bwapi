#pragma once

#include <BWAPITypes\Position.h>

namespace Bridge
{
  struct StaticGameDataStructure
  {
    int getLatency;
    int frameCount;
    int getMouseX;
    int getMouseY;
    int getScreenX;
    int getScreenY;
    int mapWidth;
    int mapHeight;
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
  };
}

#pragma once

#include <BWAPITypes\Position.h>

namespace Bridge
{
  struct StaticGameDataStructure
  {
    int getLatency;
    int getFrameCount;
    int getMouseX;
    int getMouseY;
    int getScreenX;
    int getScreenY;
    int mapWidth;
    int mapHeight;
    int getMapHash;
    bool isMultiplayer;
    bool isReplay;
    bool isPaused;
  };
}

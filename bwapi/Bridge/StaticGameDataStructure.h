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
    bool isMultiplayer;
    bool isReplay;
    bool isPaused;
  };
}

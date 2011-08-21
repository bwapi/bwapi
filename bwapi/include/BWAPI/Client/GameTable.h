#pragma once

namespace BWAPI
{
  struct GameInstance
  {
    DWORD serverProcessID;
    bool isConnected;
    time_t lastKeepAliveTime;
  };
  struct GameTable
  {
    GameTable() {}
    static const int MAX_GAME_INSTANCES = 8;
    GameInstance gameInstances[MAX_GAME_INSTANCES];
  };
}
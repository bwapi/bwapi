#pragma once

namespace BWAPI
{
  struct GameInstance
  {
    GameInstance()
     : serverProcessID(0)
     , isConnected(false)
     , lastKeepAliveTime(0)
    {};
    GameInstance(unsigned int servID, bool connected, unsigned int lastAliveTime)
      : serverProcessID(servID)
      , isConnected(connected)
      , lastKeepAliveTime(lastAliveTime)
    {};

    unsigned int serverProcessID;
    bool isConnected;
    //time_t lastKeepAliveTime;
    unsigned int lastKeepAliveTime;
  };
  struct GameTable
  {
    GameTable()
    {}
    static const int MAX_GAME_INSTANCES = 8;
    GameInstance gameInstances[MAX_GAME_INSTANCES];
  };
}
#pragma once

#include <Bridge\StaticGameData.h>

#include <string>
#include <deque>

namespace BWAgent
{
  namespace BridgeClient
  {
  //public:
    extern Bridge::StaticGameData* sharedStaticData;

    // methods
    extern std::string getLastError();
    extern bool connect();
    extern void disconnect();
    extern bool waitForEvent();

    extern std::deque<std::string> getUserInputStrings();

    // state
    enum RpcState : int
    {
      Intermediate,
      OnInitMatch,
      OnFrame,
    };
    extern RpcState getCurrentRpc();
  }
}

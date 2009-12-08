#pragma once

#include <Bridge\StaticGameData.h>

#include <string>
#include <deque>

namespace BWAPI
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

    extern bool isConnected();
    extern bool pushSendText(bool send, char *string);

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

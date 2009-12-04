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
    extern int connect();
    extern bool waitForEvent();

    extern std::deque<std::string> getUserInputStrings();

    // state
    enum BridgeState : int
    {
      Intermediate,
      OnInitMatch,
      OnFrame,
    };
    extern BridgeState getCurrentState();
  }
}

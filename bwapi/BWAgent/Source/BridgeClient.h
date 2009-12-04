#pragma once

#include <Bridge\StaticGameDataStructure.h>

#include <string>
#include <deque>

namespace BWAgent
{
  namespace BridgeClient
  {
  //public:
    extern Bridge::StaticGameDataStructure* sharedStaticData;

    extern Bridge::CommandDataStructure* sharedCommandData;

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

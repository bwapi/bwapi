#pragma once

#include <Bridge\StaticGameDataStructure.h>

#include <string>

namespace BWAgent
{
  namespace BridgeClient
  {
  //public:
    extern Bridge::StaticGameDataStructure* sharedStaticData;

    // methods
    extern std::string getLastError();
    extern int connect();
    extern bool waitForEvent();

    // state
    enum BridgeState : int
    {
      Intermediate,
      OnInitMatch,
      OnFrame,
      OnSendText
    };
    extern BridgeState getCurrentState();
  }
}

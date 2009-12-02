#pragma once

#include <Bridge\StaticGameDataStructure.h>

#include <string>

namespace BWAgent
{
  namespace BridgeAgent
  {
  //public:
    extern Bridge::StaticGameDataStructure* sharedStaticData;

    extern int connect();
    extern std::string getLastError();
  }
}

#pragma once

#include <Bridge\StaticGameDataStructure.h>

#include <string>

namespace BWAgent
{
  namespace BridgeClient
  {
  //public:
    extern Bridge::StaticGameDataStructure* sharedStaticData;

    extern int connect();
    extern std::string getLastError();
  }
}

#pragma once

#include <string>

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //public:
    extern bool initConnectionServer();
    extern bool acceptIncomingConnections();
    extern const std::string &getLastError();
    extern bool isAgentConnected();
  }
}

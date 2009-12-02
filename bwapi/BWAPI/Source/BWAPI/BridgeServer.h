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
    extern bool initMatch();
    extern bool invokeOnFrame();
    extern bool isAgentConnected();
    extern const std::string &getLastError();
  }
}

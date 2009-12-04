#pragma once

#include <Bridge\StaticGameDataStructure.h>

#include <string>

namespace Bridge {}

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //public:
    extern Bridge::StaticGameDataStructure* sharedStaticData;

    extern bool initConnectionServer();
    extern bool acceptIncomingConnections();
    extern void disconnect();
    extern bool createSharedMemory();
    extern bool exportSharedMemory();
    extern bool releaseSharedMemory();
    extern bool invokeOnFrame();
    extern bool invokeOnStartMatch(bool fromBeginning);
    extern bool pushSendText();
    extern bool isAgentConnected();               // true when pipe connection works
    extern bool isBridgeInitialized();            // true once initBridge called
    extern bool isSharedMemoryInitialized();      // true once memory allocated
    extern const std::string &getLastError();
  }
}

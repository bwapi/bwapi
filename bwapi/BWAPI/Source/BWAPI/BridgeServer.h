#pragma once

#include <Bridge\StaticGameData.h>
#include <Bridge\SendTextEntry.h>

#include <string>
#include <deque>

namespace Bridge {}

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //public:
    extern Bridge::StaticGameData* sharedStaticData;

    extern bool initConnectionServer();
    extern bool acceptIncomingConnections();
    extern void disconnect();
    extern bool createSharedMemory();
    extern bool exportSharedMemory();
    extern bool releaseSharedMemory();
    extern bool updateRemoteSharedMemory();
    extern bool invokeOnFrame();
    extern bool invokeOnStartMatch(bool fromBeginning);
    extern bool pushSendText(const char *text);
    extern bool isAgentConnected();               // true when pipe connection works
    extern bool isBridgeInitialized();            // true once initBridge called
    extern bool isSharedMemoryInitialized();      // true once memory allocated
    extern const std::string &getLastError();

    extern std::deque<Bridge::SendTextEntry*> getSendTextEntries();
  }
}

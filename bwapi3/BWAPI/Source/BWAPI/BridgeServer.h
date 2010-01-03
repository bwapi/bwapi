#pragma once

#include <Bridge\SharedStuff.h>
#include <Bridge\DrawShape.h>
#include <Bridge\CommandEntry.h>

#include <BWAPITypes\StaticGameData.h>
#include <BWAPITypes\UnitEvent.h>

#include <Util\MemoryFrame.h>

#include <string>
#include <deque>

namespace Bridge {}

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //public:
    extern Bridge::SharedStuff sharedStuff;
    extern StaticGameData* gameData;

    extern std::deque<Bridge::CommandEntry::SendText*> sendTextEntries;
    extern std::deque<Bridge::CommandEntry::UnitOrder*> orderEntries;

    extern bool initConnectionServer();
    extern void acceptIncomingConnections();
    extern void disconnect();
    extern bool createSharedMemory();
    extern bool exportSharedMemory();
    extern bool releaseSharedMemory();
    extern bool updateRemoteSharedMemory();
    extern void invokeOnFrame();
    extern void invokeOnStartMatch(bool fromBeginning);

    extern bool pushSendText(const char *text);

    // returns the index of the new unit
    extern int addKnownUnit(KnownUnit **out_pKnownUnit, UnitAddEventTypeId reason);
    extern void removeKnownUnit(int index, UnitRemoveEventTypeId reason);

    extern bool isAgentConnected();               // true when pipe connection works
    extern bool isBridgeInitialized();            // true once initBridge called
    extern bool isSharedMemoryInitialized();      // true once memory allocated
    extern const std::string &getLastError();

    typedef void (*DrawShapeCallback)(Util::MemoryFrame packet);
    extern void enumAllDrawShapes(DrawShapeCallback);
  }
}

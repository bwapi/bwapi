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

    extern void initConnectionServer();
    extern void acceptIncomingConnections();
    extern void disconnect();
    extern bool exportSharedMemory();
    extern bool releaseSharedMemory();
    extern void updateRemoteSharedMemory();
    extern void invokeOnFrame();
    extern void invokeOnStartMatch(bool fromBeginning);

    // returns the index of the new unit
    extern int addKnownUnit(KnownUnit **out_pKnownUnit, UnitAddEventTypeId reason);
    extern void removeKnownUnit(int index, UnitRemoveEventTypeId reason);

    extern bool isAgentConnected();               // true when pipe connection works
    extern bool isBridgeInitialized();            // true once initBridge called

    typedef void (*DrawShapeCallback)(Util::MemoryFrame packet);
    extern void enumAllDrawShapes(DrawShapeCallback);
  }
}

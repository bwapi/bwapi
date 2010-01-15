#pragma once

#include <Bridge\SharedStuff.h>
#include <BWAPITypes\StaticGameData.h>
#include <BWAPITypes\DynamicGameData.h>
#include <BWAPITypes\UnitEvent.h>
#include <BWAPITypes\CoordinateBase.h>

#include <Util\MemoryFrame.h>

#include <string>
#include <deque>
#include <vector>

namespace BWAPI
{
  namespace BridgeClient
  {
  //public:
    extern BWAPI::StaticGameData* gameData;
    extern BWAPI::DynamicGameData dynamicData;
    extern Bridge::SharedStuff sharedStuff;

    // additonal RPC state data
    extern bool isMatchStartFromBeginning;

    // methods
    extern std::string getLastError();
    extern bool connect();
    extern void disconnect();
    extern void waitForEvent();

    extern bool isConnected();
    extern bool pushSendText(bool send, const char *string);

    // draw stack
    extern bool pushDrawText(CoordinateBase base, Position pos, const char*);
    extern bool pushDrawRectangle(CoordinateBase base, Position pos, Position size, int color, bool solid);
    extern bool pushDrawCircle(CoordinateBase base, Position center, int r, int color, bool solid);
    extern bool pushDrawEllipse(CoordinateBase base, Position center, int w, int h, int color, bool solid);
    extern bool pushDrawLine(CoordinateBase base, Position pos, Position pos2, int color);
    extern bool pushDrawDot(CoordinateBase base, Position pos, int color);
    extern bool pushDrawTriangle(CoordinateBase base, Position posa, Position posb, Position posc, int color, bool solid);

    // state
    enum RpcState : int
    {
      Intermediate,
      OnFrame,
    };
    extern RpcState getCurrentRpc();
  }
}

#pragma once

#include <Bridge\SharedStuff.h>
#include <BWAPITypes\StaticGameData.h>
#include <BWAPITypes\DynamicGameData.h>
#include <BWAPITypes\UnitEvent.h>

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
    extern bool pushDrawText(Position pos, const char*);
    extern bool pushDrawRectangle(Position pos, Position size, int color, bool solid);
    extern bool pushDrawCircle(Position center, int r, int color, bool solid);
    extern bool pushDrawEllipse(Position center, int w, int h, int color, bool solid);
    extern bool pushDrawLine(Position pos, Position pos2, int color);
    extern bool pushDrawDot(Position pos, int color);
    extern bool pushDrawTriangle(Position posa, Position posb, Position posc, int color, bool solid);

    // state
    enum RpcState : int
    {
      Intermediate,
      OnInitMatch,
      OnFrame,
    };
    extern RpcState getCurrentRpc();
  }
}

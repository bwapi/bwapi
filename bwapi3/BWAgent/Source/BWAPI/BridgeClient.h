#pragma once

#include <Bridge\SharedStuff.h>
#include <BWAPITypes\StaticGameData.h>
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
    extern Bridge::SharedStuff sharedStuff;

    // events
    extern std::vector<const UnitAddEvent*> knownUnitAddEvents;
    extern std::vector<const UnitRemoveEvent*> knownUnitRemoveEvents;

    // additonal RPC state data
    extern bool isMatchStartFromBeginning;

    // methods
    extern std::string getLastError();
    extern bool connect();
    extern void disconnect();
    extern void waitForEvent();

    extern std::deque<std::string> getUserInputStrings();

    extern bool isConnected();
    extern bool pushSendText(bool send, const char *string);

    // draw stack
    extern bool pushDrawText(int x, int y, const char*);
    extern bool pushDrawRectangle(int x, int y, int w, int h, int color, bool solid);
    extern bool pushDrawCircle(int x, int y, int r, int color, bool solid);
    extern bool pushDrawLine(int x, int y, int x2, int y2, int color);
    extern bool pushDrawDot(int x, int y, int color);

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

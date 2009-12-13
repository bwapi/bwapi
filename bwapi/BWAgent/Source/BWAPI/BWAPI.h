#pragma once
#include "BWAPIInterface.h"

#include <BWAPITypes\UnitState.h>
#include <BWAPITypes\StaticGameData.h>

#include <windows.h>
#include <string>

// return BWAgent.dll revision number
BWAPI_FUNCTION int BWAPI_CALL BWGetVersion();

// returns zero if failed
BWAPI_FUNCTION bool BWAPI_CALL BWConnect();

// TODO: remove BWTakeover. Instead provide BWWaitForRpc()
// blocks. returns only if Disconnect() called from a callback
// returns false immediately or during run if error occurred
typedef void (BWAPI_CALL *BWMatchFrameCallback)();
typedef void (BWAPI_CALL *BWMatchStartCallback)(bool fromBeginning);
typedef void (BWAPI_CALL *BWUnitAddCallback)(BWAPI::UnitState*);
typedef void (BWAPI_CALL *BWUnitRemoveCallback)(BWAPI::UnitState*);
typedef void (BWAPI_CALL *BWSendTextCallback)(const char*);
BWAPI_FUNCTION bool BWAPI_CALL BWTakeover(
  BWMatchFrameCallback,
  BWMatchStartCallback,
  BWUnitAddCallback,
  BWUnitRemoveCallback,
  BWSendTextCallback);

// static data
BWAPI_FUNCTION BWAPI::StaticGameData* BWAPI_CALL BWGetStaticGameData();
BWAPI_FUNCTION void                   BWAPI_CALL BWPositionMapToScreen(BWAPI::Position* pos);

// draw operations
BWAPI_FUNCTION void BWAPI_CALL BWDrawText(int x, int y, const char* text);
BWAPI_FUNCTION void BWAPI_CALL BWDrawRectangle(int x, int y, int w, int h, int color, int solid);
BWAPI_FUNCTION void BWAPI_CALL BWDrawCircle(int x, int y, int r, int color, int solid);
BWAPI_FUNCTION void BWAPI_CALL BWDrawLine(int x, int y, int x2, int y2, int color);
BWAPI_FUNCTION void BWAPI_CALL BWDrawDot(int x, int y, int color);

// unit array enumeration
BWAPI_FUNCTION HANDLE             BWAPI_CALL BWAllUnitsBegin();
BWAPI_FUNCTION BWAPI::UnitState*  BWAPI_CALL BWAllUnitsNext(HANDLE h);
BWAPI_FUNCTION void               BWAPI_CALL BWAllUnitsClose(HANDLE h);

// returns last operation's error string.
BWAPI_FUNCTION const char* BWAPI_CALL BWGetLastError();

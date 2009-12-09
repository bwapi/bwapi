#pragma once
#include "BWAPIInterface.h"

#include <BWAPITypes\UnitState.h>

#include <windows.h>
#include <string>

// return BWAgent.dll revision number
BWAPI_FUNCTION int BWAPI_CALL BWGetVersion();

// returns zero if failed
BWAPI_FUNCTION bool BWAPI_CALL BWConnect();

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

// draw operations
BWAPI_FUNCTION void BWAPI_CALL BWDrawText(int x, int y, const char* text);
BWAPI_FUNCTION void BWAPI_CALL BWDrawRect(int x, int y, int w, int h, int color);

// returns last operation's error string.
BWAPI_FUNCTION const char* BWAPI_CALL BWGetLastError();

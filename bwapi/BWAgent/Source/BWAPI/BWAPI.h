#pragma once
#include "BWAPIInterface.h"

#include <windows.h>
#include <string>

namespace BWAPI
{

  // return BWAgent.dll revision number
  BWAPI_FUNCTION int BWAPI_CALL GetVersion();

  // returns zero if failed
  BWAPI_FUNCTION bool BWAPI_CALL Connect();

  // blocks. returns only if Disconnect() called from a callback
  // returns false immediately or during run if error occurred
  typedef void (__stdcall *BWAPI_MatchFrameCallback)();
  typedef void (__stdcall *BWAPI_MatchStartCallback)();
  typedef void (__stdcall *BWAPI_UnitAddCallback)();
  typedef void (__stdcall *BWAPI_UnitRemoveCallback)();
  BWAPI_FUNCTION bool BWAPI_CALL BW_Takeover(
    BWAPI_MatchFrameCallback,
    BWAPI_MatchStartCallback,
    BWAPI_UnitAddCallback,
    BWAPI_UnitRemoveCallback);

  // returns last operation's error string.
  BWAPI_FUNCTION const char* BWAPI_CALL GetErrorString();
}

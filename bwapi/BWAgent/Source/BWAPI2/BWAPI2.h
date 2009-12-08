#pragma once

#include "BWAPI2Interface.h"

#include <string>

namespace BWAPI2
{
  class AIModule;

  // return BWAgent.dll revision number
  BWAPI2_FUNCTION int BWAPI2_CALL getVersion();

  // returns zero if failed
  BWAPI2_FUNCTION int BWAPI2_CALL connect();

  // blocks. returns only if decideToDisconnect() called from a callback
  // returns false immediately or during run if error occurred
  BWAPI2_FUNCTION bool BWAPI2_CALL takeover(AIModule&);

  // returns last operation's error string.
  BWAPI2_FUNCTION const char* BWAPI2_CALL getLastError();
}

#pragma once

#include "BWAPI2Interface.h"

#include "Error.h"

#include <string>

namespace BWAPI2
{
  class AIModule;

  // return BWAgent.dll revision number
  BWAPI2_EXPORT int getVersion();

  // returns zero if failed
  BWAPI2_EXPORT bool connect();

  // blocks. returns only if decideToDisconnect() called from a callback
  // returns false immediately or during run if error occurred
  BWAPI2_EXPORT bool takeover(AIModule&);

  // returns last operation's error string.
  BWAPI2_EXPORT Error getLastError();
}

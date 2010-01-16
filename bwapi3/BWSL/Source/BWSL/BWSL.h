#pragma once

#include "BWSLInterface.h"

#include "Error.h"

#include <string>

namespace BWSL
{
  class AIModule;

  // return BWAgent.dll revision number
  BWSL_EXPORT int getVersion();

  // returns zero if failed
  BWSL_EXPORT bool connect();

  // blocks. returns only if decideToDisconnect() called from a callback
  // returns false immediately or during run if error occurred
  BWSL_EXPORT bool takeover(AIModule&);

  // returns last operation's error string.
  BWSL_EXPORT Error getLastError();
}

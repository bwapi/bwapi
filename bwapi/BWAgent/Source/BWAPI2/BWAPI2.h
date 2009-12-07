#pragma once
#include <BWAgent\BWAgentInterface.h>

#include <string>

namespace BWAPI2
{
  class AIModule;

  // return BWAgent.dll revision number
  AGENT_API int getVersion();

  // returns zero if failed
  AGENT_API int connect();

  // blocks. returns only if decideToDisconnect() called from a callback
  // returns false immediately or during run if error occurred
  AGENT_API bool takeover(AIModule&);

  // returns last operation's error string.
  AGENT_API std::string getLastError();
}

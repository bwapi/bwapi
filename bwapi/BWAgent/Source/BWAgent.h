#pragma once
#include "BWAgentInterface.h"

#include <string>

namespace BWAgent
{
  class AIModule;

  // return BWAgent.dll revision number
  AGENT_API int getVersion();

  // returns zero if failed
  AGENT_API int connect(AIModule*);

  // returns last operation's error string.
  AGENT_API std::string getLastError();
}

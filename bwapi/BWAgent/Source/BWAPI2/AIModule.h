#pragma once
#include <string>
//#include "Position.h"
#include <BWAgent\BWAgentInterface.h>

AGENT_STRIP;

namespace BWAPI2
{
  class Unit;
  class Player;
  class AIModule
  {
    public:
      AIModule();
      AGENT_INTERFACE ~AIModule();
      AGENT_INTERFACE void onStartMatch();
      AGENT_INTERFACE void onFrame();
      AGENT_INTERFACE void onSendText(std::string text);
  };
}

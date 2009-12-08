#pragma once

#include "BWAPI2Interface.h"

#include <string>

IP_STRIP;

namespace BWAPI2
{
  class Unit;
  class Player;
  class AIModule
  {
    public:
      AIModule();
      BWAPI2_METHOD ~AIModule();
      BWAPI2_METHOD void onStartMatch();
      BWAPI2_METHOD void onFrame();
      BWAPI2_METHOD void onSendText(std::string text);
  };
}

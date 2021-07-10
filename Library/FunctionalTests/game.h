#pragma once

#include <BWAPI/Game.h>
#include "../Network/BWAPIFrontendClient/ProtoClient.h"

#include <memory>

namespace Funtest
{
  class FunGame
  {
  public:
    FunGame();

    /// Initializes Game and waits for the game to begin.
    void connect();

    /// Does a send/receive without regard for frame count.
    void update();

    /// Waits the specified number of frames. (default 1)
    void advance(int frames = 1);

    /// Remove all units and reset any state.
    void reset();
    
    BWAPI::Game* operator ->();

  private:
    BWAPI::ProtoClient client;
    std::unique_ptr<BWAPI::Game> game;
  };

  extern FunGame funGame;
}

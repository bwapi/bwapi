#pragma once
#include <chrono>

namespace BWAPI
{
  // A counter for frames per second
  class FPSCounter
  {
  public:
    // Initializes all variables to defaults. Used when initializing game data.
    void init();

    // Updates the counter
    void update();

    // Used by Broodwar to get the integral fps
    int getFps() const;

    // Used by Broodwar to get the average fps
    double getAverageFps() const;

  private:
    using clock = std::chrono::steady_clock;
    using milliseconds = std::chrono::milliseconds;

    int     lastUpdateFrame = 0;
    long double fps = 0;
    long double averageFPS = 0;

    clock::time_point lastUpdateTime;
    clock::time_point initTime;
  };
}

#pragma once
#include <chrono>

namespace BWAPI
{
  // A counter for frames per second
  class FPSCounter
  {
  public:
    // Initializes all variables to defaults. Used when initializing game data.
    void init(int frame = 0);

    // Updates the counter
    void update(int frame);

    // Used by Broodwar to get the integral fps
    int getFps() const;

    // Used by Broodwar to get the average fps
    double getAverageFps() const;

  private:
    using clock = std::chrono::steady_clock;

    int lastUpdateFrame = 0;
    long double fps = 0;
    long double averageFPS = 0;

    clock::time_point lastUpdateTime;
    clock::time_point initTime;
  };
}

#include <BWAPI/FPSCounter.h>

#include <chrono>
#include <cmath>

using namespace BWAPI;
using namespace std::literals::chrono_literals;

void FPSCounter::init(int frame)
{
  initTime = lastUpdateTime = clock::now();
  lastUpdateFrame = frame;
  fps = 0;
  averageFPS = 0.0;
}

void FPSCounter::update(int frame)
{
  clock::time_point now = clock::now();
  auto elapsed = (now - lastUpdateTime);
  if (elapsed >= 1s || fps == 0.0)
  {
    auto frameDifference = frame - lastUpdateFrame;
    auto timeElapsedInSeconds = (long double)elapsed.count() / std::chrono::duration_cast<decltype(elapsed)>(1s).count();

    // number of frames over seconds
    fps = frameDifference / timeElapsedInSeconds;
    averageFPS = averageFPS*0.7 + fps*0.3;

    lastUpdateFrame = frame;
    lastUpdateTime = now;
  }
}

int FPSCounter::getFps() const
{
  return static_cast<int>(std::round(fps));
}

double FPSCounter::getAverageFps() const
{
  return static_cast<double>(averageFPS);
}

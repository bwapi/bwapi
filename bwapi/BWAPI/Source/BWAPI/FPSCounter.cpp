#include "FPSCounter.h"

#include <BWAPI/Game.h>

using namespace BWAPI;

void FPSCounter::init()
{
  initTime = lastUpdateTime = clock::now();
  lastUpdateFrame = Broodwar->getFrameCount();
  fps = 0;
  averageFPS = 0;
}

void FPSCounter::update()
{
  clock::time_point now = clock::now();
  int currentFrame = Broodwar->getFrameCount();
  auto elapsed = std::chrono::duration_cast<milliseconds>(now - lastUpdateTime).count();
  if (elapsed >= 1000)
  {
    // number of frames over seconds
    fps = (currentFrame - lastUpdateFrame) / (elapsed / 1000.0L);
    averageFPS = averageFPS*0.7 + fps*0.3;

    lastUpdateFrame = currentFrame;
    lastUpdateTime = now;
  }
}

int FPSCounter::getFps() const
{
  return static_cast<int>(fps);
}

double FPSCounter::getAverageFps() const
{
  return static_cast<double>(averageFPS);
}

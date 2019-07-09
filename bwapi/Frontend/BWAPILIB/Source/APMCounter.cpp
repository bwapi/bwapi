#include <BWAPI/APMCounter.h>

#include <cmath>

using namespace BWAPI;

void APMCounter::init()
{
  botAPM_noselects = botAPM_selects = 0;
  botAPMCounter_noselects = botAPMCounter_selects = 0.0;
}

int APMCounter::apm(bool selects) const
{
  return selects ? botAPM_selects : botAPM_noselects;
}

void APMCounter::update(int frame)
{
  // Note: formula from APMAlert
  const long double APMInterval = 0.95L;    // time after which actions are worth

  // Get the time difference between frames on fastest game speed (milliseconds).
  // That's numFrames * 42ms / frame .
  int timeDifference = (frame - lastUpdateFrame) * 42;

  int totalTime = frame * 42;

  // decay
  botAPMCounter_selects = botAPMCounter_selects * std::exp(-timeDifference / (APMInterval * 60000));
  botAPMCounter_noselects = botAPMCounter_noselects * std::exp(-timeDifference / (APMInterval * 60000));

  long double gameDurationFactor = 1.0L - std::exp(-totalTime / (APMInterval * 60000));
  if (gameDurationFactor < 1e-100L) gameDurationFactor = 1e-100L; //Prevent division by 0

  botAPM_selects = static_cast<int>(botAPMCounter_selects / (APMInterval * gameDurationFactor));
  botAPM_noselects = static_cast<int>(botAPMCounter_noselects / (APMInterval * gameDurationFactor));

  lastUpdateFrame = frame;
}

void APMCounter::addSelect()
{
  botAPMCounter_selects += 1.0L;
}

void APMCounter::addNoSelect()
{
  botAPMCounter_noselects += 1.0L;
  addSelect();
}

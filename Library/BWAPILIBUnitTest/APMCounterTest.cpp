#include <gtest/gtest.h>

#include <BWAPI/APMCounter.h>

using namespace BWAPI;

TEST(APMCounter, ctorSetsDefaults)
{
  APMCounter apm;

  EXPECT_EQ(apm.apm(true), 0);
  EXPECT_EQ(apm.apm(false), 0);
}

TEST(APMCounter, reflects204APM)
{
  APMCounter apm;

  // Send a command every 7th frame for 50 frames.
  for (int frame = 1; frame < 50; ++frame)
  {
    if (frame % 7 == 0) apm.addSelect();
    apm.update(frame);
  }

  // If you send a command every 7th frame for a full minute on fastest speed:
  // (60000ms / 42ms/frame = 1428.57.. frames) / 7frames/action = 204.08.. actions in a minute
  EXPECT_EQ(apm.apm(), 204);
}

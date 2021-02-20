#include <gtest/gtest.h>

#include <BWAPI/FPSCounter.h>

using namespace BWAPI;

TEST(FPSCounter, ctorSetsDefaults)
{
  FPSCounter fps;
  EXPECT_EQ(fps.getAverageFps(), 0.0);
  EXPECT_EQ(fps.getFps(), 0);
}

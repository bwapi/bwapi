#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "LatencyAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Set Latency: High"
#define DEFAULT_PLAYER  (1)
#define LATENCY         (1)

ReplayTool::GameAction* CreateLatencyAction () { return new LatencyAction(DEFAULT_PLAYER, LATENCY); }

class LatencyActionTest : public AbstractActionTest<CreateLatencyAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, LATENCY);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readBYTE()).
      WillOnce(testing::Return(LATENCY));

    action->read(reader);

    EXPECT_EQ(LATENCY, ((LatencyAction*)action)->getLatency());
  }
};

TEST_F(LatencyActionTest, LatencyAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Set_Latency, action->getAction());
}

TEST_F(LatencyActionTest, read)
{
  testRead();
}

TEST_F(LatencyActionTest, toString)
{
  testToString();
}
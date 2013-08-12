#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "SetReplaySpeedAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Set Replay Speed: Faster, ×%u"
#define DEFAULT_PLAYER  (1)
#define PAUSED          (false)
#define SPEED      (5)
#define  MULTIPLIER    (3)

ReplayTool::GameAction* CreateSetReplaySpeedAction () { return new SetReplaySpeedAction(DEFAULT_PLAYER, PAUSED, SPEED, MULTIPLIER); }

class SetReplaySpeedActionTest : public AbstractActionTest<CreateSetReplaySpeedAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, MULTIPLIER);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readBYTE()).
      WillOnce(testing::Return(PAUSED));
  EXPECT_CALL(reader, readDWORD()).
      WillOnce(testing::Return(SPEED)).
      WillOnce(testing::Return(MULTIPLIER));

    action->read(reader);

    EXPECT_EQ(PAUSED, ((SetReplaySpeedAction*)action)->getPaused());
    EXPECT_EQ(SPEED, ((SetReplaySpeedAction*)action)->getSpeed());
    EXPECT_EQ(MULTIPLIER, ((SetReplaySpeedAction*)action)->getMultiplier());
  }
};

TEST_F(SetReplaySpeedActionTest, SetReplaySpeedAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Set_Replay_Speed, action->getAction());
}

TEST_F(SetReplaySpeedActionTest, read)
{
  testRead();
}

TEST_F(SetReplaySpeedActionTest, toString)
{
  testToString();
}
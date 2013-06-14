#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "SetSpeedAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) [Lobby]Set Game Seed: Slow"
#define DEFAULT_PLAYER  (1)
#define SPEED           (2)

ReplayTool::GameAction* CreateSetSpeedAction
  
  () { return new SetSpeedAction(DEFAULT_PLAYER, SPEED); }

class SetSpeedActionTest : public AbstractActionTest<CreateSetSpeedAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, SPEED);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readBYTE()).
      WillOnce(testing::Return(SPEED));

    action->read(reader);

    EXPECT_EQ(SPEED, ((SetSpeedAction*)action)->getSpeed());
  }
};

TEST_F(SetSpeedActionTest, SetSpeedAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Set_Seed, action->getAction());
}

TEST_F(SetSpeedActionTest, read)
{
  testRead();
}

TEST_F(SetSpeedActionTest, toString)
{
  testToString();
}
#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "LeaveGameAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Leave Game: Defeat"
#define DEFAULT_PLAYER  (1)
#define LEAVE_TYPE      (1)

ReplayTool::GameAction* CreateLeaveGameAction () { return new LeaveGameAction(DEFAULT_PLAYER, LEAVE_TYPE); }

class LeaveGameActionTest : public AbstractActionTest<CreateLeaveGameAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, LEAVE_TYPE);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readBYTE()).
      WillOnce(testing::Return(LEAVE_TYPE));

    action->read(reader);

    EXPECT_EQ(LEAVE_TYPE, ((LeaveGameAction*)action)->getLeaveType());
  }
};

TEST_F(LeaveGameActionTest, LeaveGameAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Leave_Game, action->getAction());
}

TEST_F(LeaveGameActionTest, read)
{
  testRead();
}

TEST_F(LeaveGameActionTest, toString)
{
  testToString();
}
#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "StopAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Stop: Queued"
#define DEFAULT_PLAYER  (1)
#define HOW             (1)

ReplayTool::GameAction* CreateStopAction
  
  () { return new StopAction(DEFAULT_PLAYER, HOW, true); }

class StopActionTest : public AbstractActionTest<CreateStopAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, HOW, true);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readBYTE()).
      WillOnce(testing::Return(HOW));

    action->read(reader);

    EXPECT_EQ(HOW, ((StopAction*)action)->getHow());
  }
};

TEST_F(StopActionTest, StopAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Stop, action->getAction());
}

TEST_F(StopActionTest, read)
{
  testRead();
}

TEST_F(StopActionTest, toString)
{
  testToString();
}
#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "UnloadAllAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Unload All: Queued"
#define DEFAULT_PLAYER  (1)
#define HOW             (1)

ReplayTool::GameAction* CreateUnloadAllAction
  
  () { return new UnloadAllAction(DEFAULT_PLAYER, HOW, true); }

class UnloadAllActionTest : public AbstractActionTest<CreateUnloadAllAction>
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

    EXPECT_EQ(HOW, ((UnloadAllAction*)action)->getHow());
  }
};

TEST_F(UnloadAllActionTest, UnloadAllAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Unload_All, action->getAction());
}

TEST_F(UnloadAllActionTest, read)
{
  testRead();
}

TEST_F(UnloadAllActionTest, toString)
{
  testToString();
}
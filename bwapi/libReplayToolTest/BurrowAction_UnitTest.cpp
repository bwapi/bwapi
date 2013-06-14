#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "BurrowAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Unburrow: Queued"
#define DEFAULT_PLAYER  (1)
#define HOW             (1)

ReplayTool::GameAction* CreateBurrowAction
  
  () { return new BurrowAction(DEFAULT_PLAYER, HOW, true); }

class BurrowActionTest : public AbstractActionTest<CreateBurrowAction>
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

    EXPECT_EQ(HOW, ((BurrowAction*)action)->getHow());
  }
};

TEST_F(BurrowActionTest, BurrowAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Burrow_Up, action->getAction());
}

TEST_F(BurrowActionTest, read)
{
  testRead();
}

TEST_F(BurrowActionTest, toString)
{
  testToString();
}
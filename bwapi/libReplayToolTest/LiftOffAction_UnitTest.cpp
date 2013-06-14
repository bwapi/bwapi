#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "LiftOffAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Lift Off: (%d, %d)"
#define DEFAULT_PLAYER  (1)
#define X_POS            (123)
#define Y_POS            (321)

ReplayTool::GameAction* CreateLiftOffAction
  
  () { return new LiftOffAction(DEFAULT_PLAYER, X_POS, Y_POS); }

class LiftOffActionTest : public AbstractActionTest<CreateLiftOffAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, X_POS, Y_POS);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readWORD()).
      WillOnce(testing::Return(X_POS)).
      WillOnce(testing::Return(Y_POS));

    action->read(reader);

    EXPECT_EQ(X_POS, ((LiftOffAction*)action)->getX());
    EXPECT_EQ(Y_POS, ((LiftOffAction*)action)->getY());
  }
};

TEST_F(LiftOffActionTest, LiftOffAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Lift_Off, action->getAction());
}

TEST_F(LiftOffActionTest, read)
{
  testRead();
}

TEST_F(LiftOffActionTest, toString)
{
  testToString();
}
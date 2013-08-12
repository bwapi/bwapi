#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "PingMinimapAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Minimap Ping: (%d, %d)"
#define DEFAULT_PLAYER  (1)
#define X_POS           (123)
#define Y_POS           (321)

ReplayTool::GameAction* CreatePingMinimapAction
  
  () { return new PingMinimapAction(DEFAULT_PLAYER, X_POS, Y_POS); }

class PingMinimapActionTest : public AbstractActionTest<CreatePingMinimapAction>
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

    EXPECT_EQ(X_POS, ((PingMinimapAction*)action)->getX());
    EXPECT_EQ(Y_POS, ((PingMinimapAction*)action)->getY());
  }
};

TEST_F(PingMinimapActionTest, PingMinimapAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Ping_Minimap, action->getAction());
}

TEST_F(PingMinimapActionTest, read)
{
  testRead();
}

TEST_F(PingMinimapActionTest, toString)
{
  testToString();
}
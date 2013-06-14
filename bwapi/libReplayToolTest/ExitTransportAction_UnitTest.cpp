#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "ExitTransportAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)
#define UNIT_ID         0x0004
#define UNIT_DATA       0x3
#define EXIT_PARAM    (UNIT_ID | (UNIT_DATA << 12))

ReplayTool::GameAction* CreateExitTransportAction() { return new ExitTransportAction(DEFAULT_PLAYER, EXIT_PARAM); }

class ExitTransportActionTest : public AbstractActionTest<CreateExitTransportAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Exit Transport: (4, 0x03)";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    ExitTransportAction exitAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readWORD()).
      WillOnce(testing::Return(EXIT_PARAM));

    exitAction.read(reader);

    EXPECT_EQ(UNIT_ID, exitAction.getUnitID());
    EXPECT_EQ(UNIT_DATA, exitAction.getData());
  }
};

TEST_F(ExitTransportActionTest, ExitTransportAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Exit_Transport, action->getAction());
}

TEST_F(ExitTransportActionTest, read)
{
  testRead();
}

TEST_F(ExitTransportActionTest, toString)
{
  testToString();
}
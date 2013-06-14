#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "CheatAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)
#define CHEAT_FLAGS     0x80

ReplayTool::GameAction* CreateCheatAction() { return new CheatAction(DEFAULT_PLAYER, CHEAT_FLAGS); }

class CheatActionTest : public AbstractActionTest<CreateCheatAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Cheat: 0x00000080";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    CheatAction cheatAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readDWORD()).
      WillOnce(testing::Return(CHEAT_FLAGS));

    cheatAction.read(reader);

    EXPECT_EQ(CHEAT_FLAGS, cheatAction.getFlags());
  }
};

TEST_F(CheatActionTest, CheatAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Cancel_Train, action->getAction());
}

TEST_F(CheatActionTest, read)
{
  testRead();
}

TEST_F(CheatActionTest, toString)
{
  testToString();
}
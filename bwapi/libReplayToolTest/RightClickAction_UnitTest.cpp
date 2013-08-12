#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "RightClickAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Right Click: (%d, %d), %d:00, %s, Queued"
#define DEFAULT_PLAYER    (1)
#define X_POS        (123)
#define Y_POS        (321)
#define TARGET_ID      (2)
#define UNIT_TYPE_STRING  "Terran_Barracks"
#define UNIT_TYPE      UnitTypes::Terran_Barracks
#define HOW          (3)

ReplayTool::GameAction* CreateRightClickAction() { return new RightClickAction(DEFAULT_PLAYER, X_POS, Y_POS, TARGET_ID, UNIT_TYPE, HOW); }

class RightClickActionTest : public AbstractActionTest<CreateRightClickAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, X_POS, Y_POS, TARGET_ID, UNIT_TYPE_STRING);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readWORD()).
      WillOnce(testing::Return(X_POS)).
      WillOnce(testing::Return(Y_POS)).
      WillOnce(testing::Return(TARGET_ID));

  EXPECT_CALL(reader, readUnitType()).
      WillOnce(testing::Return(UNIT_TYPE));

  EXPECT_CALL(reader, readBYTE()).
      WillOnce(testing::Return(HOW));

    action->read(reader);

    EXPECT_EQ(X_POS, ((RightClickAction*)action)->getX());
    EXPECT_EQ(Y_POS, ((RightClickAction*)action)->getY());
    EXPECT_EQ(TARGET_ID, ((RightClickAction*)action)->getTargetID());
    EXPECT_EQ(UNIT_TYPE, ((RightClickAction*)action)->getUnitType());
    EXPECT_EQ(HOW, ((RightClickAction*)action)->getHow());
  }
};

TEST_F(RightClickActionTest, RightClickAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Right_Click, action->getAction());
}

TEST_F(RightClickActionTest, read)
{
  testRead();
}

TEST_F(RightClickActionTest, toString)
{
  testToString();
}
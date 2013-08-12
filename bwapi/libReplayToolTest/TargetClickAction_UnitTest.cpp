#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "TargetClickAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Target Click: (%d, %d), %d:00, %s, %s, Queued"
#define DEFAULT_PLAYER    (1)
#define X_POS        (123)
#define Y_POS        (321)
#define TARGET_ID      (2)
#define UNIT_TYPE_STRING  "Terran_Barracks"
#define UNIT_TYPE      UnitTypes::Terran_Barracks
#define HOW          (3)
#define ORDER_TYPE_STRING  "InfestedCommandCenter"
#define ORDER_TYPE      Orders::InfestedCommandCenter


ReplayTool::GameAction* CreateTargetClickAction() { return new TargetClickAction(DEFAULT_PLAYER, X_POS, Y_POS, TARGET_ID, UNIT_TYPE, HOW, ORDER_TYPE); }

class TargetClickActionTest : public AbstractActionTest<CreateTargetClickAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, X_POS, Y_POS, TARGET_ID, UNIT_TYPE_STRING, ORDER_TYPE_STRING);
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

  EXPECT_CALL(reader, readOrder()).
      WillOnce(testing::Return(ORDER_TYPE));

    action->read(reader);

    EXPECT_EQ(X_POS, ((TargetClickAction*)action)->getX());
    EXPECT_EQ(Y_POS, ((TargetClickAction*)action)->getY());
    EXPECT_EQ(TARGET_ID, ((TargetClickAction*)action)->getTargetID());
    EXPECT_EQ(UNIT_TYPE, ((TargetClickAction*)action)->getUnitType());
    EXPECT_EQ(HOW, ((TargetClickAction*)action)->getHow());
    EXPECT_EQ(ORDER_TYPE, ((TargetClickAction*)action)->getOrderType());
  }
};

TEST_F(TargetClickActionTest, TargetClickAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Target_Click, action->getAction());
}

TEST_F(TargetClickActionTest, read)
{
  testRead();
}

TEST_F(TargetClickActionTest, toString)
{
  testToString();
}
#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "GroupUnitsAction.h"
#include "AbstractActionTest.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER    (1)
#define GROUP_SAVE_TYPE   0 // Recall
#define GROUP_RECALL_TYPE 1 // Recall
#define GROUP_ADD_TYPE    2
#define GROUP_NUM         3

ReplayTool::GameAction* CreateGroupUnitsAction() { return new GroupUnitsAction(DEFAULT_PLAYER, GROUP_RECALL_TYPE, GROUP_NUM); }

class GroupUnitsActionTest : public AbstractActionTest<CreateGroupUnitsAction>
{
protected:
  void testToString()
  {
    GroupUnitsAction saveGrpAction(DEFAULT_PLAYER, GROUP_SAVE_TYPE, GROUP_NUM);
    string expected = "(P1) Group Units: Save, 3";
    string actual = saveGrpAction.toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());

    GroupUnitsAction rclGrpAction(DEFAULT_PLAYER, GROUP_RECALL_TYPE, GROUP_NUM);
    expected = "(P1) Group Units: Recall, 3";
    actual = rclGrpAction.toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());

    GroupUnitsAction addGrpAction(DEFAULT_PLAYER, GROUP_ADD_TYPE, GROUP_NUM);
    expected = "(P1) Group Units: Add, 3";
    actual = addGrpAction.toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());

    GroupUnitsAction invalidGrpAction(DEFAULT_PLAYER, 4, GROUP_NUM);
    expected = "(P1) Group Units: INVALID, 3";
    actual = invalidGrpAction.toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    GroupUnitsAction grpAction(DEFAULT_PLAYER);

    {
      InSequence seq;

      EXPECT_CALL(reader, readBYTE()).
        WillOnce(testing::Return(GROUP_RECALL_TYPE));
      EXPECT_CALL(reader, readBYTE()).
        WillOnce(testing::Return(GROUP_NUM));
    }
    
    grpAction.read(reader);

    EXPECT_EQ((GroupType)GROUP_RECALL_TYPE, grpAction.getGroupType());
    EXPECT_EQ((TGroupNum)GROUP_NUM, grpAction.getGroupNum());
  }
};

TEST_F(GroupUnitsActionTest, GroupUnitsAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Group_Units, action->getAction());
}

TEST_F(GroupUnitsActionTest, read)
{
  testRead();
}

TEST_F(GroupUnitsActionTest, toString)
{
  testToString();
}
#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "BuildAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Placebox: %s, (%d, %d), %s"
#define DEFAULT_PLAYER  (1)
#define ORDER_TYPE       "PlaceBuilding"
#define X_POS            (123)
#define Y_POS            (321)
#define UNIT_TYPE        "Terran_Barracks"

ReplayTool::GameAction* CreateBuildAction() 
{
  return new BuildAction(DEFAULT_PLAYER, Orders::PlaceBuilding, X_POS, Y_POS, UnitTypes::Terran_Barracks); 
}

class BuildActionTest : public AbstractActionTest<CreateBuildAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, ORDER_TYPE, X_POS, Y_POS, UNIT_TYPE);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    BuildAction bldAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readOrder()).
      WillOnce(testing::Return(Orders::PlaceBuilding));
    EXPECT_CALL(reader, readWORD()).
      WillOnce(testing::Return(X_POS)).
      WillOnce(testing::Return(Y_POS));
    EXPECT_CALL(reader, readUnitType()).
      WillOnce(testing::Return(UnitTypes::Terran_Barracks));

    bldAction.read(reader);

    EXPECT_EQ(Orders::PlaceBuilding, bldAction.getOrderType());
    EXPECT_EQ(X_POS, bldAction.getX());
    EXPECT_EQ(Y_POS, bldAction.getY());
    EXPECT_EQ(UnitTypes::Terran_Barracks, bldAction.getUnitType());
  }
};

TEST_F(BuildActionTest, BuildAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Placebox, action->getAction());
}

TEST_F(BuildActionTest, read)
{
  testRead();
}

TEST_F(BuildActionTest, toString)
{
  testToString();
}
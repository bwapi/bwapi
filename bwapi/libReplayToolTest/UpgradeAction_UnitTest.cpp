#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "UpgradeAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Upgrade: %s"
#define DEFAULT_PLAYER  (1)
#define UPGRADE_NAME    "Carrier_Capacity"

ReplayTool::GameAction* CreateUpgradeAction() { return new UpgradeAction(DEFAULT_PLAYER, UpgradeTypes::Carrier_Capacity); }

class UpgradeActionTest : public AbstractActionTest<CreateUpgradeAction>
{
protected:
  void testToString()
  {
    string expected = StrUtil::format(STR_FORMAT, DEFAULT_PLAYER, UPGRADE_NAME);
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;

    EXPECT_CALL(reader, readUpgradeType()).
      WillOnce(testing::Return(UpgradeTypes::Carrier_Capacity));

    action->read(reader);

    EXPECT_EQ(UpgradeTypes::Carrier_Capacity, ((UpgradeAction*)action)->getUpgrade());
  }
};

TEST_F(UpgradeActionTest, UpgradeAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Upgrade, action->getAction());
}

TEST_F(UpgradeActionTest, read)
{
  testRead();
}

TEST_F(UpgradeActionTest, toString)
{
  testToString();
}
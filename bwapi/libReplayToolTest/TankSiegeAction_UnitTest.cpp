#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "TankSiegeAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Siege: Queued"
#define DEFAULT_PLAYER  (1)
#define HOW             (1)

ReplayTool::GameAction* CreateTankSiegeAction
  
  () { return new TankSiegeAction(DEFAULT_PLAYER, HOW, true); }

class TankSiegeActionTest : public AbstractActionTest<CreateTankSiegeAction>
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

    EXPECT_EQ(HOW, ((TankSiegeAction*)action)->getHow());
  }
};

TEST_F(TankSiegeActionTest, TankSiegeAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Tank_Siege, action->getAction());
}

TEST_F(TankSiegeActionTest, read)
{
  testRead();
}

TEST_F(TankSiegeActionTest, toString)
{
  testToString();
}
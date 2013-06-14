#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "CloakAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Cloak: Queued"
#define DEFAULT_PLAYER  (1)
#define HOW             (1)

ReplayTool::GameAction* CreateCloakAction
  
  () { return new CloakAction(DEFAULT_PLAYER, HOW, true); }

class CloakActionTest : public AbstractActionTest<CreateCloakAction>
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

    EXPECT_EQ(HOW, ((CloakAction*)action)->getHow());
  }
};

TEST_F(CloakActionTest, CloakAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Cloak_On, action->getAction());
}

TEST_F(CloakActionTest, read)
{
  testRead();
}

TEST_F(CloakActionTest, toString)
{
  testToString();
}
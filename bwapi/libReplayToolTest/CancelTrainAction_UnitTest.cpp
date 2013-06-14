#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "CancelTrainAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)
#define UNIT_ID         0x0004
#define UNIT_DATA       0x3
#define CANCEL_PARAM    (UNIT_ID | (UNIT_DATA << 12))

ReplayTool::GameAction* CreateCancelTrainAction() { return new CancelTrainAction(DEFAULT_PLAYER, CANCEL_PARAM); }

class CancelTrainActionTest : public AbstractActionTest<CreateCancelTrainAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Cancel Train: (4, 0x03)";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    CancelTrainAction cancelAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readWORD()).
      WillOnce(testing::Return(CANCEL_PARAM));

    cancelAction.read(reader);

    EXPECT_EQ(UNIT_ID, cancelAction.getUnitID());
    EXPECT_EQ(UNIT_DATA, cancelAction.getData());
  }
};

TEST_F(CancelTrainActionTest, CancelTrainAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Cancel_Train, action->getAction());
}

TEST_F(CancelTrainActionTest, read)
{
  testRead();
}

TEST_F(CancelTrainActionTest, toString)
{
  testToString();
}
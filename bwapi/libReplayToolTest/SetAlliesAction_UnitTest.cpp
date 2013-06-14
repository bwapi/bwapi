#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "SetAlliesAction.h"
#include "AbstractActionTest.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)
#define SET_FLAGS       0x40

ReplayTool::GameAction* CreateSetAlliesAction() { return new SetAlliesAction(DEFAULT_PLAYER, SET_FLAGS); }

class SetAlliesActionTest : public AbstractActionTest<CreateSetAlliesAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Set Allies: 0x00000040";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    SetAlliesAction setAlliesAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readDWORD()).
      WillOnce(testing::Return(SET_FLAGS));

    setAlliesAction.read(reader);

    EXPECT_EQ(SET_FLAGS, setAlliesAction.getData());
  }
};

TEST_F(SetAlliesActionTest, SetAlliesAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Set_Allies, action->getAction());
}

TEST_F(SetAlliesActionTest, read)
{
  testRead();
}

TEST_F(SetAlliesActionTest, toString)
{
  testToString();
}
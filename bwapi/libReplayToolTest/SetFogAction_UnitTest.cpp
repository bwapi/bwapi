#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "SetFogAction.h"
#include "AbstractActionTest.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)
#define SET_FLAGS       0x40

ReplayTool::GameAction* CreateSetFogAction() { return new SetFogAction(DEFAULT_PLAYER, SET_FLAGS); }

class SetFogActionTest : public AbstractActionTest<CreateSetFogAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Set Fog: 0040";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    SetFogAction setFogAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readWORD()).
      WillOnce(testing::Return(SET_FLAGS));

    setFogAction.read(reader);

    EXPECT_EQ(SET_FLAGS, setFogAction.getFlags());
  }
};

TEST_F(SetFogActionTest, SetFogAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Set_Fog, action->getAction());
}

TEST_F(SetFogActionTest, read)
{
  testRead();
}

TEST_F(SetFogActionTest, toString)
{
  testToString();
}
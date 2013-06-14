#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "TrainAction.h"
#include "AbstractActionTest.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)

ReplayTool::GameAction* CreateTrainAction() { return new TrainAction(DEFAULT_PLAYER, UnitTypes::Terran_Marine); }

class TrainActionTest : public AbstractActionTest<CreateTrainAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Train: Terran_Marine";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    TrainAction trainAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readUnitType()).
      WillOnce(testing::Return(UnitTypes::Terran_Marine));

    trainAction.read(reader);

    EXPECT_EQ(UnitTypes::Terran_Marine, trainAction.getTraineeType());
  }
};

TEST_F(TrainActionTest, TrainAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Train, action->getAction());
}

TEST_F(TrainActionTest, read)
{
  testRead();
}

TEST_F(TrainActionTest, toString)
{
  testToString();
}
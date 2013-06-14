#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "MorphAction.h"
#include "AbstractActionTest.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define DEFAULT_PLAYER  (1)

ReplayTool::GameAction* CreateMorphAction() { return new BuildingMorphAction(DEFAULT_PLAYER, UnitTypes::Terran_Barracks); }

class MorphActionTest : public AbstractActionTest<CreateMorphAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Morph Building: Terran_Barracks";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    BuildingMorphAction morphAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readUnitType()).
      WillOnce(testing::Return(UnitTypes::Terran_Barracks));

    morphAction.read(reader);

    EXPECT_EQ(UnitTypes::Terran_Barracks, morphAction.getTarget());
  }
};

TEST_F(MorphActionTest, MorphAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Building_Morph, action->getAction());
}

TEST_F(MorphActionTest, read)
{
  testRead();
}

TEST_F(MorphActionTest, toString)
{
  testToString();
}
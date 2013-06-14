#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "ResearchAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Research: %s"
#define DEFAULT_PLAYER  (1)
#define TECH_NAME       "Stim_Packs"

ReplayTool::GameAction* CreateResearchAction() { return new ResearchAction(DEFAULT_PLAYER, TechTypes::Stim_Packs); }

class ResearchActionTest : public AbstractActionTest<CreateResearchAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Research: Stim_Packs";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    ResearchAction rsrchAction(DEFAULT_PLAYER);

    EXPECT_CALL(reader, readTechType()).
      WillOnce(testing::Return(TechTypes::Stim_Packs));

    rsrchAction.read(reader);

    EXPECT_EQ(TechTypes::Stim_Packs, rsrchAction.getResearch());
  }
};

TEST_F(ResearchActionTest, ResearchAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Research, action->getAction());
}

TEST_F(ResearchActionTest, read)
{
  testRead();
}

TEST_F(ResearchActionTest, toString)
{
  testToString();
}
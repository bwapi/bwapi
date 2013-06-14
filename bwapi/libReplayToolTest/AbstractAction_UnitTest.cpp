#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "GameAction.h"
#include "DefaultActions.h"
#include "ResearchAction.h"

using namespace ReplayTool;
using namespace std;

TEST(AbstractActionFactoryTest, create)
{
  GameAction* action = g_actionFactory.create(0, ReplayTool::Research);
  EXPECT_EQ(ReplayTool::Research, action->getAction());

  delete action;
}

TEST(AbstractActionFactoryTest, instance)
{
  GameAction::Factory *expectedFactory = &g_actionFactory;
  ASSERT_NE(expectedFactory, (const GameAction::Factory*)NULL);

  GameAction::Factory *actualFactory = &g_actionFactory;
  ASSERT_NE(actualFactory, (const GameAction::Factory*)NULL);

  EXPECT_EQ(expectedFactory, actualFactory);
}

TEST(AbstractActionTest, toString)
{
  GameAction* action = g_actionFactory.create(0, ReplayTool::Research);

  string expected = "(P0) Research: ";
  string actual = action->toString();

  EXPECT_TRUE(strstr(actual.c_str(), expected.c_str()) != NULL);
}

TEST(AbstractActionTest, create)
{
  GameAction* action = g_actionFactory.create(0, ReplayTool::Research);
  EXPECT_EQ(ReplayTool::Research, action->getAction());

  delete action;
}
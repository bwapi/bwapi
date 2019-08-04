#include "gtest/gtest.h"

#include "game.h"
#include "ZergBaseFixture.h"

#define COMMANDWAIT 5

using namespace Funtest;

TEST_F(ZergBaseFixture, ZergTrainTest)
{
  if (self.minerals() < 500)
  {
    funGame->sendText("Show me the money");
    funGame.advance(10);
  }

  // Test Ordering a hatchery to train.
  
  funGame.advance(200);
  
  hatchery.train(UnitTypes::Zerg_Zergling);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Zerg_Egg), 1);  
}

// Test that the infested Command Center can land on creep.
TEST_F(ZergBaseFixture, ZergLandTest)
{
  auto tilePosition = infestedCommandCenter.getTilePosition();
  infestedCommandCenter.lift();
  funGame.advance(100);
  EXPECT_EQ(infestedCommandCenter.isLifted(), true);
  infestedCommandCenter.land(tilePosition);
  funGame.advance(100);
  EXPECT_EQ(infestedCommandCenter.isLifted(), false);
}
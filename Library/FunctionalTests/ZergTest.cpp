#include <gtest/gtest.h>

#include "game.h"
#include "ZergBaseFixture.h"

#define COMMANDWAIT 5

using namespace Funtest;

TEST_F(ZergBaseFixture, ZergTrainTest)
{
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

TEST_F(ZergBaseFixture, MorphBurrowTest)
{
  hydraliskDen->research(TechTypes::Lurker_Aspect);
  funGame.advance(10);
  EXPECT_EQ(self.hasResearched(TechTypes::Lurker_Aspect), true);

  hatchery->train(UnitTypes::Zerg_Hydralisk);
  while (self.completedUnitCount(UnitTypes::Zerg_Hydralisk) < 1)
    funGame.advance();
  auto u = getFirstOwnedUnit(Filter::GetType == UnitTypes::Zerg_Hydralisk);
  u.morph(UnitTypes::Zerg_Lurker);
  funGame.advance(60);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Zerg_Lurker), 1);

  u->burrow();
  funGame.advance(60);
  EXPECT_EQ(u->isBurrowed(), true);
}
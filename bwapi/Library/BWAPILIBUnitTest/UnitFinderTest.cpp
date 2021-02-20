#include <gtest/gtest.h>

#include "UnitFinderFixture.h"

#include <BWAPI/UnitFinder.h>
#include <BWAPI/Unit.h>

using namespace BWAPI;

// TODO: More tests @_@

TEST_F(UnitFinderFixture, clearsOnUpdate)
{
  EXPECT_EQ(countUnits(), 0);

  Unitset units{ createFakeUnit() };
  finder.update(units);
  EXPECT_EQ(countUnits(), 1);

  finder.update(Unitset{});
  EXPECT_EQ(countUnits(), 0);
}

/*
This is broken, not really a unit test but was trying to test getClosestUnit
TEST_F(UnitFinderFixture, findsClosest)
{
  createFakeUnit(UnitTypes::Zerg_Hatchery);
  game.update();

  Position from = Position{ TilePosition{ game.mapWidth(), game.mapHeight() } } / 2;
  auto requirement = BWAPI::Filter::IsResourceDepot && BWAPI::Filter::IsOwned && BWAPI::Filter::IsCompleted && !BWAPI::Filter::IsLifted && BWAPI::Filter::Exists;
  
  Unit found = game.getClosestUnit(from, requirement);
  EXPECT_NE(found, nullptr);
}
*/
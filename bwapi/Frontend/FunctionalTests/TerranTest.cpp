#include "gtest/gtest.h"

#include "game.h"
#include "TerranBaseFixture.h"

TEST_F(TerranBaseFixture, Stuff2)
{
  Funtest::funGame.advance(20);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Terran_Academy), 1);
}
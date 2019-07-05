#include "gtest/gtest.h"

#include "game.h"
#include "ProtossBaseFixture.h"

TEST_F(ProtossBaseFixture, Stuff)
{
  probes.gather(mineralField);
  while (Funtest::funGame.advance(), self.minerals() < 50);
  
  nexus.train(UnitTypes::Protoss_Probe);
  Funtest::funGame.advance(10);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Protoss_Probe), 5);
}

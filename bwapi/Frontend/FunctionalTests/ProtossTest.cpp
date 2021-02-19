#include <gtest/gtest.h>

#include "game.h"
#include "ProtossBaseFixture.h"

#define COMMANDWAIT 5

TEST_F(ProtossBaseFixture, GatherTrain)
{
  probes.gather(mineralField);
  while (Funtest::funGame.advance(), self.minerals() < 50);

  nexus.train(UnitTypes::Protoss_Probe);
  Funtest::funGame.advance(10);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Protoss_Probe), 5);
}

TEST_F(ProtossBaseFixture, BuildPower)
{
  using namespace Funtest;
  funGame.advance(2);
  if (self.minerals() < 500)
  {
    funGame->sendText("Show me the money");
    funGame->sendText("operation cwal");
    funGame.advance(5);
  }
  ASSERT_FALSE(probes.empty());
  Unit probe = *probes.begin();
  UnitType toBuild = UnitTypes::Protoss_Pylon;
  probe->build(toBuild, funGame->getBuildLocation(toBuild, probe.getTilePosition()));
  funGame.advance(120);
  EXPECT_EQ(self.allUnitCount(toBuild), 1);

  while (self.completedUnitCount(toBuild) < 1)
    funGame.advance();

  toBuild = UnitTypes::Protoss_Gateway;
  probe->build(toBuild, funGame->getBuildLocation(toBuild, probe.getTilePosition()));
  funGame.advance(120);
  EXPECT_EQ(self.allUnitCount(toBuild), 1);


  funGame->sendText("operation cwal");
  funGame.advance(COMMANDWAIT);
}

TEST_F(ProtossBaseFixture, NoPowerTest)
{
  using namespace Funtest;
  UnitType toBuild = UnitTypes::Protoss_Gateway;

  ASSERT_FALSE(probes.empty());
  Unit probe = *probes.begin();
  EXPECT_EQ(probe->build(toBuild, nexus.getTilePosition() + TilePosition{ -1 * toBuild.tileWidth(), 0 }), false);

  funGame->createUnit(self, toBuild, nexus.getPosition() + Position{ -1 * toBuild.width(), 0 }, 1);
  funGame.advance(2);

  auto u = getFirstOwnedUnit(Filter::GetType == toBuild);
  EXPECT_EQ(u->isPowered(), false);
}

TEST_F(ProtossBaseFixture, StormTest)
{
  using namespace Funtest;
  funGame->sendText("operation cwal");
  funGame.advance(COMMANDWAIT);
  funGame->sendText("show me the money");
  funGame.advance(COMMANDWAIT);

  TechType storm = TechTypes::Psionic_Storm;
  EXPECT_EQ(self.hasResearched(storm), false);

  UnitType archives = UnitTypes::Protoss_Templar_Archives;
  funGame->createUnit(self, archives, static_cast<Position>(TilePosition{ 3, 18 }));
  funGame->createUnit(self, UnitTypes::Protoss_Pylon, static_cast<Position>(TilePosition{ 3, 15 }));
  funGame.advance(2);

  auto u = getFirstOwnedUnit(Filter::GetType == archives);
  u.research(storm);
  funGame.advance(COMMANDWAIT);
  while (u.isResearching())
    funGame.advance();
  EXPECT_EQ(self.hasResearched(storm), true);

  UnitType templar = UnitTypes::Protoss_High_Templar;

  funGame->createUnit(self, templar, Position{ 0, 0 }, 1);

  funGame.advance(20);

  u = getFirstOwnedUnit(Filter::GetType == templar);
  EXPECT_EQ(u.isUnderStorm(), false);
  u.useTech(storm, Position{ 10, 10 });
  funGame.advance(23);

  EXPECT_EQ(u.isUnderStorm(), true);
  funGame->sendText("operation cwal");
  funGame.advance(COMMANDWAIT);


}

TEST_F(ProtossBaseFixture, ReaverTest)
{
  using namespace Funtest;
  UnitType reaver = UnitTypes::Protoss_Reaver;
  //Unit
  funGame->sendText("show me the money");
  funGame->createUnit(self, reaver, Position{ 0, 0 }, 1);
  funGame.advance(COMMANDWAIT);

  auto u = getFirstOwnedUnit(Filter::GetType == reaver);
  EXPECT_EQ(u.getScarabCount(), 0);
  u->train(UnitTypes::Protoss_Scarab);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u->getTrainingQueue().size(), 1U);
  u->train(UnitTypes::Protoss_Scarab);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u->getTrainingQueue().size(), 2U);
  u->cancelTrain();
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u->getTrainingQueue().size(), 1U);
  funGame.advance(UnitTypes::Protoss_Scarab.buildTime() - COMMANDWAIT * 2);
  EXPECT_EQ(u.getScarabCount(), 1);
}


TEST_F(ProtossBaseFixture, CarrierTest)
{
  using namespace Funtest;
  UnitType carrier = UnitTypes::Protoss_Carrier;
  //Unit
  funGame->sendText("show me the money");
  funGame->createUnit(self, carrier, Position{ 0, 0 }, 1);
  funGame.advance(COMMANDWAIT);
  funGame->sendText("operation cwal");
  funGame.advance(COMMANDWAIT);

  auto u = getFirstOwnedUnit(Filter::GetType == carrier);
  EXPECT_EQ(u.getInterceptorCount(), 0);
  u->train(UnitTypes::Protoss_Interceptor);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u->getTrainingQueue().size(), 1U);
  u->train(UnitTypes::Protoss_Interceptor);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u->getTrainingQueue().size(), 2U);
  u->cancelTrain();
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u->getTrainingQueue().size(), 1U);
  funGame.advance(UnitTypes::Protoss_Interceptor.buildTime() - COMMANDWAIT * 2);
  EXPECT_EQ(u.getInterceptorCount(), 1);
}
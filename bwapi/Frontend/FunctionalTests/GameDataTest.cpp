#include "gtest/gtest.h"

#include "game.h"

#define COMMANDWAIT 5

using namespace BWAPI;

TEST(TriggerTests, KillUnitTest)
{
  using namespace Funtest;

  funGame->setLocalSpeed(0);
  auto self = Funtest::funGame->self();
  auto marine = UnitTypes::Terran_Marine;

  funGame->createUnit(self, marine, Position{ 20, 20 }, 1);
  funGame.advance(COMMANDWAIT);

  auto units = funGame->getAllUnits();
  auto selfMarine = *units.begin();

  funGame->killUnit(selfMarine);
  funGame.advance(COMMANDWAIT);
  funGame->killUnit(selfMarine);
  funGame.advance(COMMANDWAIT);
  funGame.reset();
  funGame.advance(2);
}

TEST(GameDataTest, CloakedUnits)
{
  using namespace Funtest;

  funGame->setLocalSpeed(0);

  auto self = Funtest::funGame->self();
  Player neutral = nullptr;
  Player enemy = nullptr;
  for (auto &p : funGame->getPlayers())
  {
    if (p.getType() == PlayerTypes::Neutral)
      neutral = p;
    if (p.getType() == PlayerTypes::Computer)
      enemy = p;
    if (neutral && enemy)
      break;
  }
  auto marine = UnitTypes::Terran_Marine;
  auto darkTemplar = UnitTypes::Protoss_Dark_Templar;

  funGame->createUnit(self, marine, Position{ 20, 20 }, 1);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(funGame->getAllUnits().size(), 1);

  funGame->createUnit(neutral, darkTemplar, Position{ 20, 21 }, 1);
  funGame.advance(COMMANDWAIT);
  auto allUnits = funGame->getAllUnits();
  
  EXPECT_EQ(allUnits.size(), 2);

  for (auto &u : allUnits)
  {
    if (u.getPlayer() == neutral)
      EXPECT_EQ(u.getType(), UnitTypes::Protoss_Dark_Templar);
  }

  funGame->createUnit(enemy, darkTemplar, Position{ 100, 100 }, 1);
  funGame.advance(COMMANDWAIT);

  allUnits = funGame->getAllUnits();
  EXPECT_EQ(allUnits.size(), 3);

  for (auto &u : allUnits)
  {
    if (u.getPlayer() == enemy)
    {
      while (!u.isCloaked())
        funGame.advance();
      EXPECT_EQ(u.getType(), UnitTypes::Protoss_Dark_Templar);
      EXPECT_EQ(u.getHitPoints(), 0);
    }
  }

  funGame.reset();
  funGame.advance(2);
}

TEST(GameDataTest, ForcesTest)
{
  using namespace Funtest;

  auto forces = funGame->getForces();
  EXPECT_EQ(forces.size(), 4);
  int i = 0;
  for (auto &f : forces)
  {
    switch (i)
    {
    case 0:
      EXPECT_EQ(f.getPlayers().size(), 1);
      break;
    case 1:
      EXPECT_EQ(f.getPlayers().size(), 7);
      break;
    case 2:
      EXPECT_EQ(f.getPlayers().size(), 0);
      break;
    case 3:
      EXPECT_EQ(f.getPlayers().size(), 0);
      break;
    }
    i++;
  }
  funGame.advance(2);
}
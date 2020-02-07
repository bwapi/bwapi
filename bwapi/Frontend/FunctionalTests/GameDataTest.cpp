#include "gtest/gtest.h"

#include "game.h"
#include <BWAPI/GameType.h>

#define COMMANDWAIT 5

using namespace BWAPI;
using namespace Funtest;

TEST(TriggerTests, CreateUnitTest)
{
  auto self = funGame->self();

  EXPECT_EQ(self.allUnitCount(), 0);

  funGame->createUnit(self, UnitTypes::Terran_Battlecruiser, Position{ 80, 80 }, 2);
  funGame->createUnit(self, UnitTypes::Zerg_Spawning_Pool, Position{ 180, 180 }, 1);
  funGame.advance(COMMANDWAIT);

  EXPECT_EQ(self.allUnitCount(), 3);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Terran_Battlecruiser), 2);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Zerg_Spawning_Pool), 1);
  funGame.reset();
}

TEST(TriggerTests, KillUnitTest)
{
  auto self = funGame->self();
  auto marine = UnitTypes::Terran_Marine;

  funGame->createUnit(self, marine, Position{ 20, 20 }, 1);
  funGame.advance(COMMANDWAIT);

  auto units = funGame->getAllUnits();
  ASSERT_EQ(units.size(), 1);

  auto selfMarine = *units.begin();

  funGame->killUnit(selfMarine);
  funGame.advance(COMMANDWAIT);
  funGame->killUnit(selfMarine);
  funGame.advance(COMMANDWAIT);
  funGame.reset();
}

TEST(GameDataTest, CloakedUnits)
{
  auto self = funGame->self();
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
}

TEST(GameDataTest, ForcesTest)
{
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
}

TEST(GameDataTest, GameTypeTest)
{
  EXPECT_EQ(funGame->getGameType(), BWAPI::GameTypes::Use_Map_Settings);
}

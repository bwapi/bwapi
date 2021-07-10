#include <gtest/gtest.h>

#include "game.h"
#include "TerranBaseFixture.h"

#define COMMANDWAIT 5

using namespace Funtest;

TEST_F(TerranBaseFixture, TerranTrain)
{
  // Test Command Center
  commandCenter.train(UnitTypes::Terran_SCV);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(commandCenter.isTraining(), true);
  EXPECT_EQ(commandCenter.isIdle(), false);
  EXPECT_EQ(commandCenter.getTrainingQueue().size(), 1U);
  commandCenter.train(UnitTypes::Terran_SCV);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(commandCenter.getTrainingQueue().size(), 2U);
  commandCenter.cancelTrain();
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(commandCenter.getTrainingQueue().size(), 1U);
  commandCenter.cancelTrain();
  funGame.advance(COMMANDWAIT);
}

TEST_F(TerranBaseFixture, TerranLift)
{
  commandCenter.lift();
  funGame.advance(100);
  EXPECT_EQ(commandCenter.isLifted(), true);
  commandCenter.land(TilePosition{ 0, 0 });
  funGame.advance(100);
  EXPECT_EQ(commandCenter.isLifted(), false);
}

TEST_F(TerranBaseFixture, TerranAddon)
{
  auto type = UnitTypes::Terran_Comsat_Station;
  commandCenter.buildAddon(type);
  funGame.advance(type.buildTime() + COMMANDWAIT);
  EXPECT_EQ(self.completedUnitCount(type), 1);
}

TEST_F(TerranBaseFixture, TerranResearch)
{
  // Test Researching at Academy
  academy.research(TechTypes::Stim_Packs);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(academy.isResearching(), true);
}

TEST_F(TerranBaseFixture, TerranUpgrade)
{
  // Test Upgrading at Engineering Bay
  engineeringBay.upgrade(UpgradeTypes::Terran_Infantry_Weapons);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(engineeringBay.isUpgrading(), true);
}

TEST_F(TerranBaseFixture, TerranAddonMove)
{
  // Test building an addon when we have to move
  factory.build(UnitTypes::Terran_Machine_Shop, TilePosition{ 4, 4 } + factory.getTilePosition());
  funGame.advance(200);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Terran_Machine_Shop), 1);
}

TEST_F(TerranBaseFixture, TerranBunker)
{
  //Test information around a bunker, and test loading/unloading.
  EXPECT_EQ(bunker.getLoadedUnits().size(), 0U);
  Position bunkerPosition = bunker.getPosition();
  funGame->createUnit(self, UnitTypes::Terran_Marine, bunkerPosition + Position{ 0, bunker.getType().height() }, 2);
  funGame.advance(2);
  auto marines = funGame->getUnitsInRadius(bunkerPosition, 500, Filter::IsOwned && Filter::GetType == UnitTypes::Terran_Marine);
  EXPECT_EQ(marines.size(), 2U);
  for (auto& u : marines)
  {
    if (!u.isLoaded())
    {
      bunker.load(u);
      break;
    }
  }
  funGame.advance(40);
  EXPECT_EQ(bunker.getLoadedUnits().size(), 1U);
  for (auto& u : marines)
  {
    if (!u.isLoaded())
    {
      bunker.load(u);
      break;
    }
  }
  funGame.advance(40);
  EXPECT_EQ(bunker.getLoadedUnits().size(), 2U);
  bunker.unloadAll();
  funGame.advance(40);
  EXPECT_EQ(bunker.getLoadedUnits().size(), 0U);
}

TEST_F(TerranBaseFixture, Nuke)
{
  //Spawn a Ghost, build silo, build a nuke, launch it, and check the nuke dot!
  auto ghost = UnitTypes::Terran_Ghost;
  auto nukeSilo = UnitTypes::Terran_Nuclear_Silo;
  funGame->createUnit(self, ghost, Position{ 100, 100 }, 1);
  scienceFacility->buildAddon(UnitTypes::Terran_Covert_Ops);
  funGame->sendText("Operation cwal");
  funGame.advance(100);
  commandCenter->buildAddon(nukeSilo);
  funGame.advance(100);
  
  auto u = getFirstOwnedUnit(Filter::GetType == nukeSilo);
  u->train(UnitTypes::Terran_Nuclear_Missile);
  funGame.advance(100);
  EXPECT_EQ(u->hasNuke(), true);
  EXPECT_EQ(funGame->getNukeDots().size(), 0U);

  u = getFirstOwnedUnit(Filter::GetType == ghost);
  u->useTech(TechTypes::Nuclear_Strike, Position{ 50, 50 });
  funGame.advance(60);
  EXPECT_EQ(funGame->getNukeDots().size(), 1U);
  EXPECT_EQ(*funGame->getNukeDots().begin(), Position(50, 50));
  funGame->killUnit(u);
  funGame.advance(10);
  EXPECT_EQ(funGame->getNukeDots().size(), 0U);
  
  funGame->createUnit(self, ghost, Position{ 100, 100 }, 1);
  u = getFirstOwnedUnit(Filter::GetType == nukeSilo);
  u->train(UnitTypes::Terran_Nuclear_Missile);
  funGame.advance(100);
  u = getFirstOwnedUnit(Filter::GetType == ghost);
  u->useTech(TechTypes::Nuclear_Strike, Position{ 50, 50 });
  funGame.advance(60);
  EXPECT_EQ(funGame->getNukeDots().size(), 1U);
  funGame->sendText("Operation cwal");
  funGame.advance(400);
  EXPECT_EQ(funGame->getNukeDots().size(), 0U);
}
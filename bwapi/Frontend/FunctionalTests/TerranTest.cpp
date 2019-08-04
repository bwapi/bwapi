#include "gtest/gtest.h"

#include "game.h"
#include "TerranBaseFixture.h"

#define COMMANDWAIT 5

using namespace Funtest;

TEST_F(TerranBaseFixture, TerranStructureTest)
{
  if (self.minerals() < 500)
  {
    funGame->sendText("Show me the money");
    funGame.advance(10);
  }

  // Test Command Center
  auto u = funGame->getBestUnit([](Unit one, Unit two) { return one; }, Filter::IsOwned && Filter::GetType == UnitTypes::Terran_Command_Center);
  u.train(UnitTypes::Terran_SCV);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u.isTraining(), true);
  EXPECT_EQ(u.isIdle(), false);
  EXPECT_EQ(u.getTrainingQueue().size(), 1);
  u.train(UnitTypes::Terran_SCV);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u.getTrainingQueue().size(), 2);
  u.cancelTrain();
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u.getTrainingQueue().size(), 1);
  u.cancelTrain();
  funGame.advance(COMMANDWAIT);
  u.lift();
  funGame.advance(100);
  EXPECT_EQ(u.isLifted(), true);
  u.land(TilePosition{ 0, 0 });
  funGame.advance(100);
  EXPECT_EQ(u.isLifted(), false);
  auto type = UnitTypes::Terran_Comsat_Station;
  u.buildAddon(type);
  funGame.advance(type.buildTime() + COMMANDWAIT);
  EXPECT_EQ(self.completedUnitCount(type), 1);

  // Test Researching at Academy
  u = funGame->getBestUnit([](Unit one, Unit two) { return one; }, Filter::IsOwned && Filter::GetType == UnitTypes::Terran_Academy);
  u.research(TechTypes::Stim_Packs);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u.isResearching(), true);

  // Test Upgrading at Engineering Bay
  u = funGame->getBestUnit([](Unit one, Unit two) { return one; }, Filter::IsOwned && Filter::GetType == UnitTypes::Terran_Engineering_Bay);
  u.upgrade(UpgradeTypes::Terran_Infantry_Weapons);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(u.isUpgrading(), true);

  // Test building an addon when we have to move
  u = funGame->getBestUnit([](Unit one, Unit two) { return one; }, Filter::IsOwned && Filter::GetType == UnitTypes::Terran_Factory);
  u.build(UnitTypes::Terran_Machine_Shop, TilePosition{ 4, 4 } + u.getTilePosition());
  funGame.advance(200);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Terran_Machine_Shop), 1);
}
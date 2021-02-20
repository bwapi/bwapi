#include <gtest/gtest.h>

#include "game.h"

int constexpr COMMANDWAIT = 5;

using namespace Funtest;
using namespace BWAPI;

//set up reused items to easier names/less function calls.
auto commandCenter = BWAPI::UnitTypes::Terran_Command_Center;
auto commandCenterWidth = commandCenter.tileWidth();
auto SCV = BWAPI::UnitTypes::Terran_SCV;
auto comsat = BWAPI::UnitTypes::Terran_Comsat_Station;
auto comsatWidth = comsat.tileWidth();


TEST(UnitSetTest, Train)
{
  auto self = funGame->self();

  funGame->sendText("Show me the money");
  funGame.advance(10);

  BWAPI::TilePosition positions[] = {
    {2, 2},
    {(commandCenter.tileWidth() + comsat.tileWidth()) + 4, 2},
    {2 * (commandCenter.tileWidth() + comsat.tileWidth()) + 6, 2}
  };

  auto testPosition = static_cast<Position>(positions[1]);
  
  //Spawn in command centers.
  for (int i = 0; i < std::extent<decltype(positions)>::value; i++)
    funGame->createUnit(self, commandCenter, static_cast<Position>(positions[i]), 1);
  funGame.advance(2);
  //verify spawn.
  EXPECT_EQ(self.allUnitCount(commandCenter), int(std::extent<decltype(positions)>::value));

  auto orderUnits = self.getUnits();
  //Verify we have the expected # of spawned units
  EXPECT_EQ(orderUnits.size(), std::extent<decltype(positions)>::value);

  //Order training.
  orderUnits.train(SCV);
  funGame.advance(COMMANDWAIT);

  auto numberTrainedUnits = self.incompleteUnitCount(SCV);
  EXPECT_EQ(numberTrainedUnits, int(std::extent<decltype(positions)>::value));

  //Let the SCVs build a little.
  funGame.advance(46);

  //Cancel the SCVs.
  orderUnits.cancelTrain();
  funGame.advance(COMMANDWAIT);
  
  numberTrainedUnits = self.incompleteUnitCount(SCV);
  EXPECT_EQ(numberTrainedUnits, 0);

  //Retrain the SCVs, but this time we'll let them complete.
  orderUnits.train(SCV);
  funGame.advance(SCV.buildTime() + COMMANDWAIT);

  //SCVs should be complete, lets verify.
  numberTrainedUnits = self.incompleteUnitCount(SCV);
  auto numberCompletedUnits = self.completedUnitCount(SCV);
  EXPECT_EQ(numberTrainedUnits, 0);
  EXPECT_EQ(numberCompletedUnits, 3);
  funGame.reset();
}

TEST(UnitSetTest, AddOn)
{
  auto self = Funtest::funGame->self();
  Unitset orderUnits;

  //Create the required Academy.
  TilePosition position = { 1, 5 };
  funGame->createUnit(self, UnitTypes::Terran_Academy, static_cast<Position>(position));
  funGame.advance(2);

  //Get the Command Centers.
  for (auto &u : self.getUnits())
  {
    if (u.getType() == commandCenter)
      orderUnits.insert(u);
  }
  //issue order, and verify the addons were built.
  orderUnits.buildAddon(comsat);
  funGame.advance(COMMANDWAIT);

  //Verify the Addons were built
  auto buildingAddOns = self.incompleteUnitCount(comsat);
  EXPECT_EQ(buildingAddOns, 3);

  //Cancel the addons.
  orderUnits.cancelAddon();
  funGame.advance(COMMANDWAIT);

  //verify they were cancelled.
  buildingAddOns = self.incompleteUnitCount(comsat);
  EXPECT_EQ(buildingAddOns, 0);

  //Order the addons again, this time we will let them complete.
  funGame.advance(23);
  orderUnits.buildAddon(comsat);
  funGame.advance(comsat.buildTime() + COMMANDWAIT);

  //Verify they were built.
  buildingAddOns = self.incompleteUnitCount(comsat);
  auto builtAddOns = self.completedUnitCount(comsat);
  EXPECT_EQ(buildingAddOns, 0);
  EXPECT_EQ(builtAddOns, 3);
  funGame.reset();
}

TEST(UnitSetTest, LurkerMixed)
{
  auto self = Funtest::funGame->self();

  funGame->createUnit(self, UnitTypes::Zerg_Lurker, Position{ 0, 0 }, 1);
  funGame.advance(2);
  auto us = self.getUnits();
  for (auto& u : us)
    u.burrow();
  funGame.advance(60);

  funGame->createUnit(self, UnitTypes::Zerg_Ultralisk, Position{ 0, 0 }, 5);
  funGame.advance(2);
  us = self.getUnits();
  EXPECT_EQ(us.attack(Position{ 100, 100 }), true);
  funGame.reset();
}

TEST(UnitSetTest, NuetralUnitSetTest)
{
  auto self = funGame->self();

  funGame->createUnit(self, UnitTypes::Resource_Mineral_Field, Position{ 200, 200 }, 1);
  funGame->createUnit(self, UnitTypes::Resource_Mineral_Field, Position{ 1920, 1920 }, 1);
  funGame->createUnit(self, UnitTypes::Special_Map_Revealer, Position{ 200, 200 }, 1);

  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(funGame->getNeutralUnits().size(), 1U);
  funGame.reset();
}
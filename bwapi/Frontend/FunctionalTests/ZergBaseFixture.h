#pragma once

#include <gtest/gtest.h>

#include "TestFixture.h"

#include <BWAPI.h>
#include "game.h"

using namespace BWAPI;

class ZergBaseFixture : public TestFixture {
public:
  ZergBaseFixture()
  {
    using namespace Funtest;

    UnitType unitTypes[17];

    int unitType = 130;

    for (int i = 0; i < 17; i++)
    {
      if (unitType != 145 && unitType != 147 && unitType != 148)
        unitTypes[i] = static_cast<UnitType>(unitType);
      else
        i--;
      unitType++;
    }

    Position positions[17];

    for (int i = 0; i < 17; i++)
    {
      Position result = { 0, 0 };
      for (int j = 0; j < i; j++)
      {
        result += Position{ 32 * unitTypes[j].tileWidth(), 0 };
      }
      result += Position{ 32 * unitTypes[i].tileWidth() / 2, 32 * unitTypes[i].tileHeight() / 2 };
      positions[i] = result;
    }

    funGame.advance(2);

    for (int i = 0; i < std::extent<decltype(positions)>::value; i++)
    {
      if (!unitTypes[i].isAddon())
        funGame->createUnit(funGame->self(), unitTypes[i], positions[i]);
    }
    funGame->sendText("show me the money");
    funGame.advance(5);
    funGame->sendText("operation cwal");
    funGame.advance(5);

    hatchery = getFirstOwnedUnit(Filter::GetType == UnitTypes::Zerg_Hatchery);
    infestedCommandCenter = getFirstOwnedUnit(Filter::GetType == UnitTypes::Zerg_Infested_Command_Center);
    hydraliskDen = getFirstOwnedUnit(Filter::GetType == UnitTypes::Zerg_Hydralisk_Den);
  }

  ~ZergBaseFixture() {}
protected:
  Unit hatchery = nullptr;
  Unit infestedCommandCenter = nullptr;
  Unit hydraliskDen = nullptr;
};
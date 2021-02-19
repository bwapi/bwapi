#pragma once
#include <gtest/gtest.h>

#include "TestFixture.h"

#include <BWAPI.h>
#include "game.h"

using namespace BWAPI;

class TerranBaseFixture : public TestFixture {
public:
  TerranBaseFixture()
  {
    using namespace Funtest;

    UnitType unitTypes[18];

    int unitType = 106;

    for (int i = 0; i < 18; i++)
    {
      if (unitType != 119 && unitType != 121)
        unitTypes[i] = static_cast<UnitType>(unitType);
      else
        i--;
      unitType++;
    }

    Position positions[18];

    for (int i = 0; i < 18; i++)
    {
      Position result = { 0, 0 };
      for (int j = 0; j < i; j++)
      {
        result += Position{ 32 * unitTypes[j].tileWidth(), 0 };
      }
      result += Position{ 32 * unitTypes[i].tileWidth() / 2, 32 * unitTypes[i].tileHeight() / 2 };
      positions[i] = result;
    }

    for (int i = 0; i < std::extent<decltype(positions)>::value; i++)
    {
      if (!unitTypes[i].isAddon())
        funGame->createUnit(funGame->self(), unitTypes[i], positions[i]);
    }

    if (funGame->self().minerals() < 500)
    {
      funGame->sendText("Show me the money");
      funGame.advance(10);
    }

    funGame.advance(100);

    BWAPI::Unitset units = self->getUnits();

    commandCenter = getFirstOwnedUnit(Filter::GetType == UnitTypes::Terran_Command_Center);
    academy = getFirstOwnedUnit(Filter::GetType == UnitTypes::Terran_Academy);
    engineeringBay = getFirstOwnedUnit(Filter::GetType == UnitTypes::Terran_Engineering_Bay);
    factory = getFirstOwnedUnit(Filter::GetType == UnitTypes::Terran_Factory);
    bunker = getFirstOwnedUnit(Filter::GetType == UnitTypes::Terran_Bunker);
    scienceFacility = getFirstOwnedUnit(Filter::GetType == UnitTypes::Terran_Science_Facility);
  }

  virtual ~TerranBaseFixture() {}
protected:
  Unit commandCenter = nullptr;
  Unit academy = nullptr;
  Unit engineeringBay = nullptr;
  Unit factory = nullptr;
  Unit bunker = nullptr;
  Unit scienceFacility = nullptr;
};
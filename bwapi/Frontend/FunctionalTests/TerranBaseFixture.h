#pragma once
#include "gtest/gtest.h"

#include<BWAPI.h>
#include"game.h"

using namespace BWAPI;

class TerranBaseFixture : public ::testing::Test {
public:
  TerranBaseFixture()
  {
    using namespace Funtest;
    funGame.advance(1);

    UnitType unitTypes[] = {
      UnitTypes::Terran_Academy,
      UnitTypes::Terran_Armory,
      UnitTypes::Terran_Barracks
    };

    TilePosition positions[] = {
      {1, 1},
      {4, 1},
      {7, 1}
    };

    for (int i = 0; i < std::extent<decltype(positions)>::value; i++)
      funGame->createUnit(funGame->self(), unitTypes[i], static_cast<Position>(positions[i]));

    funGame.advance(2);

    self = funGame->self();
  }

  virtual ~TerranBaseFixture()
  {
    Funtest::funGame.reset();
  }
protected:
  Player self = nullptr;
};
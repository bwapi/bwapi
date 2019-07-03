#pragma once
#include "gtest/gtest.h"

#include "MockClient.h"

#include <BWAPI/Game.h>
#include <BWAPI/Position.h>

class GameFixture : public ::testing::Test {
public:
  GameFixture() : game(client)
  {
    game.gameData->hasGUI = true;
  }

protected:

  MockClient client;
  BWAPI::Game game;
};

class GameFixture_SmallMap : public GameFixture {
public:
  GameFixture_SmallMap() : GameFixture()
  {
    game.gameData->map.size = BWAPI::TilePosition(64, 96);
    // TODO: Create a map w/ state
  }
};

#pragma once
#include "gtest/gtest.h"

#include "MockClient.h"

#include <BWAPI/Game.h>

class GameFixture : public ::testing::Test {
public:
  GameFixture() : game(client) {}

protected:

  MockClient client;
  BWAPI::Game game;
};

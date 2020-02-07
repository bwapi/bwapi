#pragma once
#include "gtest/gtest.h"

#include <BWAPI.h>
#include "game.h"

using namespace BWAPI;

class TestFixture : public ::testing::Test {
public:
  TestFixture()
  {
    using namespace Funtest;

    self = funGame->self();
    enemy = funGame->enemy();
  }

  virtual ~TestFixture()
  {
    Funtest::funGame.reset();
  }

protected:
  Player self = nullptr;
  Player enemy = nullptr;
};

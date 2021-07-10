#pragma once
#include <gtest/gtest.h>

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

  BWAPI::Unit getFirstUnit(const BWAPI::UnitFilter& pred) {
    using namespace Funtest;

    auto it = funGame->getAllUnits().find_if(pred);
    if (it == funGame->getAllUnits().end()) return nullptr;
    return *it;
  }

  BWAPI::Unit getFirstOwnedUnit(const BWAPI::UnitFilter& pred) {
    using namespace Funtest;

    if (self == nullptr) return nullptr;

    auto it = self->getUnits().find_if(pred);
    if (it == self->getUnits().end()) return nullptr;
    return *it;
  }

protected:
  Player self = nullptr;
  Player enemy = nullptr;
};

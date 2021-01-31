#pragma once

#include <gtest/gtest.h>

#include "GameFixture.h"

#include <BWAPI/CommandOptimizer.h>

class CommandOptimizerFixture : public GameFixture {
public:
  CommandOptimizerFixture()
    : GameFixture()
    , optimizer(game)
  {
    optimizer.init();
  }

protected:
  BWAPI::CommandOptimizer optimizer;
};

class CommandOptimizer_L0 : public CommandOptimizerFixture {
public:
  CommandOptimizer_L0() : CommandOptimizerFixture() {
    optimizer.level = 0;
  }
};
class CommandOptimizer_L1 : public CommandOptimizerFixture {
public:
  CommandOptimizer_L1() : CommandOptimizerFixture() {
    optimizer.level = 1;
  }
};
class CommandOptimizer_L2 : public CommandOptimizerFixture {
public:
  CommandOptimizer_L2() : CommandOptimizerFixture() {
    optimizer.level = 2;
  }
};
class CommandOptimizer_L3 : public CommandOptimizerFixture {
public:
  CommandOptimizer_L3() : CommandOptimizerFixture() {
    optimizer.level = 3;
  }
};
class CommandOptimizer_L4 : public CommandOptimizerFixture {
public:
  CommandOptimizer_L4() : CommandOptimizerFixture() {
    optimizer.level = 4;
  }
};

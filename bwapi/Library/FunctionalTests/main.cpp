#include <gtest/gtest.h>

#include "game.h"

class Env : public ::testing::Environment {
public:
  void SetUp() override
  {
    Funtest::funGame.connect();
  }

  void TearDown() override
  {
  }
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Env());
  return RUN_ALL_TESTS();
}

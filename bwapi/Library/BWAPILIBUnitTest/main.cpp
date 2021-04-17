#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>

#include "BW/DataEnv.h"

int main(int argc, char **argv) {
  std::cout << "Current directory: " << std::filesystem::current_path() << std::endl;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new BW::DataEnv());
  return RUN_ALL_TESTS();
}

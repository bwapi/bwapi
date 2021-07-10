#pragma once

#include <gtest/gtest.h>
#include <BWAPI/UnitType.h>
#include <vector>

class UnitTypeFixture : public ::testing::TestWithParam<BWAPI::UnitType> {
public:
  // All BW types including unused types, but excluding fake types like "None"
  static const std::vector<BWAPI::UnitType> allInternalUnitTypes;
};

class NoneUnitTypeFixture : public ::testing::TestWithParam<BWAPI::UnitType> {
public:
  // All non-existent types (None, Men, Factories, etc) except for Unknown
  static const std::vector<BWAPI::UnitType> allNoneTypes;
};

class NoneUnknownUnitTypeFixture : public ::testing::TestWithParam<BWAPI::UnitType> {
public:
  // All non-existent types
  static const std::vector<BWAPI::UnitType> allNoneUnknownTypes;
};

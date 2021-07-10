#include "UnitTypeFixtures.h"

std::vector<BWAPI::UnitType> generateInternalUnitTypes() {
  std::vector<BWAPI::UnitType> result;
  for (int i = 0; i < BWAPI::UnitTypes::Enum::None; ++i) {
    result.emplace_back(i);
  }
  return result;
}

const std::vector<BWAPI::UnitType> UnitTypeFixture::allInternalUnitTypes = generateInternalUnitTypes();

const std::vector<BWAPI::UnitType> NoneUnitTypeFixture::allNoneTypes = {
  BWAPI::UnitTypes::None,
  BWAPI::UnitTypes::AllUnits,
  BWAPI::UnitTypes::Men,
  BWAPI::UnitTypes::Buildings,
  BWAPI::UnitTypes::Factories
};

const std::vector<BWAPI::UnitType> NoneUnknownUnitTypeFixture::allNoneUnknownTypes = {
  BWAPI::UnitTypes::None,
  BWAPI::UnitTypes::AllUnits,
  BWAPI::UnitTypes::Men,
  BWAPI::UnitTypes::Buildings,
  BWAPI::UnitTypes::Factories,
  BWAPI::UnitTypes::Unknown
};

#include <gtest/gtest.h>

#include <BWAPI/UnitType.h>
#include <BWAPI/Race.h>

using namespace BWAPI;

TEST(UnitTypes, GetRace) {
	UnitType terranMarine(UnitTypes::Terran_Marine);

	EXPECT_EQ(terranMarine.getRace(), Races::Terran);
}
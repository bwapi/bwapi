#include "gtest/gtest.h"

#include "UnitFinderFixture.h"

#include <BWAPI/UnitFinder.h>
#include <BWAPI/Unit.h>

using namespace BWAPI;

TEST_F(UnitFinderFixture, clearsOnUpdate)
{
  EXPECT_EQ(countUnits(), 0);

  Unitset units{ createFakeUnit() };
  finder.update(units);
  EXPECT_EQ(countUnits(), 1);

  finder.update(Unitset{});
  EXPECT_EQ(countUnits(), 0);
}

// TODO: More tests @_@

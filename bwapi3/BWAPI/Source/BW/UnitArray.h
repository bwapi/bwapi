#pragma once
/***********************************************************************
 *  Direct maping of the unit node table in bw memory.
 *
 *******/

#include "Unit.h" // BW::Unit

namespace BW
{
  struct UnitArray
  {
    Unit unit[UNIT_ARRAY_MAX_LENGTH];

    int getIndexByUnit(Unit* unit);
  };
}

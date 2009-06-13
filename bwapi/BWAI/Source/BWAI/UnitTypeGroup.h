#pragma once

#include <BWAPI.h>
namespace BWAI
{
  /* To add a new unit type group, just add another value to the Enum and add a case in the function isInUnitTypeGroup */
  namespace UnitTypeGroup
  {
    enum Enum
    {
      All,
      Men,
      Buildings,
      Factories,
      Infantry,
      Mech,
      Zerg_Main,
      Zerg_MainLair
    };
  }
  bool isInUnitTypeGroup(UnitTypeGroup::Enum grp, BWAPI::UnitType typ);
}
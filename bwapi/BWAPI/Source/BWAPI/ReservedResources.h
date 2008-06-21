#pragma once

#include <Util/Types.h>

namespace BWAPI
{
  class ReservedResources
  {
    public :
     ReservedResources();
     ReservedResources(u16 minerals, u16 gas, u16 supply);
     void operator += (const ReservedResources& reserved);
     void clear();
     u16 minerals, gas;
     u16 supply; /** I will count supply just for the 'actual' race */
     u16 reservedIncome; /** Time of reserved income. */
  };
}
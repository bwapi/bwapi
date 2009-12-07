#pragma once
#include <BW/UnitType.h>

#include "UpgradeID.h"
#include "RaceID.h"

namespace BW
{
  /** Mapping (and interface) of bw's upgrade representation. */
  class UpgradeType
  {
    public :
      UpgradeType();
      UpgradeType(const u8& id);

      bool        operator == (const u8& id) const;
      u8          getID() const;
      const char* getName() const;
      bool        isValid() const;
      u8          upgradesMax() const;
      u16         mineralCostBase() const;
      u16         mineralCostFactor() const;
      u16         gasCostBase() const;
      u16         gasCostFactor() const;
      u16         timeCostBase() const;
      u16         timeCostFactor() const;
      u8          race() const;
      /** Our internal way to determine what building should be used to do certain upgrade . */
      BW::UnitType whereToUpgrade();
    private :
      u8 id;
  };
};


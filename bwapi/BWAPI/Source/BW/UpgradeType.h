#pragma once

#include <BW/UnitType.h>

#include "UpgradeID.h"
#include "Race.h"

namespace BW
{
  /** Mapping (and interface) of bw's upgrade representation. */
  class UpgradeType
  {
    public :
      UpgradeType();
      UpgradeType(const UpgradeID::Enum& id);
      
      bool            operator == (const UpgradeID::Enum& id) const;
      UpgradeID::Enum getID() const;
      const char*     getName() const;
      bool            isValid() const;
      u8              upgradesMax() const;
      u16             mineralCostBase() const;
      u16             mineralCostFactor() const;
      u16             gasCostBase() const;
      u16             gasCostFactor() const;
      u16             timeCostBase() const;
      u16             timeCostFactor() const;
      Race::Enum      race() const;
      /** Our internal way to determine what building should be used to do certain upgrade . */
      BW::UnitType whereToUpgrade();
    private :
      UpgradeID::Enum id;
  };
}
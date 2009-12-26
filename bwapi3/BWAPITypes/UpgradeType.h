#pragma once
#include "RaceId.h"
#include "UnitTypeId.h"

#include <string>
#include <set>

namespace BWAPI
{
  struct UpgradeType
  {
    int id;
    const char* name;
    int mineralPriceBase;
    int mineralPriceFactor;
    int gasPriceBase;
    int gasPriceFactor;
    int upgradeTimeBase;
    int upgradeTimeFactor;
    UnitTypeId whatUpgrades;
    RaceId race;
    int maxRepeats;
    bool valid;
  };
}

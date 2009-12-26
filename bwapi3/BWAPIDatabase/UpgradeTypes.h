#pragma once
#include <string>
#include <set>
#include <BWAPITypes\Race.h>
#include <BWAPITypes\UpgradeType.h>
namespace BWAPI
{
  class UpgradeTypeInternal : public UpgradeType
  {
    public:
      std::set<const BWAPI::UnitType*> whatUses;
      bool valid;
  };
  namespace UpgradeTypes
  {
    UpgradeType getUpgradeType(std::string& name);
    std::set<UpgradeTypeId>& allUpgradeTypes();
    void init();
  }
}

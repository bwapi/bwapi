#pragma once
#include <string>
#include <set>
#include <map>

#include <BWAPITypes\Race.h>
#include <BWAPITypes\UpgradeType.h>
#include <BWAPITypes\UpgradeTypeId.h>
namespace BWAPI
{
  class UpgradeTypeInternal : public UpgradeType
  {
    public:
      std::set<UnitTypeId> whatUses;
      bool valid;
  };
  extern UpgradeTypeInternal upgradeTypeData[UpgradeTypeIds::count];
  extern std::map<std::string, UpgradeTypeId> upgradeTypeMap;
  extern std::set< UpgradeTypeId > upgradeTypeSet;
  namespace UpgradeTypes
  {
    UpgradeTypeId getIdByName(const std::string& name);
    void init();
  }
}

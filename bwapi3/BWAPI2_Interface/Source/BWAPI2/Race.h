#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class UnitType;
  class Race
  {
    public:
      Race();
      Race(int id);
      Race(const Race& other);
      Race& operator=(const Race& other);
      bool operator==(const Race& other) const;
      bool operator!=(const Race& other) const;
      bool operator<(const Race& other) const;
      int getID() const;
      std::string getName() const;
      const UnitType* getWorker() const;
      const UnitType* getCenter() const;
      const UnitType* getRefinery() const;
      const UnitType* getTransport() const;
      const UnitType* getSupplyProvider() const;
    private:
      int id;
  };
  namespace Races
  {
    Race getRace(std::string& name);
    std::set<Race>& allRaces();
    void init();
    BWAPI2_EXPORT extern const Race Zerg;
    BWAPI2_EXPORT extern const Race Terran;
    BWAPI2_EXPORT extern const Race Protoss;
    BWAPI2_EXPORT extern const Race Random;
    BWAPI2_EXPORT extern const Race Other;
    BWAPI2_EXPORT extern const Race None;
    BWAPI2_EXPORT extern const Race Unknown;
  }
}

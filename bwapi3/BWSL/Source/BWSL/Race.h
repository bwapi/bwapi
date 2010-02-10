#pragma once

#include "BWSLInterface.h"

#include <string>
#include <set>

namespace BWSL
{
  class UnitType;
  class BWSL_EXPORT Race
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
    BWSL_EXPORT Race getRace(std::string& name);
    BWSL_EXPORT std::set<Race>& allRaces();
    void init();
    BWSL_EXPORT extern const Race Zerg;
    BWSL_EXPORT extern const Race Terran;
    BWSL_EXPORT extern const Race Protoss;
    BWSL_EXPORT extern const Race Random;
    BWSL_EXPORT extern const Race Other;
    BWSL_EXPORT extern const Race None;
    BWSL_EXPORT extern const Race Unknown;
  }
}

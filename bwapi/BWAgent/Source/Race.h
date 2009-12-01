#pragma once
#include <string>
#include <set>
namespace BWAgent
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
    extern const Race Zerg;
    extern const Race Terran;
    extern const Race Protoss;
    extern const Race Random;
    extern const Race Other;
    extern const Race None;
    extern const Race Unknown;
  }
}

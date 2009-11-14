#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class UnitType;
  class Race
  {
    public:
      Race();
      Race(int id);
      Race(const Race& other);
      Race& __cdecl operator=(const Race& other);
      bool __cdecl operator==(const Race& other) const;
      bool __cdecl operator!=(const Race& other) const;
      bool __cdecl operator<(const Race& other) const;
      int __cdecl getID() const;
      std::string __cdecl getName() const;
      const UnitType* __cdecl getWorker() const;
      const UnitType* __cdecl getCenter() const;
      const UnitType* __cdecl getRefinery() const;
      const UnitType* __cdecl getTransport() const;
      const UnitType* __cdecl getSupplyProvider() const;
    private:
      int id;
  };
  namespace Races
  {
    Race __cdecl getRace(std::string& name);
    std::set<Race>& __cdecl allRaces();
    void __cdecl init();
    extern const Race Zerg;
    extern const Race Terran;
    extern const Race Protoss;
    extern const Race Random;
    extern const Race Other;
    extern const Race None;
    extern const Race Unknown;
  }
}

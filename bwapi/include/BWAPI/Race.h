#pragma once
#include <string>
namespace BWAPI
{
  class Race
  {
  public:
    Race();
    Race(int id);
    Race(const Race &other);
    Race& operator=(const Race &other);
    bool operator==(const Race &other) const;
    bool operator<(const Race &other) const;
    std::string getName() const;
    int id;
  };
  namespace Races
  {
    Race getRace(std::string &name);
    std::set<Race>& allRaces();
    extern const Race Zerg;
    extern const Race Terran;
    extern const Race Protoss;
    extern const Race Random;
    extern const Race Other;
    extern const Race None;
    extern const Race Unknown;
  }
}

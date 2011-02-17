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
      Race& operator=(const Race& other);
      operator int() const;

      /** Returns a unique ID for this race. */
      int getID() const;

      /** Returns the name of the race. For example Races::Terran.getName() will return a std::string object
       * containing "Terran". */
      std::string getName() const;

      /** Returns the worker unit type for the given race. For example Races::Protoss.getWorker() will return
       * a pointer to UnitTypes::Protoss_Probe. */
      UnitType getWorker() const;

      /** Returns the center unit type for the given race. For example Races::Terran.getCenter() will return a
       * pointer to UnitTypes::Terran_Command_Center. While there are three center types for Zerg
       * (Hatchery, Lair, and Hive), Races::Zerg.getCenter() will only return a pointer to
       * UnitTypes::Zerg_Hatchery, since it is the unit type needed to make a new center. */
      UnitType getCenter() const;

      /** Returns the refinery unit type for the given race. For example: Races::Zerg.getRefinery() will
       * return a pointer to UnitTypes::Zerg_Extractor?. */
      UnitType getRefinery() const;

      /** Returns the transport unit type for the given race. For example: Races::Protoss.getTransport() will
       * return a pointer to UnitTypes::Protoss_Shuttle. */
      UnitType getTransport() const;

      /** Returns the main supply provider unit type for the given race. For example:
       * Races::Terran.getSupplyProvider() will return a pointer to UnitTypes::Terran_Supply_Depot?. */
      UnitType getSupplyProvider() const;
    private:
      int id;
  };
  namespace Races
  {
    /** Given the name of a race, this function will return the race type. For example:
     * Races::getRace("Zerg") will return Races::Zerg. */
    Race getRace(std::string name);

    /** Returns the set of all the races, which are listed below. */
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

#pragma once
#include <set>
#include <string>

#include <BWAPI/TilePosition.h>
#include <BWAPI/Race.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class Unit;
  class Force;
  class Player
  {
    public :
      virtual int getID() const = 0;
      virtual std::string getName() const = 0;
      virtual const std::set<Unit*>& getUnits() const = 0;
      virtual Race getRace() const = 0;
      virtual PlayerType playerType() const = 0;
      virtual Force* getForce() const = 0;
      virtual bool isAlly(Player* player) const = 0;
      virtual bool isEnemy(Player* player) const = 0;
      virtual bool isNeutral() const = 0;
      virtual TilePosition getStartLocation() const = 0;
      virtual bool isVictorious() const = 0;
      virtual bool isDefeated() const = 0;
      virtual bool leftGame() const = 0;

      virtual int minerals() const = 0;
      virtual int gas() const = 0;
      virtual int cumulativeMinerals() const = 0;
      virtual int cumulativeGas() const = 0;

      virtual int supplyTotal() const = 0;
      virtual int supplyUsed() const = 0;
      virtual int supplyTotal(Race race) const = 0;
      virtual int supplyUsed(Race race) const = 0;

      virtual int allUnitCount(UnitType unit) const = 0;
      virtual int completedUnitCount(UnitType unit) const = 0;
      virtual int incompleteUnitCount(UnitType unit) const = 0;
      virtual int deadUnitCount(UnitType unit) const = 0;
      virtual int killedUnitCount(UnitType unit) const = 0;

      virtual int  getUpgradeLevel(UpgradeType upgrade) const = 0;
      virtual bool hasResearched(TechType tech) const = 0;
      virtual bool isResearching(TechType tech) const = 0;
      virtual bool isUpgrading(UpgradeType upgrade) const = 0;

  };
};

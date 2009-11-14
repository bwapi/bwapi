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
      virtual int __cdecl getID() const = 0;
      virtual std::string __cdecl getName() const = 0;
      virtual std::set<Unit*> __cdecl getUnits() = 0;
      virtual Race __cdecl getRace() const = 0;
      virtual PlayerType __cdecl playerType() const = 0;
      virtual Force* __cdecl getForce() const = 0;
      virtual bool __cdecl isAlly(Player* player) const = 0;
      virtual bool __cdecl isEnemy(Player* player) const = 0;
      virtual bool __cdecl isNeutral() const = 0;
      virtual TilePosition __cdecl getStartLocation() const = 0;

      virtual int __cdecl minerals() const = 0;
      virtual int __cdecl gas() const = 0;
      virtual int __cdecl cumulativeMinerals() const = 0;
      virtual int __cdecl cumulativeGas() const = 0;

      virtual int __cdecl supplyTotal() const = 0;
      virtual int __cdecl supplyUsed() const = 0;
      virtual int __cdecl supplyTotal(Race race) const = 0;
      virtual int __cdecl supplyUsed(Race race) const = 0;

      virtual int __cdecl allUnitCount(UnitType unit) const = 0;
      virtual int __cdecl completedUnitCount(UnitType unit) const = 0;
      virtual int __cdecl incompleteUnitCount(UnitType unit) const = 0;
      virtual int __cdecl deadUnitCount(UnitType unit) const = 0;
      virtual int __cdecl killedUnitCount(UnitType unit) const = 0;

      virtual bool __cdecl researching(TechType tech) const = 0;
      virtual bool __cdecl researched(TechType tech) const = 0;
      virtual bool __cdecl upgrading(UpgradeType upgrade) const = 0;
      virtual int __cdecl upgradeLevel(UpgradeType upgrade) const = 0;

  };
};

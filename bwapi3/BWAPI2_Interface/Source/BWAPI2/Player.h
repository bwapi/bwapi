#pragma once
#include "BWAPI2Interface.h"

IP_STRIP

#include "Order.h"
#include "TechType.h"
#include "UpgradeType.h"
#include "UnitType.h"
#include "Position.h"
#include "TilePosition.h"
#include "Race.h"
#include "TechType.h"
#include "UpgradeType.h"
#include "PlayerType.h"

#include <set>
#include <string>
#include <BWAPI\all.h>

namespace BWAPI2
{
  class Unit;
  class Force;
  class Player
  {
  private:
    int id;
  public:
    Player(int id);
    BWAPI2_EXPORT int getID() const;
    BWAPI2_EXPORT std::string getName() const;
    BWAPI2_EXPORT std::set<Unit*>& getUnits() const;
    BWAPI2_EXPORT Race getRace() const;
    BWAPI2_EXPORT PlayerType playerType() const;
    BWAPI2_EXPORT Force* getForce() const;
    BWAPI2_EXPORT bool isAlly(Player* player) const;
    BWAPI2_EXPORT bool isEnemy(Player* player) const;
    BWAPI2_EXPORT bool isNeutral() const;
    BWAPI2_EXPORT TilePosition getStartLocation() const;
    BWAPI2_EXPORT bool isVictorious() const;
    BWAPI2_EXPORT bool isDefeated() const;
    BWAPI2_EXPORT bool leftGame() const;

    BWAPI2_EXPORT int minerals() const;
    BWAPI2_EXPORT int gas() const;
    BWAPI2_EXPORT int cumulativeMinerals() const;
    BWAPI2_EXPORT int cumulativeGas() const;

    BWAPI2_EXPORT int supplyTotal() const;
    BWAPI2_EXPORT int supplyUsed() const;
    BWAPI2_EXPORT int supplyTotal(Race race) const;
    BWAPI2_EXPORT int supplyUsed(Race race) const;

    BWAPI2_EXPORT int allUnitCount(UnitType unit) const;
    BWAPI2_EXPORT int completedUnitCount(UnitType unit) const;
    BWAPI2_EXPORT int incompleteUnitCount(UnitType unit) const;
    BWAPI2_EXPORT int deadUnitCount(UnitType unit) const;
    BWAPI2_EXPORT int killedUnitCount(UnitType unit) const;

    BWAPI2_EXPORT int  getUpgradeLevel(UpgradeType upgrade) const;
    BWAPI2_EXPORT bool hasResearched(TechType tech) const;
    BWAPI2_EXPORT bool isResearching(TechType tech) const;
    BWAPI2_EXPORT bool isUpgrading(UpgradeType upgrade) const;
  };
};

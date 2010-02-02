#pragma once
#include "BWSLInterface.h"

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

namespace BWSL
{
  class Unit;
  class Force;
  class Player
  {
  private:
    int id;
    const BWAPI::Player* self;
  public:
    Player(int id);
    BWSL_EXPORT int getID() const;
    BWSL_EXPORT std::string getName() const;
    BWSL_EXPORT std::set<Unit*>& getUnits() const;
    BWSL_EXPORT Race getRace() const;
    BWSL_EXPORT PlayerType playerType() const;
    BWSL_EXPORT Force* getForce() const;
    BWSL_EXPORT bool isAlly(Player* player) const;
    BWSL_EXPORT bool isEnemy(Player* player) const;
    BWSL_EXPORT bool isNeutral() const;
    BWSL_EXPORT TilePosition getStartLocation() const;
    BWSL_EXPORT bool isVictorious() const;
    BWSL_EXPORT bool isDefeated() const;
    BWSL_EXPORT bool leftGame() const;

    BWSL_EXPORT int minerals() const;
    BWSL_EXPORT int gas() const;
    BWSL_EXPORT int cumulativeMinerals() const;
    BWSL_EXPORT int cumulativeGas() const;

    BWSL_EXPORT int supplyTotal() const;
    BWSL_EXPORT int supplyUsed() const;
    BWSL_EXPORT int supplyTotal(Race race) const;
    BWSL_EXPORT int supplyUsed(Race race) const;

    BWSL_EXPORT int allUnitCount(UnitType unit) const;
    BWSL_EXPORT int completedUnitCount(UnitType unit) const;
    BWSL_EXPORT int incompleteUnitCount(UnitType unit) const;
    BWSL_EXPORT int deadUnitCount(UnitType unit) const;
    BWSL_EXPORT int killedUnitCount(UnitType unit) const;

    BWSL_EXPORT int  getUpgradeLevel(UpgradeType upgrade) const;
    BWSL_EXPORT bool hasResearched(TechType tech) const;
    BWSL_EXPORT bool isResearching(TechType tech) const;
    BWSL_EXPORT bool isUpgrading(UpgradeType upgrade) const;
  };
};

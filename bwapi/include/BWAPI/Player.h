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
    virtual std::string getName() const=0;
    virtual std::set<Unit*> getUnits()=0;
    virtual Race getRace() const=0;
    virtual PlayerType playerType() const=0;
    virtual Force* getForce() const=0;
    virtual int getID() const=0;
    virtual TilePosition getStartLocation() const=0;

    virtual int minerals() const=0;
    virtual int gas() const=0;
    virtual int supplyTotal() const=0;
    virtual int supplyUsed() const=0;

    virtual int getAllUnits(UnitType unit)=0;
    virtual int getCompletedUnits(UnitType unit)=0;
    virtual int getIncompleteUnits(UnitType unit)=0;
    virtual int getDeaths(UnitType unit)=0;
    virtual int getKills(UnitType unit)=0;

    virtual bool researching(TechType tech) const=0;
    virtual bool researched(TechType tech) const=0;
    virtual bool upgrading(UpgradeType upgrade) const=0;
    virtual int upgradeLevel(UpgradeType upgrade) const=0;

  };
};

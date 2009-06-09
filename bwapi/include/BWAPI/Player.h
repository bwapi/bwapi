#pragma once
#include <set>
#include <string>

#include <BWAPI/TechType.h>
#include <BWAPI/Race.h>
#include <BW/UpgradeType.h>
#include <BW/UnitType.h>

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
    virtual BW::PlayerType::Enum playerType() const=0;
    virtual Force* getForce() const=0;
    virtual u8 getID() const=0;

    virtual int minerals() const=0;
    virtual int gas() const=0;
    virtual int supplyTotal() const=0;
    virtual int supplyUsed() const=0;

    virtual int getAllUnits(BW::UnitType unit)=0;
    virtual int getCompletedUnits(BW::UnitType unit)=0;
    virtual int getCompletedUnits(BW::UnitType unit, Race race)=0;
    virtual int getIncompleteUnits(BW::UnitType unit)=0;
    virtual int getDeaths(BW::UnitType unit)=0;
    virtual int getKills(BW::UnitType unit)=0;

    virtual bool researching(TechType tech) const=0;
    virtual bool researched(TechType tech) const=0;
    virtual bool upgrading(BW::UpgradeType upgrade) const=0;
    virtual int upgradeLevel(BW::UpgradeType upgrade) const=0;

  };
};

#include "PlayerImpl.h"

#include <BWAPI/Color.h>
#include <BWAPI/WeaponType.h>

namespace BWAPI
{
  //--------------------------------------------- GET ID -----------------------------------------------------
  int PlayerImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET UNITS --------------------------------------------------
  const Unitset &PlayerImpl::getUnits() const
  {
    return this->units;
  }
  //--------------------------------------------- MINERALS ---------------------------------------------------
  int PlayerImpl::minerals() const
  {
    return data.minerals;
  }
  //--------------------------------------------- GAS --------------------------------------------------------
  int PlayerImpl::gas() const
  {
    return data.gas;
  }
  //--------------------------------------------- GATHERED MINERALS ------------------------------------------
  int PlayerImpl::gatheredMinerals() const
  {
    return data.gatheredMinerals;
  }
  //--------------------------------------------- GATHERED GAS -----------------------------------------------
  int PlayerImpl::gatheredGas() const
  {
    return data.gatheredGas;
  }
  //--------------------------------------------- REPAIRED MINERALS ------------------------------------------
  int PlayerImpl::repairedMinerals() const
  {
    return data.repairedMinerals;
  }
  //--------------------------------------------- REPAIRED GAS -----------------------------------------------
  int PlayerImpl::repairedGas() const
  {
    return data.repairedGas;
  }
  //--------------------------------------------- REFUNDED MINERALS ------------------------------------------
  int PlayerImpl::refundedMinerals() const
  {
    return data.refundedMinerals;
  }
  //--------------------------------------------- REFUNDED GAS -----------------------------------------------
  int PlayerImpl::refundedGas() const
  {
    return data.refundedGas;
  }
  //--------------------------------------------- SPENT MINERALS ---------------------------------------------
  int PlayerImpl::spentMinerals() const
  {
    return data.gatheredMinerals + data.refundedMinerals - data.minerals - data.repairedMinerals;
  }
  //--------------------------------------------- SPENT GAS --------------------------------------------------
  int PlayerImpl::spentGas() const
  {
    return data.gatheredGas + data.refundedGas - data.gas - data.repairedGas;
  }
  //--------------------------------------------- SUPPLY TOTAL -----------------------------------------------
  int PlayerImpl::supplyTotal(Race race) const
  {
    if ( race == Races::None )  // Get current race's supply if None is specified
      race = this->getRace();

    if (static_cast<unsigned>(race) < std::extent<decltype(data.supplyTotal)>::value)
      return data.supplyTotal[race];
    return 0;
  }
  //--------------------------------------------- SUPPLY USED ------------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    if ( race == Races::None )  // Get current race's supply if None is specified
      race = this->getRace();

    if (static_cast<unsigned>(race) < std::extent<decltype(data.supplyUsed)>::value)
      return data.supplyUsed[race];
    return 0;
  }
  //--------------------------------------------- ALL UNIT COUNT ---------------------------------------------
  int PlayerImpl::allUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? data.allUnitCount[unit] : 0;
  }
  //--------------------------------------------- VISIBLE UNIT COUNT -----------------------------------------
  int PlayerImpl::visibleUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? data.visibleUnitCount[unit] : 0;
  }
  //--------------------------------------------- COMPLETED UNIT COUNT ---------------------------------------
  int PlayerImpl::completedUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? data.completedUnitCount[unit] : 0;
  }
  //--------------------------------------------- DEAD UNIT COUNT --------------------------------------------
  int PlayerImpl::deadUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? data.deadUnitCount[unit] : 0;
  }
  //--------------------------------------------- KILLED UNIT COUNT ------------------------------------------
  int PlayerImpl::killedUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? data.killedUnitCount[unit] : 0;
  }
  //--------------------------------------------------- SCORE ------------------------------------------------
  int PlayerImpl::getUnitScore() const
  {
    return data.totalUnitScore;
  }
  int PlayerImpl::getKillScore() const
  {
    return data.totalKillScore;
  }
  int PlayerImpl::getBuildingScore() const
  {
    return data.totalBuildingScore;
  }
  int PlayerImpl::getRazingScore() const
  {
    return data.totalRazingScore;
  }
  int PlayerImpl::getCustomScore() const
  {
    return data.customScore;
  }
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
  int PlayerImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? data.upgradeLevel[upgrade] : 0;
  }
  //--------------------------------------------- HAS RESEARCHED ---------------------------------------------
  bool PlayerImpl::hasResearched(TechType tech) const
  {
    return tech.isValid() ? data.hasResearched[tech] : false;
  }
  //--------------------------------------------- IS RESEARCHING ---------------------------------------------
  bool PlayerImpl::isResearching(TechType tech) const
  {
    return tech.isValid() ? data.isResearching[tech] : false;
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? data.isUpgrading[upgrade] : false;
  }
  //-------------------------------------------------- COLOUR ------------------------------------------------
  BWAPI::Color PlayerImpl::getColor() const
  {
    return BWAPI::Color(data.color);
  }
  //------------------------------------------------- OBSERVER -----------------------------------------------
  bool PlayerImpl::isObserver() const
  {
    return !data.isParticipating;
  }
  //------------------------------------------------ AVAILABILITY --------------------------------------------
  int PlayerImpl::getMaxUpgradeLevel(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? data.maxUpgradeLevel[upgrade] : 0;
  }
  bool PlayerImpl::isResearchAvailable(TechType tech) const
  {
    return tech.isValid() ? data.isResearchAvailable[tech] : false;
  }
  bool PlayerImpl::isUnitAvailable(UnitType unit) const
  {
    return unit.isValid() ? data.isUnitAvailable[unit] : false;
  }
  //--------------------------------------------- LEFT GAME --------------------------------------------------
  bool PlayerImpl::leftGame() const
  {
    return data.leftGame;
  }
}


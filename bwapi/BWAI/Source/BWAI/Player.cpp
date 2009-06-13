#include <BWAPI.h>

#include "Player.h"
#include "Globals.h"
#include "AI.h"
namespace BWAI
{
  Player::Player(BWAPI::Player* player)
  :player(player)
  {
    this->player=player;
  }
  u8 Player::getID()
  {
    return this->player->getID();
  }
  BWAPI::Race Player::getRace()
  {
    return this->player->getRace();
  }
  BWAPI::PlayerType Player::playerType()
  {
    return this->player->playerType();
  }
  int Player::getMinerals() const
  {
    return this->player->minerals();
  }
  int Player::getGas() const
  {
    return this->player->gas();
  }
  int Player::getSuppliesAvailable() const
  {
    return this->player->supplyTotal();
  }
  int Player::getSuppliesUsed() const
  {
    return this->player->supplyUsed();
  }
  int Player::getSuppliesFree() const
  {
    return this->player->supplyTotal()-this->player->supplyUsed();
  }
  BW::TilePosition Player::getStartLocation() const
  {
    return this->player->getStartLocation();
  }
  int Player::getAllUnits(BWAPI::UnitType unit) const
  {
    return this->player->getAllUnits(unit);
  }
  int Player::getCompletedUnits(BWAPI::UnitType unit) const
  {
    return this->player->getCompletedUnits(unit);
  }
  int Player::getIncompleteUnits(BWAPI::UnitType unit) const
  {
    return this->player->getIncompleteUnits(unit);
  }
  int Player::getDeaths(BWAPI::UnitType unit) const
  {
    return this->player->getDeaths(unit);
  }
  int Player::getKills(BWAPI::UnitType unit) const
  {
    return this->player->getKills(unit);
  }

  int Player::getAllUnits(BWAI::UnitTypeGroup::Enum group) const
  {
    int total=0;
    for(std::set<BWAPI::UnitType>::iterator i=BWAPI::UnitTypes::allUnitTypes().begin();i!=BWAPI::UnitTypes::allUnitTypes().end();i++)
    {
      if (isInUnitTypeGroup(group,*i))
      {
        total+=this->player->getAllUnits(*i);
      }
    }
    return total;
  }
  int Player::getCompletedUnits(BWAI::UnitTypeGroup::Enum group) const
  {
    int total=0;
    for(std::set<BWAPI::UnitType>::iterator i=BWAPI::UnitTypes::allUnitTypes().begin();i!=BWAPI::UnitTypes::allUnitTypes().end();i++)
    {
      if (isInUnitTypeGroup(group,*i))
      {
        total+=this->player->getCompletedUnits(*i);
      }
    }
    return total;
  }
  int Player::getIncompleteUnits(BWAI::UnitTypeGroup::Enum group) const
  {
    int total=0;
    for(std::set<BWAPI::UnitType>::iterator i=BWAPI::UnitTypes::allUnitTypes().begin();i!=BWAPI::UnitTypes::allUnitTypes().end();i++)
    {
      if (isInUnitTypeGroup(group,*i))
      {
        total+=this->player->getIncompleteUnits(*i);
      }
    }
    return total;
  }
  int Player::getDeaths(BWAI::UnitTypeGroup::Enum group) const
  {
    int total=0;
    for(std::set<BWAPI::UnitType>::iterator i=BWAPI::UnitTypes::allUnitTypes().begin();i!=BWAPI::UnitTypes::allUnitTypes().end();i++)
    {
      if (isInUnitTypeGroup(group,*i))
      {
        total+=this->player->getDeaths(*i);
      }
    }
    return total;
  }
  int Player::getKills(BWAI::UnitTypeGroup::Enum group) const
  {
    int total=0;
    for(std::set<BWAPI::UnitType>::iterator i=BWAPI::UnitTypes::allUnitTypes().begin();i!=BWAPI::UnitTypes::allUnitTypes().end();i++)
    {
      if (isInUnitTypeGroup(group,*i))
      {
        total+=this->player->getKills(*i);
      }
    }
    return total;
  }
  bool Player::researchInProgress(BWAPI::TechType tech) const
  {
    return this->player->researching(tech);
  }
  bool Player::techResearched(BWAPI::TechType tech) const
  {
    return this->player->researched(tech);
  }
  int Player::upgradeLevel(BWAPI::UpgradeType upgrade) const
  {
    return this->player->upgradeLevel(upgrade);
  }
  bool Player::upgradeInProgress(BWAPI::UpgradeType upgrade) const
  {
    return this->player->upgrading(upgrade);
  }

  //------------------------------------------------ CAN BUILD -----------------------------------------------
  bool Player::canBuild(BWAPI::UnitType unit) const
  {
    for(std::map<const BWAPI::UnitType*,int>::const_iterator i=unit.requiredUnits().begin();i!=unit.requiredUnits().end();i++)
    {
      if (this->getCompletedUnits(*(i->first))<i->second)
        return false;
    }
    BWAPI::TechType tech=*(unit.requiredTech());
    if (tech!=BWAPI::TechTypes::None && !this->techResearched(tech))
      return false;
    return true;
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BWAPI::UnitType unit) const
  {
    if (this==ai->player)
    {
      return ((int)this->getSuppliesFree()) - ai->reserved.supply   >= unit.suppliesConsumed() &&
             ((int)this->getMinerals())     - ai->reserved.minerals >= unit.mineralPrice() &&
             ((int)this->getGas())          - ai->reserved.gas      >= unit.gasPrice();
    }
    else
    {
      return ((int)this->getSuppliesFree()) >= unit.suppliesConsumed() &&
             ((int)this->getMinerals())     >= unit.mineralPrice() &&
             ((int)this->getGas())          >= unit.gasPrice();
    }
  }
  //--------------------------------------------- CAN AFFORD NOW ---------------------------------------------
  bool Player::canAffordNow(BWAPI::UnitType unit) const
  {
    return ((int)this->getSuppliesFree()) >= unit.suppliesConsumed() &&
           ((int)this->getMinerals())     >= unit.mineralPrice() &&
           ((int)this->getGas())          >= unit.gasPrice();
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BWAPI::TechType tech) const
  {
    if (this==ai->player)
    {
      return ((int)this->getMinerals()) - ai->reserved.minerals >= tech.mineralPrice() &&
             ((int)this->getGas())      - ai->reserved.gas      >= tech.gasPrice();
    }
    else
    {
      return ((int)this->getMinerals()) >= tech.mineralPrice() &&
             ((int)this->getGas())      >= tech.gasPrice();
    }
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BWAPI::UpgradeType upgrade, int level) const
  {
    if (this==ai->player)
    {
      return ((int)this->getMinerals()) - ai->reserved.minerals >= upgrade.mineralPriceBase() + upgrade.mineralPriceFactor()*(level-1) &&
             ((int)this->getGas())      - ai->reserved.gas      >= upgrade.gasPriceBase()     + upgrade.gasPriceFactor()    *(level-1);
    }
    else
    {
      return ((int)this->getMinerals()) >= upgrade.mineralPriceBase() + upgrade.mineralPriceFactor()*(level-1) &&
             ((int)this->getGas())      >= upgrade.gasPriceBase()     + upgrade.gasPriceFactor()    *(level-1);
    }
  }
}

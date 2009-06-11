#include <BWAPI.h>

#include <BW/UnitType.h>

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
  int Player::getAllUnits(BW::UnitType unit) const
  {
    return this->player->getAllUnits(unit);
  }
  int Player::getCompletedUnits(BW::UnitType unit) const
  {
    return this->player->getCompletedUnits(unit);
  }
  int Player::getCompletedUnits(BW::UnitType unit, BWAPI::Race race) const
  {
    return this->player->getCompletedUnits(unit,race);
  }
  int Player::getIncompleteUnits(BW::UnitType unit) const
  {
    return this->player->getIncompleteUnits(unit);
  }
  int Player::getDeaths(BW::UnitType unit) const
  {
    return this->player->getDeaths(unit);
  }
  int Player::getKills(BW::UnitType unit) const
  {
    return this->player->getKills(unit);
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
  bool Player::canBuild(BW::UnitType unit) const
  {
    for(std::map<BW::UnitType,int>::const_iterator i=unit.getRequiredUnits().begin();i!=unit.getRequiredUnits().end();i++)
    {
      if (this->getCompletedUnits(i->first)<i->second)
        return false;
    }
    BWAPI::TechType tech((int)unit.getRequiredTech());
    if (tech!=BWAPI::TechTypes::None && !this->techResearched(tech))
      return false;
    return true;
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::UnitType unit) const
  {
    if (this==ai->player)
    {
      return ((int)this->getSuppliesFree()) - ai->reserved.supply   >= unit.getSupplies() &&
             ((int)this->getMinerals())     - ai->reserved.minerals >= unit.getMineralPrice() &&
             ((int)this->getGas())          - ai->reserved.gas      >= unit.getGasPrice();
    }
    else
    {
      return ((int)this->getSuppliesFree()) >= unit.getSupplies() &&
             ((int)this->getMinerals())     >= unit.getMineralPrice() &&
             ((int)this->getGas())          >= unit.getGasPrice();
    }
  }
  //--------------------------------------------- CAN AFFORD NOW ---------------------------------------------
  bool Player::canAffordNow(BW::UnitType unit) const
  {
    return ((int)this->getSuppliesFree()) >= unit.getSupplies() &&
           ((int)this->getMinerals())     >= unit.getMineralPrice() &&
           ((int)this->getGas())          >= unit.getGasPrice();
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

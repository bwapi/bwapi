#include <BW/UnitType.h>
#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/PlayerType.h>
#include <BWAPI/Player.h>

#include "Player.h"
#include "Globals.h"
#include "AI.h"
namespace BWAI
{
  Player::Player(BWAPI::Player* player)
  {
    this->player=player;
  }
  BW::Race::Enum Player::getRace()
  {
    return this->player->getRace();
  }
  int Player::getAlliance(int opposingID)
  {
    return this->player->getAlliance(opposingID);
  }
  BW::PlayerType::Enum Player::getOwner()
  {
    return this->player->getOwner();
  }
  int Player::getMinerals() const
  {
    return this->player->getMinerals();
  }
  int Player::getGas() const
  {
    return this->player->getGas();
  }
  int Player::getSuppliesAvailable(BW::Race::Enum race)
  {
    return this->player->getSuppliesAvailable(race);
  }
  int Player::getSuppliesUsed(BW::Race::Enum race)
  {
    return this->player->getSuppliesUsed(race);
  }
  int Player::getSuppliesFree(BW::Race::Enum race)
  {
    return this->player->getSuppliesAvailable(race)-this->player->getSuppliesUsed(race);
  }
  int Player::getAllUnits(BW::UnitType unit) const
  {
    return this->player->getAllUnits(unit);
  }
  int Player::getCompletedUnits(BW::UnitType unit) const
  {
    return this->player->getCompletedUnits(unit);
  }
  int Player::getCompletedUnits(BW::UnitType unit, BW::Race::Enum race) const
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
  bool Player::researchInProgress(BW::TechType tech) const
  {
    return this->player->researchInProgress(tech);
  }
  bool Player::techResearched(BW::TechType tech) const
  {
    return this->player->techResearched(tech);
  }
  int Player::upgradeLevel(BW::UpgradeType upgrade) const
  {
    return this->player->upgradeLevel(upgrade);
  }
  bool Player::upgradeInProgress(BW::UpgradeType upgrade) const
  {
    return this->player->upgradeInProgress(upgrade);
  }

  //------------------------------------------------ CAN BUILD -----------------------------------------------
  bool Player::canBuild(BW::UnitType unit) const
  {
    for(std::map<BW::UnitType,int>::const_iterator i=unit.getRequiredUnits().begin();i!=unit.getRequiredUnits().end();i++)
    {
      if (this->getCompletedUnits(i->first)<i->second)
        return false;
    }
    BW::TechType tech=unit.getRequiredTech();
    if (tech!=BW::TechID::None && !this->techResearched(tech))
      return false;
    return true;
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::UnitType unit) const
  {
    if (this==ai->player)
    {
      return ((int)this->player->getSuppliesFree(unit.getRace())) - ai->reserved.supply   >= unit.getSupplies() &&
             ((int)this->player->getMinerals())                   - ai->reserved.minerals >= unit.getMineralPrice() &&
             ((int)this->player->getGas())                        - ai->reserved.gas      >= unit.getGasPrice();
    }
    else
    {
      return ((int)this->player->getSuppliesFree(unit.getRace())) >= unit.getSupplies() &&
             ((int)this->player->getMinerals())                   >= unit.getMineralPrice() &&
             ((int)this->player->getGas())                        >= unit.getGasPrice();
    }
  }
  //--------------------------------------------- CAN AFFORD NOW ---------------------------------------------
  bool Player::canAffordNow(BW::UnitType unit) const
  {
    return ((int)this->player->getSuppliesFree(unit.getRace())) >= unit.getSupplies() &&
           ((int)this->player->getMinerals())                   >= unit.getMineralPrice() &&
           ((int)this->player->getGas())                        >= unit.getGasPrice();
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::TechType tech) const
  {
    if (this==ai->player)
    {
      return ((int)this->player->getMinerals()) - ai->reserved.minerals >= tech.getMineralPrice() &&
             ((int)this->player->getGas())      - ai->reserved.gas      >= tech.getGasPrice();
    }
    else
    {
      return ((int)this->player->getMinerals()) >= tech.getMineralPrice() &&
             ((int)this->player->getGas())      >= tech.getGasPrice();
    }
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::UpgradeType upgrade, int level) const
  {
    if (this==ai->player)
    {
      return ((int)this->player->getMinerals()) - ai->reserved.minerals >= upgrade.mineralCostBase() + upgrade.mineralCostFactor()*(level-1) &&
             ((int)this->player->getGas())      - ai->reserved.gas      >= upgrade.gasCostBase()     + upgrade.gasCostFactor()    *(level-1);
    }
    else
    {
      return ((int)this->player->getMinerals()) >= upgrade.mineralCostBase() + upgrade.mineralCostFactor()*(level-1) &&
             ((int)this->player->getGas())      >= upgrade.gasCostBase()     + upgrade.gasCostFactor()    *(level-1);
    }
  }
}

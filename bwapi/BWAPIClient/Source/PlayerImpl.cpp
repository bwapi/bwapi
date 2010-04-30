#include <BWAPI.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"

namespace BWAPI
{
  PlayerImpl::PlayerImpl(int id)
  {
    this->id = id;
    this->self = &(BWAPI::Client::gameData->players[id]);
    this->leftTheGame = false;
  }
  void PlayerImpl::update()
  {
    if (playerType() == PlayerTypes::HumanDefeated ||
        playerType() == PlayerTypes::Computer ||
       (playerType() == PlayerTypes::Neutral && !this->isNeutral()))
    {
      leftTheGame = true;
    }
  }
  int PlayerImpl::getID() const
  {
    return id;
  }
  std::string PlayerImpl::getName() const
  {
    if (id==11)
      return "Neutral";
    return std::string(self->name);
  }
  const std::set<Unit*>& PlayerImpl::getUnits() const
  {
    return units;
  }
  Race PlayerImpl::getRace() const
  {
    if (id==11)
      return Races::None;
    return Race(self->race);
  }
  PlayerType PlayerImpl::playerType() const
  {
    if (id==11)
      return PlayerTypes::Neutral;
    return PlayerType(self->type);
  }
  Force* PlayerImpl::getForce() const
  {
    return BroodwarImpl.getForce(self->force);
  }
  bool PlayerImpl::isAlly(Player* player) const
  {
    if (this->isNeutral() || player->isNeutral()) return false;
    return self->alliance[player->getID()];
  }
  bool PlayerImpl::isEnemy(Player* player) const
  {
    if (this->isNeutral() || player->isNeutral()) return false;
    return !this->isAlly(player);
  }
  bool PlayerImpl::isNeutral() const
  {
    return id == 11;
  }
  TilePosition PlayerImpl::getStartLocation() const
  {
    if (isNeutral())
      return TilePositions::None;
    return TilePosition(self->startLocationX,self->startLocationY);
  }
  bool PlayerImpl::isVictorious() const
  {
    return self->isVictorious;
  }
  bool PlayerImpl::isDefeated() const
  {
    return self->isDefeated;
  }
  bool PlayerImpl::leftGame() const
  {
    return leftTheGame;
  }
  int PlayerImpl::minerals() const
  {
    return self->minerals;
  }
  int PlayerImpl::gas() const
  {
    return self->gas;
  }
  int PlayerImpl::cumulativeMinerals() const
  {
    return self->cumulativeMinerals;
  }
  int PlayerImpl::cumulativeGas() const
  {
    return self->cumulativeGas;
  }
  int PlayerImpl::supplyTotal() const
  {
    return self->suppliesAvailable[self->race];
  }
  int PlayerImpl::supplyUsed() const
  {
    return self->suppliesUsed[self->race];
  }
  int PlayerImpl::supplyTotal(Race race) const
  {
    return self->suppliesAvailable[race.getID()];
  }
  int PlayerImpl::supplyUsed(Race race) const
  {
    return self->suppliesUsed[race.getID()];
  }
  int PlayerImpl::allUnitCount(UnitType unit) const
  {
    return self->allUnitCount[unit.getID()];
  }
  int PlayerImpl::completedUnitCount(UnitType unit) const
  {
    return self->completedUnitCount[unit.getID()];
  }
  int PlayerImpl::incompleteUnitCount(UnitType unit) const
  {
    return self->allUnitCount[unit.getID()]-self->completedUnitCount[unit.getID()];
  }
  int PlayerImpl::deadUnitCount(UnitType unit) const
  {
    return self->deadUnitCount[unit.getID()];
  }
  int PlayerImpl::killedUnitCount(UnitType unit) const
  {
    return self->killedUnitCount[unit.getID()];
  }
  int PlayerImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    return self->upgradeLevel[upgrade.getID()];
  }
  bool PlayerImpl::hasResearched(TechType tech) const
  {
    return self->hasResearched[tech.getID()];
  }
  bool PlayerImpl::isResearching(TechType tech) const
  {
    return self->isResearching[tech.getID()];
  }
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    return self->isUpgrading[upgrade.getID()];
  }
}
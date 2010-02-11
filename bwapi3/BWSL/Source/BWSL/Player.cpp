#include "Player.h"
#include "Constants.h"
#include <Util\Version.h>

#include <BWAPI\all.h>
#include "Game.h"
#include <string>

namespace BWSL
{
  Player::Player(int id)
  {
    this->id = id;
    this->self = &(BWAPI::getStaticGameData()->players[id]);
    this->leftTheGame = false;
  }
  void Player::update()
  {
    if (playerType() == PlayerTypes::HumanDefeated ||
        playerType() == PlayerTypes::Computer ||
       (playerType() == PlayerTypes::Neutral && !this->isNeutral()))
    {
      leftTheGame = true;
    }
  }
  int Player::getID() const
  {
    return id;
  }
  std::string Player::getName() const
  {
    if (id==11)
      return "Neutral";
    return std::string(BWAPI::getPlayer(id)->name);
  }
  std::set<Unit*>& Player::getUnits() const
  {
    return Game::getPlayerUnits(this);
  }
  Race Player::getRace() const
  {
    if (id==11)
      return Races::None;
    return Race(self->race);
  }
  PlayerType Player::playerType() const
  {
    return PlayerType(self->type);
  }
  Force* Player::getForce() const
  {
    return Game::getForce(self->force);
  }
  bool Player::isAlly(Player* player) const
  {
    if (this->isNeutral() || player->isNeutral()) return false;
    return self->alliance[player->id];
  }
  bool Player::isEnemy(Player* player) const
  {
    if (this->isNeutral() || player->isNeutral()) return false;
    return !this->isAlly(player);
  }
  bool Player::isNeutral() const
  {
    return id == 11;
  }
  TilePosition Player::getStartLocation() const
  {
    if (self->startLocation == BWAPI::Position::Invalid)
    {
      if (isNeutral())
        return TilePositions::None;
      else
        return TilePositions::Unknown;
    }
    return TilePosition((int)((self->startLocation.x - BWSL::TILE_SIZE * 2) / BWSL::TILE_SIZE),
                               (int)((self->startLocation.y - (int)(BWSL::TILE_SIZE * 1.5)) / BWSL::TILE_SIZE));
  }
  bool Player::isVictorious() const
  {
    return self->isVictorious;
  }
  bool Player::isDefeated() const
  {
    return self->isDefeated;
  }
  bool Player::leftGame() const
  {
    return leftTheGame;
  }
  int Player::minerals() const
  {
    return self->minerals;
  }
  int Player::gas() const
  {
    return self->gas;
  }
  int Player::cumulativeMinerals() const
  {
    return self->cumulativeMinerals;
  }
  int Player::cumulativeGas() const
  {
    return self->cumulativeGas;
  }
  int Player::supplyTotal() const
  {
    return self->suppliesAvailable[self->race];
  }
  int Player::supplyUsed() const
  {
    return self->suppliesUsed[self->race];
  }
  int Player::supplyTotal(Race race) const
  {
    return self->suppliesAvailable[race.getID()];
  }
  int Player::supplyUsed(Race race) const
  {
    return self->suppliesUsed[race.getID()];
  }
  int Player::allUnitCount(UnitType unit) const
  {
    return self->allUnitCount[unit.getID()];
  }
  int Player::completedUnitCount(UnitType unit) const
  {
    return self->completedUnitCount[unit.getID()];
  }
  int Player::incompleteUnitCount(UnitType unit) const
  {
    return self->allUnitCount[unit.getID()]-self->completedUnitCount[unit.getID()];
  }
  int Player::deadUnitCount(UnitType unit) const
  {
    return self->deadUnitCount[unit.getID()];
  }
  int Player::killedUnitCount(UnitType unit) const
  {
    return self->killedUnitCount[unit.getID()];
  }
  int Player::getUpgradeLevel(UpgradeType upgrade) const
  {
    return self->upgradeLevel[upgrade.getID()];
  }
  bool Player::hasResearched(TechType tech) const
  {
    return self->hasResearched[tech.getID()];
  }
  bool Player::isResearching(TechType tech) const
  {
    return self->isResearching[tech.getID()];
  }
  bool Player::isUpgrading(UpgradeType upgrade) const
  {
    return self->isUpgrading[upgrade.getID()];
  }
}
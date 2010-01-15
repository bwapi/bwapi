#include "Player.h"

#include <Util\Version.h>

#include <BWAPI\all.h>
#include "Game.h"
#include <string>

namespace BWAPI2
{
  Player::Player(int id)
  {
    this->id=id;
  }
  int Player::getID() const
  {
    return id;
  }
  std::string Player::getName() const
  {
    return std::string(BWAPI::getPlayer(id)->name);
  }
  std::set<Unit*>& Player::getUnits() const
  {
    return Game::getPlayerUnits(this);
  }
  Race Player::getRace() const
  {
    return Races::None;//Race(BWAPI::GetStaticGameData()->players.data[id].race);
  }
  PlayerType Player::playerType() const
  {
    return PlayerType(0);//PlayerType(BWAPI::GetStaticGameData()->players.data[id].type);
  }
  Force* Player::getForce() const
  {
    return Game::getForce(BWAPI::getStaticGameData()->players[id].force);
  }
  bool Player::isAlly(Player* player) const
  {
    return false;//BWAPI::GetStaticGameData()->players.data[id].isAlly[player->id];
  }
  bool Player::isEnemy(Player* player) const
  {
    return false;//BWAPI::GetStaticGameData()->players.data[id].isEnemy[player->id];
  }
  bool Player::isNeutral() const
  {
    return id == 11;
  }
  TilePosition Player::getStartLocation() const
  {
    return TilePositions::Unknown;
  }
  bool Player::isVictorious() const
  {
    return false;//BWAPI::GetStaticGameData()->players.data[id].isVictorious;
  }
  bool Player::isDefeated() const
  {
    return false;//BWAPI::GetStaticGameData()->players.data[id].isDefeated;
  }
  bool Player::leftGame() const
  {
    return false;//BWAPI::GetStaticGameData()->players.data[id].leftGame;
  }
  int Player::minerals() const
  {
    return BWAPI::getStaticGameData()->players[id].minerals;
  }
  int Player::gas() const
  {
    return BWAPI::getStaticGameData()->players[id].gas;
  }
  int Player::cumulativeMinerals() const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].cumulativeMinerals;
  }
  int Player::cumulativeGas() const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].cumulativeGas;
  }
  int Player::supplyTotal() const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].supplyTotal[getRace().getID()];
  }
  int Player::supplyUsed() const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].supplyUsed[getRace().getID()];
  }
  int Player::supplyTotal(Race race) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].supplyTotal[race.getID()];
  }
  int Player::supplyUsed(Race race) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].supplyUsed[race.getID()];
  }
  int Player::allUnitCount(UnitType unit) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].allUnitCount[unit.getID()];
  }
  int Player::completedUnitCount(UnitType unit) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].completedUnitCount[unit.getID()];
  }
  int Player::incompleteUnitCount(UnitType unit) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].incompleteUnitCount[unit.getID()];
  }
  int Player::deadUnitCount(UnitType unit) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].deadUnitCount[unit.getID()];
  }
  int Player::killedUnitCount(UnitType unit) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].killedUnitCount[unit.getID()];
  }
  int Player::getUpgradeLevel(UpgradeType upgrade) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].upgradeLevel[upgrade.getID()];
  }
  bool Player::hasResearched(TechType tech) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].hasResearched[tech.getID()];
  }
  bool Player::isResearching(TechType tech) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].isResearching[tech.getID()];
  }
  bool Player::isUpgrading(UpgradeType upgrade) const
  {
    return 0;//BWAPI::GetStaticGameData()->players.data[id].isUpgrading[upgrade.getID()];
  }
}
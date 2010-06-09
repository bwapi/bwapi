#include <BWAPI.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"

namespace BWAPI
{
  PlayerImpl::PlayerImpl(int id)
  {
    this->self = &(BWAPI::BWAPIClient.data->players[id]);
    this->id = id;
    clear();
  }
  void PlayerImpl::clear()
  {
    units.clear();
  }
  int PlayerImpl::getID() const
  {
    return id;
  }
  std::string PlayerImpl::getName() const
  {
    return std::string(self->name);
  }
  const std::set<Unit*>& PlayerImpl::getUnits() const
  {
    return units;
  }
  Race PlayerImpl::getRace() const
  {
    return Race(self->race);
  }
  PlayerType PlayerImpl::getType() const
  {
    return PlayerType(self->type);
  }
  Force* PlayerImpl::getForce() const
  {
    return ((GameImpl*)Broodwar)->getForce(self->force);
  }
  bool PlayerImpl::isAlly(Player* player) const
  {
    return self->isAlly[player->getID()];
  }
  bool PlayerImpl::isEnemy(Player* player) const
  {
    return self->isEnemy[player->getID()];
  }
  bool PlayerImpl::isNeutral() const
  {
    return self->isNeutral;
  }
  TilePosition PlayerImpl::getStartLocation() const
  {
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
    return self->leftGame;
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
    return self->supplyTotal[self->race];
  }
  int PlayerImpl::supplyUsed() const
  {
    return self->supplyUsed[self->race];
  }
  int PlayerImpl::supplyTotal(Race race) const
  {
    return self->supplyTotal[race.getID()];
  }
  int PlayerImpl::supplyUsed(Race race) const
  {
    return self->supplyUsed[race.getID()];
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
  int PlayerImpl::maxEnergy(UnitType unit) const
  {
    int energy = unit.maxEnergy();
    if ((unit == UnitTypes::Protoss_Arbiter       && getUpgradeLevel(UpgradeTypes::Khaydarin_Core)>0) ||
        (unit == UnitTypes::Protoss_Corsair       && getUpgradeLevel(UpgradeTypes::Argus_Jewel)>0) ||
        (unit == UnitTypes::Protoss_Dark_Archon   && getUpgradeLevel(UpgradeTypes::Argus_Talisman)>0) ||
        (unit == UnitTypes::Protoss_High_Templar  && getUpgradeLevel(UpgradeTypes::Khaydarin_Amulet)>0) ||
        (unit == UnitTypes::Terran_Ghost          && getUpgradeLevel(UpgradeTypes::Moebius_Reactor)>0) ||
        (unit == UnitTypes::Terran_Battlecruiser  && getUpgradeLevel(UpgradeTypes::Colossus_Reactor)>0) ||
        (unit == UnitTypes::Terran_Science_Vessel && getUpgradeLevel(UpgradeTypes::Titan_Reactor)>0) ||
        (unit == UnitTypes::Terran_Wraith         && getUpgradeLevel(UpgradeTypes::Apollo_Reactor)>0) ||
        (unit == UnitTypes::Terran_Medic          && getUpgradeLevel(UpgradeTypes::Caduceus_Reactor)>0) ||
        (unit == UnitTypes::Zerg_Defiler          && getUpgradeLevel(UpgradeTypes::Metasynaptic_Node)>0) ||
        (unit == UnitTypes::Zerg_Queen            && getUpgradeLevel(UpgradeTypes::Gamete_Meiosis)>0))
      energy+=50;
    return energy;
  }
}
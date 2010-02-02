#include "Unit.h"
#include "Player.h"
#include "Constants.h"
#include <Util\Version.h>
#include <Util\Foreach.h>

#include <BWAPI\all.h>
#include "Game.h"
#include <string>
#include "Order.h"
namespace BWSL
{
//  void pushCommand(BWAPI::CommandID commandID, int unitID, int x=0, int y=0, int targetID=0, int specialID=0);
  Unit::Unit(int id)
  {
    this->self=BWAPI::getUnit(id);
    this->id=id;
    this->alive=true;
  }
  int Unit::getID() const
  {
    return id;
  }
  Player* Unit::getPlayer() const
  {
    return Game::getPlayer(self->player);
  }
  UnitType Unit::getType() const
  {
    return UnitType(self->type);
  }
  int Unit::getHitPoints() const
  {
    return self->hitPoints;
  }
  int Unit::getShields() const
  {
    return self->shields;
  }
  int Unit::getEnergy() const
  {
    return self->energy;
  }
  int Unit::getResources() const
  {
    return self->resources;
  }
  int Unit::getKillCount() const
  {
    return self->killCount;
  }
  int Unit::getGroundWeaponCooldown() const
  {
    return self->groundWeaponCooldown;
  }
  int Unit::getAirWeaponCooldown() const
  {
    return self->airWeaponCooldown;
  }
  int Unit::getSpellCooldown() const
  {
    return self->spellCooldown;
  }
  int Unit::getDefenseMatrixPoints() const
  {
    return self->defenseMatrixPoints;
  }
  int Unit::getDefenseMatrixTimer() const
  {
    return self->defenseMatrixTimer;
  }
  int Unit::getEnsnareTimer() const
  {
    return self->ensnareTimer;
  }
  int Unit::getIrradiateTimer() const
  {
    return self->irradiateTimer;
  }
  int Unit::getLockdownTimer() const
  {
    return self->lockdownTimer;
  }
  int Unit::getMaelstromTimer() const
  {
    return self->maelstromTimer;
  }
  int Unit::getPlagueTimer() const
  {
    return self->plagueTimer;
  }
  int Unit::getRemoveTimer() const
  {
    return self->removeTimer;
  }
  int Unit::getStasisTimer() const
  {
    return self->stasisTimer;
  }
  int Unit::getStimTimer() const
  {
    return self->stimTimer;
  }
  Position Unit::getPosition() const
  {
    return Position(self->position.x,self->position.y);
  }
  Position Unit::getInitialPosition() const
  {
    return Position(0,0);//FIX FIX FIX
  }
  TilePosition Unit::getTilePosition() const
  {
    return TilePosition(Position(this->getPosition().x() - this->getType().tileWidth() * TILE_SIZE / 2,
                                 this->getPosition().y() - this->getType().tileHeight() * TILE_SIZE / 2));
  }
  TilePosition Unit::getInitialTilePosition() const
  {
    return TilePosition(0,0);//FIX FIX FIX
  }
  double Unit::getDistance(Unit* target) const
  {
    if (this == target)
      return 0;
    const Unit* i=this;
    const Unit* j=target;
    double result = 0;
    if (i->getPosition().y() - i->getType().dimensionUp() <= j->getPosition().y() + j->getType().dimensionDown() &&
        i->getPosition().y() + i->getType().dimensionDown() >= j->getPosition().y() - j->getType().dimensionUp())
    {
        if (i->getPosition().x() > j->getPosition().x())
          result = i->getPosition().x() - i->getType().dimensionLeft()  - j->getPosition().x() - j->getType().dimensionRight();
        else
          result = j->getPosition().x() - j->getType().dimensionRight() - i->getPosition().x() - i->getType().dimensionLeft();
    }
    else if (i->getPosition().x() - i->getType().dimensionLeft() <= j->getPosition().x() + j->getType().dimensionRight() && 
             i->getPosition().x() + i->getType().dimensionRight() >= j->getPosition().x() - j->getType().dimensionLeft())
    {
        if (i->getPosition().y() > j->getPosition().y())
          result = i->getPosition().y() - i->getType().dimensionUp()   - j->getPosition().y() - j->getType().dimensionDown();
        else
          result = j->getPosition().y() - j->getType().dimensionDown() - i->getPosition().y() - i->getType().dimensionUp();
    }
    else if (i->getPosition().x() > j->getPosition().x())
    {
      if (i->getPosition().y() > j->getPosition().y())
        result = Position(i->getPosition().x() - i->getType().dimensionLeft(),
                          i->getPosition().y() - i->getType().dimensionUp()).getApproxDistance(
                 Position(j->getPosition().x() + j->getType().dimensionRight(),
                          j->getPosition().y() + j->getType().dimensionDown()));
      else
        result = Position(i->getPosition().x() - i->getType().dimensionLeft(),
                          i->getPosition().y() + i->getType().dimensionDown()).getApproxDistance(
                 Position(j->getPosition().x() + j->getType().dimensionRight(),
                          j->getPosition().y() - j->getType().dimensionUp()));
    }
    else
    {
      if (i->getPosition().y() > j->getPosition().y())
        result = Position(i->getPosition().x() + i->getType().dimensionRight(),
                          i->getPosition().y() - i->getType().dimensionUp()).getApproxDistance(
                 Position(j->getPosition().x() - j->getType().dimensionLeft(),
                          j->getPosition().y() + j->getType().dimensionDown()));
      else
        result = Position(i->getPosition().x() + i->getType().dimensionRight(),
                          i->getPosition().y() + i->getType().dimensionDown()).getApproxDistance(
                 Position(j->getPosition().x() - j->getType().dimensionLeft(),
                          j->getPosition().y() - j->getType().dimensionUp()));
    }
    if (result > 0)
      return result;
    return 0;
  }
  double Unit::getDistance(Position target) const
  {
    double result = 0;
    if (getPosition().y() - getType().dimensionUp() <= target.y())
      if (getPosition().y() + getType().dimensionDown() >= target.y())
        if (getPosition().x() > target.x())
          result = getPosition().x() - getType().dimensionLeft()  - target.x();
        else
          result = target.x() - getPosition().x() - getType().dimensionLeft();

    if (getPosition().x() - getType().dimensionLeft() <= target.x())
      if (getPosition().x() + getType().dimensionRight() >= target.x())
        if (getPosition().y() > target.y())
          result = getPosition().y() - getType().dimensionUp()   - target.y();
        else
          result = target.y() - getPosition().y() - getType().dimensionUp();

    if (this->getPosition().x() > target.x())
    {
      if (this->getPosition().y() > target.y())
        result = Position(getPosition().x() - getType().dimensionLeft(),
                          getPosition().y() - getType().dimensionUp()).getDistance(target);
      else
        result = Position(getPosition().x() - getType().dimensionLeft(),
                          getPosition().y() + getType().dimensionDown()).getDistance(target);
    }
    else
    {
      if (this->getPosition().y() > target.y())
        result = Position(getPosition().x() + getType().dimensionRight(),
                          getPosition().y() - getType().dimensionUp()).getDistance(target);
      else
        result = Position(getPosition().x() + getType().dimensionRight(),
                          getPosition().y() + getType().dimensionDown()).getDistance(target);
    }
    if (result > 0)
      return result;
    return 0;
  }
  double Unit::getAngle() const
  {
    return self->angle;
  }
  double Unit::getVelocityX() const
  {
    return self->velocity.x;
  }
  double Unit::getVelocityY() const
  {
    return self->velocity.y;
  }
  Unit* Unit::getTarget() const
  {
    return Game::getUnit(self->movementTargetUnit);
  }
  Position Unit::getTargetPosition() const
  {
    return Position(self->movementTargetPosition.x,self->movementTargetPosition.y);
  }
  Order Unit::getOrder() const
  {
    return BWSL::Order(self->order.type);
  }
  Unit* Unit::getOrderTarget() const
  {
    return NULL;//FIX FIX FIX Game::getUnit(self->orderTarget);
  }
  int Unit::getOrderTimer() const
  {
    return 0;//FIX FIX FIX self->orderTimer;
  }
  Order Unit::getSecondaryOrder() const
  {
    return 0;//FIX FIX FIX Order(self->secondaryOrder);
  }
  Unit* Unit::getBuildUnit() const
  {
    return Game::getUnit(self->buildUnit);
  }
  UnitType Unit::getBuildType() const
  {
    return UnitTypes::None;//UnitType(self->buildType);
  }
  int Unit::getRemainingBuildTime() const
  {
    return 0;//self->remainingBuildTime;
  }
  int Unit::getRemainingTrainTime() const
  {
    return self->trainTimer;
  }
  Unit* Unit::getChild() const
  {
    return Game::getUnit(self->child);
  }
  std::list<UnitType > Unit::getTrainingQueue() const
  {
    std::list<UnitType > trainingQueue;
    for (int i=0;i<self->trainingQueue.size;i++)
    {
      trainingQueue.push_back(self->trainingQueue.data[i]);
    }
    return trainingQueue;
  }
  Unit* Unit::getTransport() const
  {
    return Game::getUnit(self->transport);
  }
  std::list<Unit*> Unit::getLoadedUnits() const
  {
    std::list<Unit*> loadedUnits;
    for (int i=0;i<self->loadedUnits.size;i++)
    {
      loadedUnits.push_back(Game::getUnit(self->loadedUnits.data[i]));
    }
    return loadedUnits;
  }
  int Unit::getInterceptorCount() const
  {
    return self->interceptorInHangarCount+self->interceptorOutOfHangarCount;
  }
  int Unit::getScarabCount() const
  {
    return self->scarabCount;
  }
  int Unit::getSpiderMineCount() const
  {
    return self->spiderMineCount;
  }
  TechType Unit::getTech() const
  {
    return TechType(self->tech);
  }
  UpgradeType Unit::getUpgrade() const
  {
    return UpgradeType(self->tech);
  }
  int Unit::getRemainingResearchTime() const
  {
    return self->researchTimer;
  }
  int Unit::getRemainingUpgradeTime() const
  {
    return self->upgradeTimer;
  }
  Position Unit::getRallyPosition() const
  {
    return Position(self->rallyPosition.x,self->rallyPosition.y);
  }
  Unit* Unit::getRallyUnit() const
  {
    return Game::getUnit(self->rallyUnit);
  }
  Unit* Unit::getAddon() const
  {
    return Game::getUnit(self->addon);
  }
  Unit* Unit::getHatchery() const
  {
    return Game::getUnit(self->hatchery);
  }
  std::set<Unit*> Unit::getLarva() const
  {
    return this->larva;
  }
  int Unit::getUpgradeLevel(UpgradeType upgrade) const
  {
    if (this->getPlayer()->getUpgradeLevel(upgrade)==0) return 0;
    foreach (const UnitType* u, upgrade.whatUses())
    {
      if (*u==this->getType())
        return this->getPlayer()->getUpgradeLevel(upgrade);
    }
    return 0;
  }

  bool Unit::exists() const
  {
    return alive;
  }
  bool Unit::isAccelerating() const
  {
    return self->isAccelerating;
  }
  bool Unit::isBeingConstructed() const
  {
    return self->isBeingConstructed;
  }
  bool Unit::isBeingGathered() const
  {
    return self->isBeingGathered;
  }
  bool Unit::isBeingHealed() const
  {
    return self->isBeingHealed;
  }
  bool Unit::isBlind() const
  {
    return self->isBlind;
  }
  bool Unit::isBraking() const
  {
    return self->isBraking;
  }
  bool Unit::isBurrowed() const
  {
    return self->isBurrowed;
  }
  bool Unit::isCarryingGas() const
  {
    return self->isCarryingGas;
  }
  bool Unit::isCarryingMinerals() const
  {
    return self->isCarryingMinerals;
  }
  bool Unit::isCloaked() const
  {
    return self->isCloaked;
  }
  bool Unit::isCompleted() const
  {
    return self->isCompleted;
  }
  bool Unit::isConstructing() const
  {
    return self->isConstructing;
  }
  bool Unit::isDefenseMatrixed() const
  {
    return self->isDefenseMatrixed;
  }
  bool Unit::isEnsnared() const
  {
    return self->isEnsnared;
  }
  bool Unit::isFollowing() const
  {
    return self->isFollowing;
  }
  bool Unit::isGatheringGas() const
  {
    return self->isGatheringGas;
  }
  bool Unit::isGatheringMinerals() const
  {
    return self->isGatheringMinerals;
  }
  bool Unit::isHallucination() const
  {
    return self->isHallucination;
  }
  bool Unit::isIdle() const
  {
    return self->isIdle;
  }
  bool Unit::isIrradiated() const
  {
    return self->isIrradiated;
  }
  bool Unit::isLifted() const
  {
    return self->isLifted;
  }
  bool Unit::isLoaded() const
  {
    return self->isLoaded;
  }
  bool Unit::isLockedDown() const
  {
    return self->isLockedDown;
  }
  bool Unit::isMaelstrommed() const
  {
    return self->isMaelstrommed;
  }
  bool Unit::isMorphing() const
  {
    return self->isMorphing;
  }
  bool Unit::isMoving() const
  {
    return self->isMoving;
  }
  bool Unit::isParasited() const
  {
    return self->isParasited;
  }
  bool Unit::isPatrolling() const
  {
    return self->isPatrolling;
  }
  bool Unit::isPlagued() const
  {
    return self->isPlagued;
  }
  bool Unit::isRepairing() const
  {
    return self->isRepairing;
  }
  bool Unit::isResearching() const
  {
    return self->isResearching;
  }
  bool Unit::isSelected() const
  {
    return self->isSelected;
  }
  bool Unit::isSieged() const
  {
    return self->isSieged;
  }
  bool Unit::isStartingAttack() const
  {
    return this->startingAttack;
  }
  bool Unit::isStasised() const
  {
    return self->isStasised;
  }
  bool Unit::isStimmed() const
  {
    return self->isStimmed;
  }
  bool Unit::isTraining() const
  {
    return self->isTraining;
  }
  bool Unit::isUnderStorm() const
  {
    return self->isUnderStorm;
  }
  bool Unit::isUnpowered() const
  {
    return self->isUnpowered;
  }
  bool Unit::isUpgrading() const
  {
    return self->isUpgrading;
  }
  bool Unit::isVisible() const
  {
    return self->clearanceLevel>=3;
  }
  bool Unit::attackUnit(Unit* target)
  {
    BWAPI::orderAttackUnit(this->id,target->id);
    return true;
  }
  bool Unit::rightClick(Unit* target)
  {
    BWAPI::orderRightClickUnit(this->id,target->id);
    return true;
  }
  bool Unit::stop()
  {
    BWAPI::orderStop(this->id);
    return true;
  }
  bool Unit::holdPosition()
  {
    BWAPI::orderHoldPosition(this->id);
    return true;
  }
  bool Unit::follow(Unit* target)
  {
    BWAPI::orderFollow(this->id,target->id);
    return true;
  }
  bool Unit::setRallyUnit(Unit* target)
  {
    BWAPI::orderSetRallyUnit(this->id,target->id);
    return true;
  }
  bool Unit::repair(Unit* target)
  {
    BWAPI::orderRepair(this->id,target->id);
    return true;
  }
  bool Unit::burrow()
  {
    BWAPI::orderBurrow(this->id);
    return true;
  }
  bool Unit::unburrow()
  {
    BWAPI::orderUnburrow(this->id);
    return true;
  }
  bool Unit::siege()
  {
    BWAPI::orderSiege(this->id);
    return true;
  }
  bool Unit::unsiege()
  {
    BWAPI::orderUnsiege(this->id);
    return true;
  }
  bool Unit::cloak()
  {
    BWAPI::orderCloak(this->id);
    return true;
  }
  bool Unit::decloak()
  {
    BWAPI::orderDecloak(this->id);
    return true;
  }
  bool Unit::lift()
  {
    BWAPI::orderLift(this->id);
    return true;
  }
  bool Unit::load(Unit* target)
  {
    BWAPI::orderLoad(this->id,target->id);
    return true;
  }
  bool Unit::unload(Unit* target)
  {
    BWAPI::orderUnload(this->id,target->id);
    return true;
  }
  bool Unit::unloadAll()
  {
    BWAPI::orderUnloadAll(this->id);
    return true;
  }
  bool Unit::cancelConstruction()
  {
    BWAPI::orderCancelConstruction(this->id);
    return true;
  }
  bool Unit::haltConstruction()
  {
    BWAPI::orderHaltConstruction(this->id);
    return true;
  }
  bool Unit::cancelMorph()
  {
    BWAPI::orderCancelMorph(this->id);
    return true;
  }
  bool Unit::cancelTrain()
  {
    BWAPI::orderCancelTrain(this->id);
    return true;
  }
  bool Unit::cancelTrain(int slot)
  {
    BWAPI::orderCancelTrainSlot(this->id,slot);
    return true;
  }
  bool Unit::cancelAddon()
  {
    BWAPI::orderCancelAddon(this->id);
    return true;
  }
  bool Unit::cancelResearch()
  {
    BWAPI::orderCancelResearch(this->id);
    return true;
  }
  bool Unit::cancelUpgrade()
  {
    BWAPI::orderCancelUpgrade(this->id);
    return true;
  }

  /*
  void pushCommand(BWAPI::UnitCommandTypeID commandID, int unitID, int x, int y, int targetID, int specialID)
  {
    BWAPI::UnitCommand* c = &(BridgeClient::sharedCommandData->commandQueue[BridgeClient::sharedCommandData->lastFreeCommandSlot]);
    c->commandID=commandID;
    c->unitID=unitID;
    c->x=x;
    c->y=y;
    c->targetID=targetID;
    c->specialID=specialID;
    BridgeClient::sharedCommandData->lastFreeCommandSlot++;
  }
  */
}

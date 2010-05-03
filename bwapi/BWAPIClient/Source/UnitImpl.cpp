#include <BWAPI.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"

#include <Util\Foreach.h>

#include <string>
namespace BWAPI
{
  UnitImpl::UnitImpl(int id)
  {
    this->self=&(BWAPI::BWAPIClient.data->units[id]);
    this->id=id;
    clear();
  }
  void UnitImpl::clear()
  {
    initialType=UnitTypes::None;
    initialResources=0;
    initialHitPoints=0;
    initialPosition=Positions::None;
    larva.clear();
  }
  void UnitImpl::saveInitialState()
  {
    this->initialType=getType();
    this->initialResources=getResources();
    this->initialHitPoints=getHitPoints();
    this->initialPosition=getPosition();
  }
  int UnitImpl::getID() const
  {
    return id;
  }
  Player* UnitImpl::getPlayer() const
  {
    return ((GameImpl*)Broodwar)->getPlayer(self->player);
  }
  UnitType UnitImpl::getType() const
  {
    return UnitType(self->type);
  }
  UnitType UnitImpl::getInitialType() const
  {
    return this->initialType;
  }
  int UnitImpl::getHitPoints() const
  {
    return self->hitPoints;
  }
  int UnitImpl::getInitialHitPoints() const
  {
    return this->initialHitPoints;
  }
  int UnitImpl::getShields() const
  {
    return self->shields;
  }
  int UnitImpl::getEnergy() const
  {
    return self->energy;
  }
  int UnitImpl::getResources() const
  {
    return self->resources;
  }
  int UnitImpl::getInitialResources() const
  {
    return this->initialResources;
  }
  int UnitImpl::getKillCount() const
  {
    return self->killCount;
  }
  int UnitImpl::getGroundWeaponCooldown() const
  {
    return self->groundWeaponCooldown;
  }
  int UnitImpl::getAirWeaponCooldown() const
  {
    return self->airWeaponCooldown;
  }
  int UnitImpl::getSpellCooldown() const
  {
    return self->spellCooldown;
  }
  int UnitImpl::getDefenseMatrixPoints() const
  {
    return self->defenseMatrixPoints;
  }
  int UnitImpl::getDefenseMatrixTimer() const
  {
    return self->defenseMatrixTimer;
  }
  int UnitImpl::getEnsnareTimer() const
  {
    return self->ensnareTimer;
  }
  int UnitImpl::getIrradiateTimer() const
  {
    return self->irradiateTimer;
  }
  int UnitImpl::getLockdownTimer() const
  {
    return self->lockdownTimer;
  }
  int UnitImpl::getMaelstromTimer() const
  {
    return self->maelstromTimer;
  }
  int UnitImpl::getPlagueTimer() const
  {
    return self->plagueTimer;
  }
  int UnitImpl::getRemoveTimer() const
  {
    return self->removeTimer;
  }
  int UnitImpl::getStasisTimer() const
  {
    return self->stasisTimer;
  }
  int UnitImpl::getStimTimer() const
  {
    return self->stimTimer;
  }
  Position UnitImpl::getPosition() const
  {
    return Position(self->positionX,self->positionY);
  }
  Position UnitImpl::getInitialPosition() const
  {
    return this->initialPosition;
  }
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(self->positionX - this->getType().tileWidth() * TILE_SIZE / 2,
                                 self->positionY - this->getType().tileHeight() * TILE_SIZE / 2));
  }
  TilePosition UnitImpl::getInitialTilePosition() const
  {
    return TilePosition(Position(this->initialPosition.x() - this->initialType.tileWidth() * TILE_SIZE / 2,
                                 this->initialPosition.y() - this->initialType.tileHeight() * TILE_SIZE / 2));
  }
  double UnitImpl::getDistance(Unit* target) const
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
  double UnitImpl::getDistance(Position target) const
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
  double UnitImpl::getAngle() const
  {
    return self->angle;
  }
  double UnitImpl::getVelocityX() const
  {
    return self->velocityX;
  }
  double UnitImpl::getVelocityY() const
  {
    return self->velocityY;
  }
  Unit* UnitImpl::getTarget() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->target);
  }
  Position UnitImpl::getTargetPosition() const
  {
    return Position(self->targetPositionX,self->targetPositionY);
  }
  Order UnitImpl::getOrder() const
  {
    return BWAPI::Order(self->order);
  }
  Unit* UnitImpl::getOrderTarget() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->orderTarget);
  }
  int UnitImpl::getOrderTimer() const
  {
    return self->orderTimer;
  }
  Order UnitImpl::getSecondaryOrder() const
  {
    return BWAPI::Order(self->secondaryOrder);
  }
  Unit* UnitImpl::getBuildUnit() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->buildUnit);
  }
  UnitType UnitImpl::getBuildType() const
  {
    return UnitType(self->buildType);
  }
  int UnitImpl::getRemainingBuildTime() const
  {
    return self->remainingBuildTime;
  }
  int UnitImpl::getRemainingTrainTime() const
  {
    return self->remainingTrainTime;
  }
  Unit* UnitImpl::getChild() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->child);
  }
  std::list<UnitType > UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType > trainingQueue;
    for (int i=0;i<self->trainingQueueCount;i++)
    {
      trainingQueue.push_back(self->trainingQueue[i]);
    }
    return trainingQueue;
  }
  Unit* UnitImpl::getTransport() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->transport);
  }
  std::list<Unit*> UnitImpl::getLoadedUnits() const
  {
    std::list<Unit*> loadedUnits;
    for (int i=0;i<self->loadedUnitCount;i++)
    {
      loadedUnits.push_back(((GameImpl*)Broodwar)->getUnit(self->loadedUnits[i]));
    }
    return loadedUnits;
  }
  int UnitImpl::getInterceptorCount() const
  {
    return self->interceptorCount;
  }
  int UnitImpl::getScarabCount() const
  {
    return self->scarabCount;
  }
  int UnitImpl::getSpiderMineCount() const
  {
    return self->spiderMineCount;
  }
  TechType UnitImpl::getTech() const
  {
    return TechType(self->tech);
  }
  UpgradeType UnitImpl::getUpgrade() const
  {
    return UpgradeType(self->tech);
  }
  int UnitImpl::getRemainingResearchTime() const
  {
    return self->remainingResearchTime;
  }
  int UnitImpl::getRemainingUpgradeTime() const
  {
    return self->remainingUpgradeTime;
  }
  Position UnitImpl::getRallyPosition() const
  {
    return Position(self->rallyPositionX,self->rallyPositionY);
  }
  Unit* UnitImpl::getRallyUnit() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->rallyUnit);
  }
  Unit* UnitImpl::getAddon() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->addon);
  }
  Unit* UnitImpl::getHatchery() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->hatchery);
  }
  std::set<Unit*> UnitImpl::getLarva() const
  {
    return this->larva;
  }
  int UnitImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    if (this->getPlayer()->getUpgradeLevel(upgrade)==0) return 0;
    foreach (const UnitType* u, upgrade.whatUses())
    {
      if (*u==this->getType())
        return this->getPlayer()->getUpgradeLevel(upgrade);
    }
    return 0;
  }

  bool UnitImpl::exists() const
  {
    return self->exists;
  }
  bool UnitImpl::isAccelerating() const
  {
    return self->isAccelerating;
  }
  bool UnitImpl::isAttacking() const
  {
    return self->isAttacking;
  }
  bool UnitImpl::isBeingConstructed() const
  {
    return self->isBeingConstructed;
  }
  bool UnitImpl::isBeingGathered() const
  {
    return self->isBeingGathered;
  }
  bool UnitImpl::isBeingHealed() const
  {
    return self->isBeingHealed;
  }
  bool UnitImpl::isBlind() const
  {
    return self->isBlind;
  }
  bool UnitImpl::isBraking() const
  {
    return self->isBraking;
  }
  bool UnitImpl::isBurrowed() const
  {
    return self->isBurrowed;
  }
  bool UnitImpl::isCarryingGas() const
  {
    return self->isCarryingGas;
  }
  bool UnitImpl::isCarryingMinerals() const
  {
    return self->isCarryingMinerals;
  }
  bool UnitImpl::isCloaked() const
  {
    return self->isCloaked;
  }
  bool UnitImpl::isCompleted() const
  {
    return self->isCompleted;
  }
  bool UnitImpl::isConstructing() const
  {
    return self->isConstructing;
  }
  bool UnitImpl::isDefenseMatrixed() const
  {
    return self->isDefenseMatrixed;
  }
  bool UnitImpl::isEnsnared() const
  {
    return self->isEnsnared;
  }
  bool UnitImpl::isFollowing() const
  {
    return self->isFollowing;
  }
  bool UnitImpl::isGatheringGas() const
  {
    return self->isGatheringGas;
  }
  bool UnitImpl::isGatheringMinerals() const
  {
    return self->isGatheringMinerals;
  }
  bool UnitImpl::isHallucination() const
  {
    return self->isHallucination;
  }
  bool UnitImpl::isIdle() const
  {
    return self->isIdle;
  }
  bool UnitImpl::isIrradiated() const
  {
    return self->isIrradiated;
  }
  bool UnitImpl::isLifted() const
  {
    return self->isLifted;
  }
  bool UnitImpl::isLoaded() const
  {
    return self->isLoaded;
  }
  bool UnitImpl::isLockedDown() const
  {
    return self->isLockedDown;
  }
  bool UnitImpl::isMaelstrommed() const
  {
    return self->isMaelstrommed;
  }
  bool UnitImpl::isMorphing() const
  {
    return self->isMorphing;
  }
  bool UnitImpl::isMoving() const
  {
    return self->isMoving;
  }
  bool UnitImpl::isParasited() const
  {
    return self->isParasited;
  }
  bool UnitImpl::isPatrolling() const
  {
    return self->isPatrolling;
  }
  bool UnitImpl::isPlagued() const
  {
    return self->isPlagued;
  }
  bool UnitImpl::isRepairing() const
  {
    return self->isRepairing;
  }
  bool UnitImpl::isResearching() const
  {
    return self->isResearching;
  }
  bool UnitImpl::isSelected() const
  {
    return self->isSelected;
  }
  bool UnitImpl::isSieged() const
  {
    return self->isSieged;
  }
  bool UnitImpl::isStartingAttack() const
  {
    return self->isStartingAttack;
  }
  bool UnitImpl::isStasised() const
  {
    return self->isStasised;
  }
  bool UnitImpl::isStimmed() const
  {
    return self->isStimmed;
  }
  bool UnitImpl::isTraining() const
  {
    return self->isTraining;
  }
  bool UnitImpl::isUnderStorm() const
  {
    return self->isUnderStorm;
  }
  bool UnitImpl::isUnpowered() const
  {
    return self->isUnpowered;
  }
  bool UnitImpl::isUpgrading() const
  {
    return self->isUpgrading;
  }
  bool UnitImpl::isVisible() const
  {
    return self->isVisible;
  }
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    BWAPIC::UnitCommand c;
    c.type=command.type;
    c.unitIndex=this->id;
    if (command.target!=NULL)
      c.targetIndex=command.target->getID();
    else
      c.targetIndex=-1;
    c.x=command.x;
    c.y=command.y;
    c.extra=command.extra;
    ((GameImpl*)Broodwar)->addUnitCommand(c);
    return true;
  }
  bool UnitImpl::attackMove(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::AttackPosition,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::attackUnit(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::AttackUnit,target,0,0,0));
    return true;
  }
  bool UnitImpl::rightClick(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::RightClickPosition,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::rightClick(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::RightClickUnit,target,0,0,0));
    return true;
  }
  bool UnitImpl::train(UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Train,NULL,0,0,type.getID()));
    return true;
  }
  bool UnitImpl::build(TilePosition target, UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Build,NULL,target.x(),target.y(),type.getID()));
    return true;
  }
  bool UnitImpl::buildAddon(UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandType::BuildAddon,NULL,0,0,type.getID()));
    return true;
  }
  bool UnitImpl::research(TechType tech)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Research,NULL,0,0,tech.getID()));
    return true;
  }
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Upgrade,NULL,0,0,upgrade.getID()));
    return true;
  }
  bool UnitImpl::stop()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Stop,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::holdPosition()
  {
    this->issueCommand(UnitCommand(UnitCommandType::HoldPosition,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::patrol(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Patrol,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::follow(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Follow,target,0,0,0));
    return true;
  }
  bool UnitImpl::setRallyPosition(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::SetRallyPosition,0,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::SetRallyUnit,target,0,0,0));
    return true;
  }
  bool UnitImpl::repair(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Repair,target,0,0,0));
    return true;
  }
  bool UnitImpl::returnCargo()
  {
    this->issueCommand(UnitCommand(UnitCommandType::ReturnCargo,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::morph(UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Morph,NULL,0,0,type.getID()));
    return true;
  }
  bool UnitImpl::burrow()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Burrow,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::unburrow()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Unburrow,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::siege()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Siege,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::unsiege()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Unsiege,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cloak()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Cloak,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::decloak()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Decloak,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::lift()
  {
    this->issueCommand(UnitCommand(UnitCommandType::Lift,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::land(TilePosition target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Land,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::load(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Load,target,0,0,0));
    return true;
  }
  bool UnitImpl::unload(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::Unload,target,0,0,0));
    return true;
  }
  bool UnitImpl::unloadAll()
  {
    this->issueCommand(UnitCommand(UnitCommandType::UnloadAll,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::unloadAll(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::UnloadAllPosition,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::cancelConstruction()
  {
    this->issueCommand(UnitCommand(UnitCommandType::CancelConstruction,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::haltConstruction()
  {
    this->issueCommand(UnitCommand(UnitCommandType::HaltConstruction,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelMorph()
  {
    this->issueCommand(UnitCommand(UnitCommandType::CancelMorph,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelTrain()
  {
    this->issueCommand(UnitCommand(UnitCommandType::CancelTrain,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelTrain(int slot)
  {
    this->issueCommand(UnitCommand(UnitCommandType::CancelTrainSlot,NULL,0,0,slot));
    return true;
  }
  bool UnitImpl::cancelAddon()
  {
    this->issueCommand(UnitCommand(UnitCommandType::CancelAddon,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelResearch()
  {
    this->issueCommand(UnitCommand(UnitCommandType::CancelResearch,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelUpgrade()
  {
    this->issueCommand(UnitCommand(UnitCommandType::CancelUpgrade,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::useTech(TechType tech)
  {
    this->issueCommand(UnitCommand(UnitCommandType::UseTech,NULL,0,0,tech.getID()));
    return true;
  }
  bool UnitImpl::useTech(TechType tech,Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::UseTechPosition,NULL,target.x(),target.y(),tech.getID()));
    return true;
  }
  bool UnitImpl::useTech(TechType tech,Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandType::UseTechUnit,target,0,0,tech.getID()));
    return true;
  }
}

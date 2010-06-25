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
    connectedUnits.clear();
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
  Position UnitImpl::getPosition() const
  {
    return Position(self->positionX,self->positionY);
  }
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(self->positionX - this->getType().tileWidth() * TILE_SIZE / 2,
                                 self->positionY - this->getType().tileHeight() * TILE_SIZE / 2));
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
  int UnitImpl::getHitPoints() const
  {
    return self->hitPoints;
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
  int UnitImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    if (this->getPlayer()->getUpgradeLevel(upgrade)==0) return 0;
    if (upgrade.whatUses().find(this->getType()) != upgrade.whatUses().end())
      return this->getPlayer()->getUpgradeLevel(upgrade);
    return 0;
  }
  UnitType UnitImpl::getInitialType() const
  {
    return this->initialType;
  }
  Position UnitImpl::getInitialPosition() const
  {
    return this->initialPosition;
  }
  TilePosition UnitImpl::getInitialTilePosition() const
  {
    return TilePosition(Position(this->initialPosition.x() - this->initialType.tileWidth() * TILE_SIZE / 2,
                                 this->initialPosition.y() - this->initialType.tileHeight() * TILE_SIZE / 2));
  }
  int UnitImpl::getInitialHitPoints() const
  {
    return this->initialHitPoints;
  }
  int UnitImpl::getInitialResources() const
  {
    return this->initialResources;
  }
  int UnitImpl::getKillCount() const
  {
    return self->killCount;
  }
  int UnitImpl::getInterceptorCount() const
  {
    if (this->getType()!=UnitTypes::Protoss_Carrier)
      return 0;
    return this->connectedUnits.size();
  }
  int UnitImpl::getScarabCount() const
  {
    return self->scarabCount;
  }
  int UnitImpl::getSpiderMineCount() const
  {
    return self->spiderMineCount;
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
  int UnitImpl::getOrderTimer() const
  {
    return self->orderTimer;
  }
  UnitType UnitImpl::getBuildType() const
  {
    return UnitType(self->buildType);
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
  TechType UnitImpl::getTech() const
  {
    return TechType(self->tech);
  }
  UpgradeType UnitImpl::getUpgrade() const
  {
    return UpgradeType(self->tech);
  }
  int UnitImpl::getRemainingBuildTime() const
  {
    return self->remainingBuildTime;
  }
  int UnitImpl::getRemainingTrainTime() const
  {
    return self->remainingTrainTime;
  }
  int UnitImpl::getRemainingResearchTime() const
  {
    return self->remainingResearchTime;
  }
  int UnitImpl::getRemainingUpgradeTime() const
  {
    return self->remainingUpgradeTime;
  }
  Unit* UnitImpl::getBuildUnit() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->buildUnit);
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
  Order UnitImpl::getSecondaryOrder() const
  {
    return BWAPI::Order(self->secondaryOrder);
  }
  Unit* UnitImpl::getChild() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->child);
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
  Unit* UnitImpl::getNydusExit() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->nydusExit);
  }
  Unit* UnitImpl::getTransport() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->transport);
  }
  std::set<Unit*> UnitImpl::getLoadedUnits() const
  {
    return loadedUnits;
  }
  Unit* UnitImpl::getCarrier() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->carrier);
  }
  std::set<Unit*> UnitImpl::getInterceptors() const
  {
    std::set<Unit*> nothing;
    if (getType()!=UnitTypes::Protoss_Carrier)
      return nothing;
    return this->connectedUnits;
  }
  Unit* UnitImpl::getHatchery() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->hatchery);
  }
  std::set<Unit*> UnitImpl::getLarva() const
  {
    std::set<Unit*> nothing;
    if (getType()!=UnitTypes::Zerg_Hatchery &&
        getType()!=UnitTypes::Zerg_Lair &&
        getType()!=UnitTypes::Zerg_Hive)
        return nothing;
    return this->connectedUnits;
  }
  bool UnitImpl::exists() const
  {
    return self->exists;
  }
  bool UnitImpl::hasNuke() const
  {
    return self->hasNuke;
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
    return self->defenseMatrixTimer > 0;
  }
  bool UnitImpl::isEnsnared() const
  {
    return self->ensnareTimer > 0;
  }
  bool UnitImpl::isFollowing() const
  {
    return self->order == Orders::Follow.getID();
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
    return self->irradiateTimer > 0;
  }
  bool UnitImpl::isLifted() const
  {
    return self->isLifted;
  }
  bool UnitImpl::isLoaded() const
  {
    return self->transport != -1;
  }
  bool UnitImpl::isLockedDown() const
  {
    return self->lockdownTimer > 0;
  }
  bool UnitImpl::isMaelstrommed() const
  {
    return self->maelstromTimer > 0;
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
    return self->order == Orders::Patrol.getID();
  }
  bool UnitImpl::isPlagued() const
  {
    return self->plagueTimer > 0;
  }
  bool UnitImpl::isRepairing() const
  {
    return self->order == Orders::Repair1.getID() || self->order == Orders::Repair2.getID();
  }
  bool UnitImpl::isResearching() const
  {
    return self->order == Orders::ResearchTech.getID();
  }
  bool UnitImpl::isSelected() const
  {
    return self->isSelected;
  }
  bool UnitImpl::isSieged() const
  {
    return self->type == UnitTypes::Terran_Siege_Tank_Siege_Mode.getID();
  }
  bool UnitImpl::isStartingAttack() const
  {
    return self->isStartingAttack;
  }
  bool UnitImpl::isStasised() const
  {
    return self->stasisTimer > 0;
  }
  bool UnitImpl::isStimmed() const
  {
    return self->stimTimer > 0;
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
    return self->order == Orders::Upgrade.getID();
  }
  bool UnitImpl::isVisible() const
  {
    return self->isVisible[self->player];
  }
  bool UnitImpl::isVisible(Player* player) const
  {
    return self->isVisible[player->getID()];
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
    this->issueCommand(UnitCommand(UnitCommandTypes::Attack_Position,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::attackUnit(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Attack_Unit,target,0,0,0));
    return true;
  }
  bool UnitImpl::rightClick(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Right_Click_Position,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::rightClick(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Right_Click_Unit,target,0,0,0));
    return true;
  }
  bool UnitImpl::train(UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Train,NULL,0,0,type.getID()));
    return true;
  }
  bool UnitImpl::build(TilePosition target, UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Build,NULL,target.x(),target.y(),type.getID()));
    return true;
  }
  bool UnitImpl::buildAddon(UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Build_Addon,NULL,0,0,type.getID()));
    return true;
  }
  bool UnitImpl::research(TechType tech)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Research,NULL,0,0,tech.getID()));
    return true;
  }
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Upgrade,NULL,0,0,upgrade.getID()));
    return true;
  }
  bool UnitImpl::stop()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Stop,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::holdPosition()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Hold_Position,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::patrol(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Patrol,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::follow(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Follow,target,0,0,0));
    return true;
  }
  bool UnitImpl::setRallyPosition(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Set_Rally_Position,0,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Set_Rally_Unit,target,0,0,0));
    return true;
  }
  bool UnitImpl::repair(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Repair,target,0,0,0));
    return true;
  }
  bool UnitImpl::returnCargo()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Return_Cargo,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::morph(UnitType type)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Morph,NULL,0,0,type.getID()));
    return true;
  }
  bool UnitImpl::burrow()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Burrow,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::unburrow()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Unburrow,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::siege()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Siege,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::unsiege()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Unsiege,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cloak()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cloak,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::decloak()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Decloak,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::lift()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Lift,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::land(TilePosition target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Land,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::load(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Load,target,0,0,0));
    return true;
  }
  bool UnitImpl::unload(Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Unload,target,0,0,0));
    return true;
  }
  bool UnitImpl::unloadAll()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Unload_All,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::unloadAll(Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Unload_All_Position,NULL,target.x(),target.y(),0));
    return true;
  }
  bool UnitImpl::cancelConstruction()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cancel_Construction,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::haltConstruction()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Halt_Construction,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelMorph()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cancel_Morph,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelTrain()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cancel_Train,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelTrain(int slot)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cancel_Train_Slot,NULL,0,0,slot));
    return true;
  }
  bool UnitImpl::cancelAddon()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cancel_Addon,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelResearch()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cancel_Research,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::cancelUpgrade()
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Cancel_Upgrade,NULL,0,0,0));
    return true;
  }
  bool UnitImpl::useTech(TechType tech)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Use_Tech,NULL,0,0,tech.getID()));
    return true;
  }
  bool UnitImpl::useTech(TechType tech,Position target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Use_Tech_Position,NULL,target.x(),target.y(),tech.getID()));
    return true;
  }
  bool UnitImpl::useTech(TechType tech,Unit* target)
  {
    this->issueCommand(UnitCommand(UnitCommandTypes::Use_Tech_Unit,target,0,0,tech.getID()));
    return true;
  }
}

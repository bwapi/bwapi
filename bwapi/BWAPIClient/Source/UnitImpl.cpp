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
  //--------------------------------------------- GET ID -----------------------------------------------------
  int UnitImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET PLAYER -------------------------------------------------
  Player* UnitImpl::getPlayer() const
  {
    return ((GameImpl*)Broodwar)->getPlayer(self->player);
  }
  //--------------------------------------------- GET TYPE ---------------------------------------------------
  UnitType UnitImpl::getType() const
  {
    return UnitType(self->type);
  }
  //--------------------------------------------- GET POSITION -----------------------------------------------
  Position UnitImpl::getPosition() const
  {
    return Position(self->positionX,self->positionY);
  }
  //--------------------------------------------- GET TILE POSITION ------------------------------------------
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(self->positionX - this->getType().tileWidth() * TILE_SIZE / 2,
                                 self->positionY - this->getType().tileHeight() * TILE_SIZE / 2));
  }
  //--------------------------------------------- GET ANGLE --------------------------------------------------
  double UnitImpl::getAngle() const
  {
    return self->angle;
  }
  //--------------------------------------------- GET VELOCITY X ---------------------------------------------
  double UnitImpl::getVelocityX() const
  {
    return self->velocityX;
  }
  //--------------------------------------------- GET VELOCITY Y ---------------------------------------------
  double UnitImpl::getVelocityY() const
  {
    return self->velocityY;
  }
  //--------------------------------------------- GET HIT POINTS ---------------------------------------------
  int UnitImpl::getHitPoints() const
  {
    return self->hitPoints;
  }
  //--------------------------------------------- GET SHIELDS ------------------------------------------------
  int UnitImpl::getShields() const
  {
    return self->shields;
  }
  //--------------------------------------------- GET ENERGY -------------------------------------------------
  int UnitImpl::getEnergy() const
  {
    return self->energy;
  }
  //--------------------------------------------- GET RESOURCES ----------------------------------------------
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
  //--------------------------------------------- GET KILL COUNT ---------------------------------------------
  int UnitImpl::getKillCount() const
  {
    return self->killCount;
  }
  //--------------------------------------------- GET INTERCEPTOR COUNT --------------------------------------
  int UnitImpl::getInterceptorCount() const
  {
    if (this->getType() != UnitTypes::Protoss_Carrier)
      return 0;
    return this->connectedUnits.size();
  }
  //--------------------------------------------- GET SCARAB COUNT -------------------------------------------
  int UnitImpl::getScarabCount() const
  {
    return self->scarabCount;
  }
  //--------------------------------------------- GET SPIDER MINE COUNT --------------------------------------
  int UnitImpl::getSpiderMineCount() const
  {
    return self->spiderMineCount;
  }
  //--------------------------------------------- GET GROUND WEAPON COOLDOWN ---------------------------------
  int UnitImpl::getGroundWeaponCooldown() const
  {
    return self->groundWeaponCooldown;
  }
  //--------------------------------------------- GET AIR WEAPON COOLDOWN ------------------------------------
  int UnitImpl::getAirWeaponCooldown() const
  {
    return self->airWeaponCooldown;
  }
  //--------------------------------------------- GET SPELL COOLDOWN -----------------------------------------
  int UnitImpl::getSpellCooldown() const
  {
    return self->spellCooldown;
  }
  //--------------------------------------------- GET DEFENSE MATRIX POINTS ----------------------------------
  int UnitImpl::getDefenseMatrixPoints() const
  {
    return self->defenseMatrixPoints;
  }
  //--------------------------------------------- GET DEFENSE MATRIX TIMER -----------------------------------
  int UnitImpl::getDefenseMatrixTimer() const
  {
    return self->defenseMatrixTimer;
  }
  //--------------------------------------------- GET ENSNARE TIMER ------------------------------------------
  int UnitImpl::getEnsnareTimer() const
  {
    return self->ensnareTimer;
  }
  //--------------------------------------------- GET IRRADIATE TIMER ----------------------------------------
  int UnitImpl::getIrradiateTimer() const
  {
    return self->irradiateTimer;
  }
  //--------------------------------------------- GET LOCKDOWN TIMER -----------------------------------------
  int UnitImpl::getLockdownTimer() const
  {
    return self->lockdownTimer;
  }
  //--------------------------------------------- GET MAELSTROM TIMER ----------------------------------------
  int UnitImpl::getMaelstromTimer() const
  {
    return self->maelstromTimer;
  }
  //--------------------------------------------- GET ORDER TIMER --------------------------------------------
  int UnitImpl::getOrderTimer() const
  {
    return self->orderTimer;
  }
  //--------------------------------------------- GET PLAGUE TIMER -------------------------------------------
  int UnitImpl::getPlagueTimer() const
  {
    return self->plagueTimer;
  }
  //--------------------------------------------- GET REMOVE TIMER -------------------------------------------
  int UnitImpl::getRemoveTimer() const
  {
    return self->removeTimer;
  }
  //--------------------------------------------- GET STASIS TIMER -------------------------------------------
  int UnitImpl::getStasisTimer() const
  {
    return self->stasisTimer;
  }
  //--------------------------------------------- GET STIM TIMER ---------------------------------------------
  int UnitImpl::getStimTimer() const
  {
    return self->stimTimer;
  }
  //--------------------------------------------- GET BUILD TYPE ---------------------------------------------
  UnitType UnitImpl::getBuildType() const
  {
    return UnitType(self->buildType);
  }
  //--------------------------------------------- GET TRAINING QUEUE -----------------------------------------
  std::list<UnitType > UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType > trainingQueue;
    for (int i=0;i<self->trainingQueueCount;i++)
      trainingQueue.push_back(self->trainingQueue[i]);
    return trainingQueue;
  }
  //--------------------------------------------- GET TECH ---------------------------------------------------
  TechType UnitImpl::getTech() const
  {
    return TechType(self->tech);
  }
  //--------------------------------------------- GET UPGRADE ------------------------------------------------
  UpgradeType UnitImpl::getUpgrade() const
  {
    return UpgradeType(self->tech);
  }
  //--------------------------------------------- GET REMAINING BUILD TIME -----------------------------------
  int UnitImpl::getRemainingBuildTime() const
  {
    return self->remainingBuildTime;
  }
  //--------------------------------------------- GET REMAINING TRAIN TIME -----------------------------------
  int UnitImpl::getRemainingTrainTime() const
  {
    return self->remainingTrainTime;
  }
  //--------------------------------------------- GET REMAINING RESEARCH TIME --------------------------------
  int UnitImpl::getRemainingResearchTime() const
  {
    return self->remainingResearchTime;
  }
  //--------------------------------------------- GET REMAINING UPGRADE TIME ---------------------------------
  int UnitImpl::getRemainingUpgradeTime() const
  {
    return self->remainingUpgradeTime;
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getBuildUnit() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->buildUnit);
  }
  //--------------------------------------------- GET TARGET -------------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->target);
  }
  //--------------------------------------------- GET TARGET POSITION ----------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    return Position(self->targetPositionX,self->targetPositionY);
  }
  //--------------------------------------------- GET ORDER --------------------------------------------------
  Order UnitImpl::getOrder() const
  {
    return Order(self->order);
  }
  //--------------------------------------------- GET ORDER TARGET -------------------------------------------
  Unit* UnitImpl::getOrderTarget() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->orderTarget);
  }
  //--------------------------------------------- GET SECONDARY ORDER ID -------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return Order(self->secondaryOrder);
  }
  //--------------------------------------------- GET CHILD --------------------------------------------------
  Unit* UnitImpl::getChild() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->child);
  }
  //--------------------------------------------- GET RALLY POSITION -----------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    return Position(self->rallyPositionX,self->rallyPositionY);
  }
  //--------------------------------------------- GET RALLY UNIT ---------------------------------------------
  Unit* UnitImpl::getRallyUnit() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->rallyUnit);
  }
  //--------------------------------------------- GET ADDON --------------------------------------------------
  Unit* UnitImpl::getAddon() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->addon);
  }
  //--------------------------------------------- GET NYDUS EXIT ---------------------------------------------
  Unit* UnitImpl::getNydusExit() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->nydusExit);
  }
  //--------------------------------------------- GET TRANSPORT ----------------------------------------------
  Unit* UnitImpl::getTransport() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->transport);
  }
  //--------------------------------------------- GET LOADED UNITS -------------------------------------------
  std::set<Unit*> UnitImpl::getLoadedUnits() const
  {
    return loadedUnits;
  }
  //--------------------------------------------- GET CARRIER ------------------------------------------------
  Unit* UnitImpl::getCarrier() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->carrier);
  }
  //--------------------------------------------- GET INTERCEPTORS -------------------------------------------
  std::set<Unit*> UnitImpl::getInterceptors() const
  {
    std::set<Unit*> nothing;
    if (getType() != UnitTypes::Protoss_Carrier)
      return nothing;
    return connectedUnits;
  }
  //--------------------------------------------- GET HATCHERY -----------------------------------------------
  Unit* UnitImpl::getHatchery() const
  {
    return ((GameImpl*)Broodwar)->getUnit(self->hatchery);
  }
  //--------------------------------------------- GET LARVA --------------------------------------------------
  std::set<Unit*> UnitImpl::getLarva() const
  {
    std::set<Unit*> nothing;
    if (getType() != UnitTypes::Zerg_Hatchery &&
        getType() != UnitTypes::Zerg_Lair &&
        getType() != UnitTypes::Zerg_Hive)
        return nothing;
    return connectedUnits;
  }
  //--------------------------------------------- EXISTS -----------------------------------------------------
  bool UnitImpl::exists() const
  {
    return self->exists;
  }
  //--------------------------------------------- HAS NUKE ---------------------------------------------------
  bool UnitImpl::hasNuke() const
  {
    return self->hasNuke;
  }
  //--------------------------------------------- IS ACCELERATING --------------------------------------------
  bool UnitImpl::isAccelerating() const
  {
    return self->isAccelerating;
  }
  //--------------------------------------------- IS ATTACKING -----------------------------------------------
  bool UnitImpl::isAttacking() const
  {
    return self->isAttacking;
  }
  //--------------------------------------------- IS BEING CONSTRUCTED ---------------------------------------
  bool UnitImpl::isBeingConstructed() const
  {
    if (self->isMorphing)
      return true;
    if (self->isCompleted)
      return false;
    if (getType().getRace()!=Races::Terran)
      return true;
    return self->buildUnit != -1;
  }
  //--------------------------------------------- IS BEING GATHERED ------------------------------------------
  bool UnitImpl::isBeingGathered() const
  {
    return self->isBeingGathered;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const
  {
    return self->isBeingHealed;
  }
  //--------------------------------------------- IS BLIND ---------------------------------------------------
  bool UnitImpl::isBlind() const
  {
    return self->isBlind;
  }
  //--------------------------------------------- IS BRAKING -------------------------------------------------
  bool UnitImpl::isBraking() const
  {
    return self->isBraking;
  }
  //--------------------------------------------- IS BURROWED ------------------------------------------------
  bool UnitImpl::isBurrowed() const
  {
    return self->isBurrowed;
  }
  //--------------------------------------------- IS CARRYING GAS --------------------------------------------
  bool UnitImpl::isCarryingGas() const
  {
    return self->carryResourceType == 1;
  }
  //--------------------------------------------- IS CARRYING MINERALS ---------------------------------------
  bool UnitImpl::isCarryingMinerals() const
  {
    return self->carryResourceType == 2;
  }
  //--------------------------------------------- IS CLOAKED -------------------------------------------------
  bool UnitImpl::isCloaked() const
  {
    return self->isCloaked;
  }
  //--------------------------------------------- IS COMPLETED -----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    return self->isCompleted;
  }
  //--------------------------------------------- IS CONSTRUCTING --------------------------------------------
  bool UnitImpl::isConstructing() const
  {
    return self->isConstructing;
  }
  //--------------------------------------------- IS DEFENSE MATRIXED ----------------------------------------
  bool UnitImpl::isDefenseMatrixed() const
  {
    return self->defenseMatrixTimer > 0;
  }
  //--------------------------------------------- IS ENSNARED ------------------------------------------------
  bool UnitImpl::isEnsnared() const
  {
    return self->ensnareTimer > 0;
  }
  //--------------------------------------------- IS FOLLOWING -----------------------------------------------
  bool UnitImpl::isFollowing() const
  {
    return self->order == Orders::Follow.getID();
  }
  //--------------------------------------------- IS GATHERING GAS -------------------------------------------
  bool UnitImpl::isGatheringGas() const
  {
    if (!self->isGathering)
      return false;

    if (self->order != Orders::MoveToGas.getID()  &&
        self->order != Orders::WaitForGas.getID() &&
        self->order != Orders::HarvestGas.getID() &&
        self->order != Orders::ReturnGas.getID()  &&
        self->order != Orders::ResetCollision2.getID())
      return false;

    if (self->order == Orders::ResetCollision2.getID())
      return self->carryResourceType == 1;

    //return true if BWOrder is WaitForGas, HarvestGas, or ReturnGas
    if (self->order != Orders::MoveToGas.getID())
      return true;

    //if BWOrder is MoveToGas, we need to do some additional checks to make sure the unit is really gathering
    if (self->target != -1)
    {
      if (getTarget()->getType() == UnitTypes::Resource_Vespene_Geyser)
        return false;
      if (getTarget()->getPlayer() != getPlayer())
        return false;
      if (!getTarget()->isCompleted() && !getTarget()->getType().isResourceDepot())
        return false;
      if (getTarget()->getType().isRefinery() || getTarget()->getType().isResourceDepot())
        return true;
    }
    if (getOrderTarget() != NULL)
    {
      if (getOrderTarget()->getType() == UnitTypes::Resource_Vespene_Geyser)
        return false;
      if (getOrderTarget()->getPlayer() != getPlayer())
        return false;
      if (!this->getOrderTarget()->isCompleted() && !getOrderTarget()->getType().isResourceDepot())
        return false;
      if (this->getOrderTarget()->getType().isRefinery() || getOrderTarget()->getType().isResourceDepot())
        return true;
    }
    return false;
  }
  //--------------------------------------------- IS GATHERING MINERALS --------------------------------------
  bool UnitImpl::isGatheringMinerals() const
  {
    if (!self->isGathering)
      return false;

    if (self->order != Orders::MoveToMinerals.getID() &&
        self->order != Orders::WaitForMinerals.getID() &&
        self->order != Orders::MiningMinerals.getID() &&
        self->order != Orders::ReturnMinerals.getID() &&
        self->order != Orders::ResetCollision2.getID())
      return false;

    if (self->order == Orders::ResetCollision2.getID())
      return self->carryResourceType == 2;
    return true;
  }
  //--------------------------------------------- IS HALLUCINATION -------------------------------------------
  bool UnitImpl::isHallucination() const
  {
    return self->isHallucination;
  }
  //--------------------------------------------- IS IDLE ----------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    return self->isIdle;
  }
  //--------------------------------------------- IS IRRADIATED ----------------------------------------------
  bool UnitImpl::isIrradiated() const
  {
    return self->irradiateTimer > 0;
  }
  //--------------------------------------------- IS LIFTED --------------------------------------------------
  bool UnitImpl::isLifted() const
  {
    return self->isLifted;
  }
  //--------------------------------------------- IS LOADED --------------------------------------------------
  bool UnitImpl::isLoaded() const
  {
    return self->transport != -1;
  }
  //--------------------------------------------- IS LOCKED DOWN ---------------------------------------------
  bool UnitImpl::isLockedDown() const
  {
    return self->lockdownTimer > 0;
  }
  //--------------------------------------------- IS MAELSTROMMED --------------------------------------------
  bool UnitImpl::isMaelstrommed() const
  {
    return self->maelstromTimer > 0;
  }
  //--------------------------------------------- IS MORPHING ------------------------------------------------
  bool UnitImpl::isMorphing() const
  {
    return self->isMorphing;
  }
  //--------------------------------------------- IS MOVING --------------------------------------------------
  bool UnitImpl::isMoving() const
  {
    return self->isMoving;
  }
  //--------------------------------------------- IS PARASITED -----------------------------------------------
  bool UnitImpl::isParasited() const
  {
    return self->isParasited;
  }
  //--------------------------------------------- IS PATROLLING ----------------------------------------------
  bool UnitImpl::isPatrolling() const
  {
    return self->order == Orders::Patrol.getID();
  }
  //--------------------------------------------- IS PLAGUED -------------------------------------------------
  bool UnitImpl::isPlagued() const
  {
    return self->plagueTimer > 0;
  }
  //--------------------------------------------- IS REPAIRING -----------------------------------------------
  bool UnitImpl::isRepairing() const
  {
    return self->order == Orders::Repair1.getID() || self->order == Orders::Repair2.getID();
  }
  //--------------------------------------------- IS RESEARCHING ---------------------------------------------
  bool UnitImpl::isResearching() const
  {
    return self->order == Orders::ResearchTech.getID();
  }
  //--------------------------------------------- IS SELECTED ------------------------------------------------
  bool UnitImpl::isSelected() const
  {
    return self->isSelected;
  }
  //--------------------------------------------- IS SELECTED ------------------------------------------------
  bool UnitImpl::isSieged() const
  {
    return self->type == UnitTypes::Terran_Siege_Tank_Siege_Mode.getID();
  }
  //--------------------------------------------- IS STARTING ATTACK -----------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    return self->isStartingAttack;
  }
  //--------------------------------------------- IS STASISED ------------------------------------------------
  bool UnitImpl::isStasised() const
  {
    return self->stasisTimer > 0;
  }
  //--------------------------------------------- IS STIMMED -------------------------------------------------
  bool UnitImpl::isStimmed() const
  {
    return self->stimTimer > 0;
  }
  //--------------------------------------------- IS TRAINING ------------------------------------------------
  bool UnitImpl::isTraining() const
  {
    return self->isTraining;
  }
  //--------------------------------------------- IS UNDER STORM ---------------------------------------------
  bool UnitImpl::isUnderStorm() const
  {
    return self->isUnderStorm;
  }
  //--------------------------------------------- IS UNPOWERED -----------------------------------------------
  bool UnitImpl::isUnpowered() const
  {
    return self->isUnpowered;
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool UnitImpl::isUpgrading() const
  {
    return self->order == Orders::Upgrade.getID();
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible() const
  {
    return self->isVisible[Broodwar->self()->getID()];
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible(Player* player) const
  {
    if (player==NULL) return false;
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

#include "UnitImpl.h"

#include <math.h>
#include <limits>
#include <sstream>

#include <Util/Logger.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include <BWAPI/WeaponType.h>
#include "Command.h"
#include "DLLMain.h"

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>

namespace BWAPI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  UnitImpl::UnitImpl(BW::Unit* originalUnit,
                     u16 index)
      : getOriginalRawData(originalUnit)
      , index(index)
      , userSelected(false)
      , isAlive(false)
      , wasAlive(false)
      , wasAccessible(false)
      , wasVisible(false)
      , staticInformation(false)
      , lastType(UnitTypes::Unknown)
      , lastPlayer(NULL)
      , nukeDetected(false)
      , lastGroundWeaponCooldown(0)
      , lastAirWeaponCooldown(0)
      , startingAttack(false)
      , lastOrderFrame(0)
      , id(-1)
      , clientInfo(NULL)
  {
    self = &data;
  }
  //--------------------------------------------- DESTRUCTOR -------------------------------------------------
  UnitImpl::~UnitImpl()
  {
  }
  //--------------------------------------------- REVOKE ID --------------------------------------------------
  void UnitImpl::setID(int newID)
  {
    id = newID;
  }
  //--------------------------------------------- GET ID -----------------------------------------------------
  int UnitImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET PLAYER -------------------------------------------------
  Player* UnitImpl::getPlayer() const
  {
    return Broodwar->getPlayer(self->player);
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
    for (int i = 0; i < self->trainingQueueCount; ++i)
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
    return UpgradeType(self->upgrade);
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
    return Broodwar->getUnit(self->buildUnit);
  }
  //--------------------------------------------- GET TARGET -------------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    return Broodwar->getUnit(self->target);
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
    return Broodwar->getUnit(self->orderTarget);
  }
  //--------------------------------------------- GET SECONDARY ORDER ID -------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return Order(self->secondaryOrder);
  }
  //--------------------------------------------- GET RALLY POSITION -----------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    return Position(self->rallyPositionX,self->rallyPositionY);
  }
  //--------------------------------------------- GET RALLY UNIT ---------------------------------------------
  Unit* UnitImpl::getRallyUnit() const
  {
    return Broodwar->getUnit(self->rallyUnit);
  }
  //--------------------------------------------- GET ADDON --------------------------------------------------
  Unit* UnitImpl::getAddon() const
  {
    return Broodwar->getUnit(self->addon);
  }
  //--------------------------------------------- GET NYDUS EXIT ---------------------------------------------
  Unit* UnitImpl::getNydusExit() const
  {
    return Broodwar->getUnit(self->nydusExit);
  }
  //--------------------------------------------- GET TRANSPORT ----------------------------------------------
  Unit* UnitImpl::getTransport() const
  {
    return Broodwar->getUnit(self->transport);
  }
  //--------------------------------------------- GET LOADED UNITS -------------------------------------------
  std::set<Unit*> UnitImpl::getLoadedUnits() const
  {
    return loadedUnits;
  }
  //--------------------------------------------- GET CARRIER ------------------------------------------------
  Unit* UnitImpl::getCarrier() const
  {
    return Broodwar->getUnit(self->carrier);
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
    return Broodwar->getUnit(self->hatchery);
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
    if (getType().getRace() != Races::Terran)
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
    return getType().getRace() == Races::Terran &&
           self->isCompleted &&
           self->hitPoints>self->lastHitPoints;
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
  //--------------------------------------------- IS DETECTED ------------------------------------------------
  bool UnitImpl::isDetected() const
  {
    return self->isDetected;
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

    if (self->order != Orders::Harvest1.getID()   &&
        self->order != Orders::Harvest2.getID()   &&
        self->order != Orders::MoveToGas.getID()  &&
        self->order != Orders::WaitForGas.getID() &&
        self->order != Orders::HarvestGas.getID() &&
        self->order != Orders::ReturnGas.getID()  &&
        self->order != Orders::ResetCollision.getID())
      return false;

    if (self->order == Orders::ResetCollision.getID())
      return self->carryResourceType == 1;

    //return true if BWOrder is WaitForGas, HarvestGas, or ReturnGas
    if (self->order == Orders::WaitForGas.getID() ||
        self->order == Orders::HarvestGas.getID() ||
        self->order == Orders::ReturnGas.getID())
      return true;

    //if BWOrder is MoveToGas, Harvest1, or Harvest2 we need to do some additional checks to make sure the unit is really gathering
    if (getTarget() &&
        getTarget()->exists() && 
        getTarget()->isCompleted() &&
        getTarget()->getPlayer() == getPlayer() &&
        getTarget()->getType() != UnitTypes::Resource_Vespene_Geyser &&
        (getTarget()->getType().isRefinery() || getTarget()->getType().isResourceDepot()))
      return true;
    if (getOrderTarget() &&
        getOrderTarget()->exists() && 
        getOrderTarget()->isCompleted() &&
        getOrderTarget()->getPlayer() == getPlayer() &&
        getOrderTarget()->getType() != UnitTypes::Resource_Vespene_Geyser &&
        (getOrderTarget()->getType().isRefinery() || getOrderTarget()->getType().isResourceDepot()))
      return true;
    return false;
  }
  //--------------------------------------------- IS GATHERING MINERALS --------------------------------------
  bool UnitImpl::isGatheringMinerals() const
  {
    if (!self->isGathering)
      return false;

    if (self->order != Orders::Harvest1.getID()        &&
        self->order != Orders::Harvest2.getID()        &&
        self->order != Orders::MoveToMinerals.getID()  &&
        self->order != Orders::WaitForMinerals.getID() &&
        self->order != Orders::MiningMinerals.getID()  &&
        self->order != Orders::ReturnMinerals.getID()  &&
        self->order != Orders::ResetCollision.getID())
      return false;

    if (self->order == Orders::ResetCollision.getID())
      return self->carryResourceType == 2;

    //return true if BWOrder is WaitForMinerals, MiningMinerals, or ReturnMinerals
    if (self->order == Orders::WaitForMinerals.getID() ||
        self->order == Orders::MiningMinerals.getID() ||
        self->order == Orders::ReturnMinerals.getID())
      return true;

    //if BWOrder is MoveToMinerals, Harvest1, or Harvest2 we need to do some additional checks to make sure the unit is really gathering
    if (getTarget() &&
        getTarget()->exists() &&
        (getTarget()->getType() == UnitTypes::Resource_Mineral_Field ||
            (getTarget()->isCompleted() &&
             getTarget()->getPlayer() == getPlayer() &&
             getTarget()->getType().isResourceDepot())))
      return true;
    if (getOrderTarget() &&
        getOrderTarget()->exists() &&
        (getOrderTarget()->getType() == UnitTypes::Resource_Mineral_Field ||
            (getOrderTarget()->isCompleted() &&
             getOrderTarget()->getPlayer() == getPlayer() &&
             getOrderTarget()->getType().isResourceDepot())))
      return true;
    return false;
  }
  //--------------------------------------------- IS HALLUCINATION -------------------------------------------
  bool UnitImpl::isHallucination() const
  {
    return self->isHallucination;
  }
  //--------------------------------------------- IS HOLDING POSITION ----------------------------------------
  bool UnitImpl::isHoldingPosition() const
  {
    return self->order == Orders::HoldPosition.getID();
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
    return self->order == Orders::Repair.getID();
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
    if ( !Broodwar->self() )
    {
      for(int i = 0; i < 9; ++i)
      {
        if (self->isVisible[i])
          return true;
      }
      return false;
    }
    return self->isVisible[Broodwar->self()->getID()];
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible(Player* player) const
  {
    if ( !player )
      return false;
    return self->isVisible[player->getID()];
  }
  //--------------------------------------------- IS STARTING ATTACK SEQUENCE --------------------------------
  bool UnitImpl::isStartingAttackSequence() const
  {
    checkAccessBool();
    u8 animState = 0;
    if ( getOriginalRawData->sprite && getOriginalRawData->sprite->mainGraphic )
      animState = getOriginalRawData->sprite->mainGraphic->anim;
    return animState == BW::Image::Anims::GndAttkInit || animState == BW::Image::Anims::AirAttkInit;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    userSelected = selectedState;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  double UnitImpl::getDistance(Unit* target) const
  {
    if ( !this->attemptAccess() || !target || !((UnitImpl*)target)->attemptAccess() )
      return std::numeric_limits<double>::infinity();

    if (this == target)
      return 0;

    const Unit* i = this;
    const Unit* j = target;
    double result = 0;
    if (i->getPosition().y() - i->getType().dimensionUp()   <= j->getPosition().y() + j->getType().dimensionDown() &&
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
        result = BWAPI::Position(i->getPosition().x() - i->getType().dimensionLeft(),
                                 i->getPosition().y() - i->getType().dimensionUp()).getApproxDistance(
                 BWAPI::Position(j->getPosition().x() + j->getType().dimensionRight(),
                                 j->getPosition().y() + j->getType().dimensionDown()));
      else
        result = BWAPI::Position(i->getPosition().x() - i->getType().dimensionLeft(),
                                 i->getPosition().y() + i->getType().dimensionDown()).getApproxDistance(
                 BWAPI::Position(j->getPosition().x() + j->getType().dimensionRight(),
                                 j->getPosition().y() - j->getType().dimensionUp()));
    }
    else
    {
      if (i->getPosition().y() > j->getPosition().y())
        result = BWAPI::Position(i->getPosition().x() + i->getType().dimensionRight(),
                                 i->getPosition().y() - i->getType().dimensionUp()).getApproxDistance(
                 BWAPI::Position(j->getPosition().x() - j->getType().dimensionLeft(),
                                 j->getPosition().y() + j->getType().dimensionDown()));
      else
        result = BWAPI::Position(i->getPosition().x() + i->getType().dimensionRight(),
                                 i->getPosition().y() + i->getType().dimensionDown()).getApproxDistance(
                 BWAPI::Position(j->getPosition().x() - j->getType().dimensionLeft(),
                                 j->getPosition().y() - j->getType().dimensionUp()));
    }
    if (result > 0)
      return result;
    return 0;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  double UnitImpl::getDistance(Position target) const
  {
    if (!this->attemptAccess()) return std::numeric_limits<double>::infinity();
    double result = 0;
    if (getPosition().y() - getType().dimensionUp() <= target.y() &&
        getPosition().y() + getType().dimensionDown() >= target.y())
    {
      if (getPosition().x() > target.x())
        result = getPosition().x() - getType().dimensionLeft()  - target.x();
      else
        result = target.x() - getPosition().x() - getType().dimensionLeft();
    }

    if (getPosition().x() - getType().dimensionLeft() <= target.x() &&
        getPosition().x() + getType().dimensionRight() >= target.x())
    {
      if (getPosition().y() > target.y())
        result = getPosition().y() - getType().dimensionUp()   - target.y();
      else
        result = target.y() - getPosition().y() - getType().dimensionUp();
    }

    if (this->getPosition().x() > target.x())
    {
      if (this->getPosition().y() > target.y())
        result = BWAPI::Position(getPosition().x() - getType().dimensionLeft(),
                                 getPosition().y() - getType().dimensionUp()).getDistance(target);
      else
        result = BWAPI::Position(getPosition().x() - getType().dimensionLeft(),
                                 getPosition().y() + getType().dimensionDown()).getDistance(target);
    }
    else
    {
      if (this->getPosition().y() > target.y())
        result = BWAPI::Position(getPosition().x() + getType().dimensionRight(),
                                 getPosition().y() - getType().dimensionUp()).getDistance(target);
      else
        result = BWAPI::Position(getPosition().x() + getType().dimensionRight(),
                                 getPosition().y() + getType().dimensionDown()).getDistance(target);
    }
    if (result > 0)
      return result;
    return 0;
  }
  //------------------------------------------- GET UPGRADE LEVEL --------------------------------------------
  int UnitImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    if (!this->attemptAccess()) return 0;

    int pId = _getPlayer->getID();
    int uId = upgrade.getID();
    if ( (uId  < 46 && BW::BWDATA_UpgradeLevelSC->level[pId][uId] == 0) ||
         (uId >= 46 && uId < UPGRADE_TYPE_COUNT && BW::BWDATA_UpgradeLevelBW->level[pId][uId - 46] == 0) ||
          uId >= UPGRADE_TYPE_COUNT )
      return 0;
    
    if (upgrade.whatUses().find(_getType) != upgrade.whatUses().end())
    {
      if ( uId < 46 )
        return BW::BWDATA_UpgradeLevelSC->level[pId][uId];
      else
        return BW::BWDATA_UpgradeLevelBW->level[pId][uId - 46];
    }
    return 0;
  }
  //---------------------------------------------- REGION CHECK ----------------------------------------------
  bool UnitImpl::hasPath(Position target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    BWAPI::Position srcPos = this->getPosition();

    if ( srcPos.x() >= 256*32 ||
         srcPos.y() >= 256*32 ||
         target.x() >= 256*32 ||
         target.y() >= 256*32 )
      return BroodwarImpl.setLastError(Errors::Unknown);

    if ( BW::BWDATA_SAIPathing )
    {
      u16 srcIdx = BW::BWDATA_SAIPathing->mapTileRegionId[srcPos.y()/32][srcPos.x()/32];
      u16 dstIdx = BW::BWDATA_SAIPathing->mapTileRegionId[target.y()/32][target.x()/32];

      u16 srcGroup = 0;
      u16 dstGroup = 0;
      if ( srcIdx & 0x2000 )
      {
        int minitilePosX = (srcPos.x()&0x1F)/8;
        int minitilePosY = (srcPos.y()&0x1F)/8;
        int minitileShift = minitilePosX + minitilePosY * 4;
        BW::split *t = &BW::BWDATA_SAIPathing->splitTiles[srcIdx&0x1FFF];
        if ( (t->minitileMask >> minitileShift) & 1 )
          srcGroup = BW::BWDATA_SAIPathing->regions[t->rgn2].groupIndex;
        else
          srcGroup = BW::BWDATA_SAIPathing->regions[t->rgn1].groupIndex;
      }
      else
      {
        srcGroup = BW::BWDATA_SAIPathing->regions[srcIdx].groupIndex;
      }

      if ( dstIdx & 0x2000 )
      {
        int minitilePosX = (target.x()&0x1F)/8;
        int minitilePosY = (target.y()&0x1F)/8;
        int minitileShift = minitilePosX + minitilePosY * 4;
        BW::split *t = &BW::BWDATA_SAIPathing->splitTiles[dstIdx&0x1FFF];
        if ( (t->minitileMask >> minitileShift) & 1 )
          dstGroup = BW::BWDATA_SAIPathing->regions[t->rgn2].groupIndex;
        else
          dstGroup = BW::BWDATA_SAIPathing->regions[t->rgn1].groupIndex;
      }
      else
      {
        dstGroup = BW::BWDATA_SAIPathing->regions[dstIdx].groupIndex;
      }

      if ( srcGroup == dstGroup )
        return true;
    }
    return BroodwarImpl.setLastError(Errors::Out_Of_Range);
  }
  bool UnitImpl::hasPath(Unit *target)
  {
    if ( !target )
      return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
    return hasPath(target->getPosition());
  }
  //---------------------------------------------- ORDER SELECT ----------------------------------------------
  void UnitImpl::orderSelect()
  {
    BW::Orders::Select sel = BW::Orders::Select(1, this);
    QueueGameCommand((PBYTE)&sel, sel.size);
  }
  //----------------------------------------------------------------------------------------------------------
  UnitImpl* UnitImpl::BWUnitToBWAPIUnit(BW::Unit* unit)
  {
    if ( !unit )
      return NULL;

    u16 index = (u16)( ((u32)unit - (u32)BW::BWDATA_UnitNodeTable) / 336) & 0x7FF;
    if (index > UNIT_ARRAY_MAX_LENGTH)
    {
      if (BroodwarImpl.invalidIndices.find(index) == BroodwarImpl.invalidIndices.end())
      {
        BroodwarImpl.newUnitLog->log("Error: Found new invalid unit index: %d, broodwar address: 0x%x",index,unit);
        BroodwarImpl.invalidIndices.insert(index);
      }
      return NULL;
    }
    return BroodwarImpl.getUnitFromIndex(index);
  }

  void UnitImpl::die()
  {
    //set pointers to null so we don't read information from unit table anymore
    getOriginalRawData = NULL;
    index              = 0xFFFF;
    userSelected       = false;
    isAlive            = false;
    wasAlive           = false;
    wasAccessible      = false;
    wasVisible         = false;
    nukeDetected       = false;
    lastType           = UnitTypes::Unknown;
    lastPlayer         = NULL;
    updateData();
  }

  /* canAccess returns true if the AI module is allowed to access the unit.
    If the unit is visible, returns true.
    If the unit is does not exist, returns false.
    If the unit is not visible and exists, returns true only if CompleteMapInformation is enabled.
  */
  bool UnitImpl::canAccess() const
  {
    if (!this->isAlive)
      return false;
    if (this->isVisible())
      return true;
    //if we get here, the unit exists but is not visible
    if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return true;
    /* neutral units visible during AIModule::onStart */
    if (Broodwar->getFrameCount() == 0)
      if (this->_getType.isNeutral() || this->_getPlayer->isNeutral())
        return true;
    return false;
  }
  bool UnitImpl::canAccessDetected() const
  {
    if (!canAccess())
      return false;
    if (this->_getPlayer == BroodwarImpl.self())
      return true;
    if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return true;
    /* neutral units visible during AIModule::onStart */
    if (Broodwar->getFrameCount() == 0)
      if (this->_getType.isNeutral())
        return true;
    return self->isDetected;

  }

  //returns true if canAccess() is true and the unit is owned by self (or complete map info is turned on)
  bool UnitImpl::canAccessInside() const
  {
    if (!canAccessDetected())
      return false;
    if (this->_getPlayer == BroodwarImpl.self())
      return true;

    //If we get here, canAccess()==true but unit is not owned by self.
    //Return value depends on state of complete map info flag
    return BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation);
  }

  // calls canAccess, setting error codes as needed
  bool UnitImpl::attemptAccess() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->canAccess())
      return true;
    return BroodwarImpl.setLastError(Errors::Unit_Not_Visible);
  }

  //---------------------------------------------- UPDATE NEXT -----------------------------------------------
  UnitImpl* UnitImpl::getNext() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->next);
  }
  //----------------------------------------------- GET INDEX ------------------------------------------------
  u16 UnitImpl::getIndex() const
  {
    return this->index;
  }
  //------------------------------------- INITIAL INFORMATION FUNCTIONS --------------------------------------
  void UnitImpl::saveInitialInformation()
  {
    this->staticInformation  = true;
    this->staticType         = this->_getType;
    this->staticPosition     = this->_getPosition;
    this->staticResources    = this->_getResources;
    this->staticHitPoints    = this->_getHitPoints;
  }
  UnitType UnitImpl::getInitialType() const
  {
    if (this->staticInformation)
      return this->staticType;
    return UnitTypes::Unknown;
  }
  Position UnitImpl::getInitialPosition() const
  {
    if (this->staticInformation)
      return this->staticPosition;
    return Positions::Unknown;
  }
  TilePosition UnitImpl::getInitialTilePosition() const
  {
    if (this->staticInformation)
      return TilePosition(Position(staticPosition.x() - staticType.tileWidth() * TILE_SIZE / 2,
                                   staticPosition.y() - staticType.tileHeight() * TILE_SIZE / 2));
    return TilePositions::Unknown;
  }
  int UnitImpl::getInitialResources() const
  {
    if (this->staticInformation)
      return this->staticResources;
    return 0;
  }
  int UnitImpl::getInitialHitPoints() const
  {
    if (this->staticInformation)
      return this->staticHitPoints;
    return 0;
  }
  //----------------------------------------- GET LAST ORDER FRAME -------------------------------------------
  int UnitImpl::getLastOrderFrame()
  {
    return this->lastOrderFrame;
  }
  //------------------------------------------ SET/GET CLIENT INFO -------------------------------------------
  void UnitImpl::setClientInfo(void* clientinfo)
  {
    this->clientInfo = clientinfo;
  }
  void* UnitImpl::getClientInfo() const
  {
    return clientInfo;
  }
  //------------------------------------------ GET RESOURCE GROUP --------------------------------------------
  int UnitImpl::getResourceGroup()
  {
    if ( this->getType().isResourceContainer() )
      return this->getOriginalRawData->building.resource.resourceGroup;
    return 0;
  }
  TechType UnitImpl::getCloakingTech()
  {
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_Wraith:
    case BW::UnitID::Terran_Hero_TomKazansky:
      return TechTypes::Cloaking_Field;
    case BW::UnitID::Terran_Ghost:
    case BW::UnitID::Terran_Hero_AlexeiStukov:
    case BW::UnitID::Terran_Hero_SamirDuran:
    case BW::UnitID::Terran_Hero_SarahKerrigan:
    case BW::UnitID::Zerg_Hero_InfestedDuran:
    case BW::UnitID::Zerg_Hero_InfestedKerrigan:
      return TechTypes::Personnel_Cloaking;
    default:
      return TechTypes::None;
    }
  }
  //------------------------------------------ CAN ISSUE COMMAND ---------------------------------------------
  bool UnitImpl::canIssueCommand(UnitCommand c)
  {
    // Basic header
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    BWAPI::UnitCommandType ct = c.type;

    // Global can be ordered check
    if ( this->isLockedDown() || this->isMaelstrommed() || this->isStasised() || this->isUnpowered() )
      return BroodwarImpl.setLastError(Errors::Unit_Busy);

    // Hallucination check
    if ( this->isHallucination()                      &&
         UnitCommandTypes::Attack_Move          != ct &&
         UnitCommandTypes::Attack_Unit          != ct &&
         UnitCommandTypes::Move                 != ct &&
         UnitCommandTypes::Patrol               != ct &&
         UnitCommandTypes::Hold_Position        != ct &&
         UnitCommandTypes::Stop                 != ct &&
         UnitCommandTypes::Follow               != ct &&
         UnitCommandTypes::Load                 != ct &&
         UnitCommandTypes::Right_Click_Position != ct &&
         UnitCommandTypes::Right_Click_Unit     != ct )
       return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

    // valid target check
    if ( UnitCommandTypes::Attack_Unit      == ct ||
         UnitCommandTypes::Set_Rally_Unit   == ct ||
         UnitCommandTypes::Follow           == ct ||
         UnitCommandTypes::Gather           == ct ||
         UnitCommandTypes::Repair           == ct ||
         UnitCommandTypes::Load             == ct ||
         UnitCommandTypes::Unload           == ct ||
         UnitCommandTypes::Right_Click_Unit == ct ||
         UnitCommandTypes::Use_Tech_Unit    == ct )
    {
      if ( !c.target || !((UnitImpl*)c.target)->attemptAccess() )
        return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
    }

    // Build/Train requirements
    if ( UnitCommandTypes::Build       == ct ||
         UnitCommandTypes::Build_Addon == ct ||
         UnitCommandTypes::Morph       == ct ||
         UnitCommandTypes::Train       == ct )
    {
      UnitType uType = UnitType(c.extra);
      if ( !Broodwar->canMake(this, uType) )
        return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

      if ( this->isConstructing() || 
           !this->isCompleted()   || 
           (this->_getType.isFlyingBuilding() && this->isLifted()) ||
           (UnitCommandTypes::Train != ct && this->_getType.isBuilding() && !this->isIdle()) )
        return BroodwarImpl.setLastError(Errors::Unit_Busy);

      if ( UnitCommandTypes::Build == ct )
      {
        if ( !uType.isBuilding() )
          return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

        if ( !uType.isAddon() && !Broodwar->canBuildHere(this, BWAPI::TilePosition(c.x, c.y), uType) )
          return false;
      }
      else if ( UnitCommandTypes::Build_Addon == ct )
      {
        if ( !uType.isAddon() )
          return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      }
      else
      {
        if ( this->_getType.producesLarva() && uType.whatBuilds().first == UnitTypes::Zerg_Larva && connectedUnits.size() == 0 )
          return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
      }
    } // build/train

    // Research/Upgrade requirements
    if ( UnitCommandTypes::Research == ct || UnitCommandTypes::Upgrade == ct )
    {
      if ( this->isLifted() || !this->isIdle() || !this->isCompleted() )
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
      if ( UnitCommandTypes::Research == ct && !Broodwar->canResearch(this,TechType(c.extra)) )
        return false;
      if ( UnitCommandTypes::Upgrade == ct && !Broodwar->canUpgrade(this,UpgradeType(c.extra)) )
        return false;
    } // research/upgrade

    // Set Rally 
    if ( UnitCommandTypes::Set_Rally_Position == ct || UnitCommandTypes::Set_Rally_Unit == ct )
    {
      if ( !this->_getType.canProduce() )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    } // rally

    // Move
    if ( UnitCommandTypes::Move          == ct || 
         UnitCommandTypes::Patrol        == ct ||
         UnitCommandTypes::Hold_Position == ct ||
         UnitCommandTypes::Stop          == ct ||
         UnitCommandTypes::Follow        == ct )
    {
      if ( this->_getType.isBuilding() && !isLifted() && (!_getType.canAttack() || ct == UnitCommandTypes::Patrol || ct == UnitCommandTypes::Follow) )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    } // move

    // Gather
    if ( UnitCommandTypes::Gather == ct )
    {
      UnitType uType = c.target->getType();
      if ( !this->_getType.isWorker() ||
           !uType.isResourceContainer() ||
           uType == UnitTypes::Resource_Vespene_Geyser )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

      if ( this->getOriginalRawData->worker.powerup || !c.target->isCompleted() )
        return BroodwarImpl.setLastError(Errors::Unit_Busy);
    } // gather

    // return cargo
    if ( UnitCommandTypes::Return_Cargo == ct )
    {
      if ( !this->_getType.isWorker() )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

      if ( !this->isCarryingGas() && !this->isCarryingMinerals() )
        return BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
    } // return

    // Repair
    if ( UnitCommandTypes::Repair == ct )
    {
      UnitType targType = c.target->getType();
      if ( this->_getType != BWAPI::UnitTypes::Terran_SCV ||
           targType.getRace() != BWAPI::Races::Terran ||
           !targType.isMechanical() )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    } // repair

    // Ability requirements
    if ( UnitCommandTypes::Use_Tech          == ct ||
         UnitCommandTypes::Use_Tech_Position == ct ||
         UnitCommandTypes::Use_Tech_Unit     == ct ||
         UnitCommandTypes::Burrow            == ct ||
         UnitCommandTypes::Cloak             == ct ||
         UnitCommandTypes::Siege             == ct )
    {
      // Retrieve the tech type
      BWAPI::TechType tech = TechType(c.extra);
      if ( UnitCommandTypes::Burrow == ct )
        tech = BWAPI::TechTypes::Burrowing;
      else if ( UnitCommandTypes::Cloak == ct )
        tech = this->getCloakingTech();
      else if ( UnitCommandTypes::Siege == ct )
        tech = BWAPI::TechTypes::Tank_Siege_Mode;

      // do checks
      if ( !Broodwar->self()->hasResearched(tech) && this->_getType != UnitTypes::Zerg_Lurker )
        return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

      if ( this->getEnergy() < tech.energyUsed() )
        return BroodwarImpl.setLastError(Errors::Insufficient_Energy);

      if ( tech == TechTypes::Burrowing )
      {
        if ( !this->_getType.isBurrowable() )
          return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      }
      else if ( tech.whatUses().find(this->_getType) == tech.whatUses().end() )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

      if ( tech == TechTypes::Spider_Mines && this->getSpiderMineCount() <= 0 )
        return BroodwarImpl.setLastError(Errors::Insufficient_Ammo);

      //if ( tech == TechTypes::Nuclear_Strike && this->_getPlayer->completedUnitCount(UnitTypes::Terran_Nuclear_Missile) > 0 )
      //  return BroodwarImpl.setLastError(Errors::Insufficient_Ammo);

      if ( UnitCommandTypes::Burrow == ct && this->isBurrowed() )
        return false;

      if ( UnitCommandTypes::Cloak == ct && this->isCloaked() )
        return false;

      if ( UnitCommandTypes::Siege == ct && this->isSieged() )
        return false;

      if ( tech == TechTypes::None || tech == TechTypes::Unknown )
        return false;
    } // ability

    // Unburrow
    if ( UnitCommandTypes::Unburrow == ct )
    {
      if ( !this->getType().isBurrowable() )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if ( !this->isBurrowed() )
        return false;
    } // unburrow

    // Decloak
    if ( UnitCommandTypes::Decloak == ct )
    {
      if ( getCloakingTech() == TechTypes::None )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if ( !this->isCloaked() )
        return false;
    } // decloak

    // Unsiege
    if ( UnitCommandTypes::Unsiege == ct )
    {
      int tId = _getType.getID();
      if ( tId != BW::UnitID::Terran_SiegeTankSiegeMode &&
           tId != BW::UnitID::Terran_Hero_EdmundDukeS )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      if ( !this->isSieged() )
        return false;
    } // unsiege

    // lift/land
    if ( UnitCommandTypes::Lift == ct || UnitCommandTypes::Land == ct )
    {
      if ( !this->_getType.isFlyingBuilding() )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

      if ( UnitCommandTypes::Lift == ct && this->isLifted() )
        return false;

      if ( UnitCommandTypes::Land == ct && !this->isLifted() )
        return false;
    } // lift/land

    // unload
    if ( UnitCommandTypes::Unload              == ct ||
         UnitCommandTypes::Unload_All          == ct ||
         UnitCommandTypes::Unload_All_Position == ct )
    {
      if ( this->loadedUnits.size() == 0 )
        return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);

      if ( this->_getType.spaceProvided() <= 0 )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);

      if ( this->_getType == UnitTypes::Zerg_Overlord && this->getPlayer()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0 )
        return BroodwarImpl.setLastError(Errors::Insufficient_Tech);

      if ( UnitCommandTypes::Unload == ct )
      {
        bool canUnload = false;
        foreach ( BWAPI::Unit *u, this->loadedUnits )
        {
          if ( c.target == u )
          {
            canUnload = true;
            break;
          }
        }
        if ( !canUnload )
          return BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
      }
    } // unload

    // Halt construction
    if ( UnitCommandTypes::Halt_Construction == ct && this->getOrder() != Orders::ConstructingBuilding )
      return false;

    // Cancel construction
    if ( UnitCommandTypes::Cancel_Construction == ct )
    {
      if ( this->isCompleted() )
        return false;
      if ( !this->_getType.isBuilding() )
        return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    } // cancel construct

    // cancel addon
    if ( UnitCommandTypes::Cancel_Addon == ct && (!this->getAddon() || this->getAddon()->isCompleted()) )
      return false;

    // cancel train
    if ( UnitCommandTypes::Cancel_Train == ct && !isTraining() )
      return false;
    if ( UnitCommandTypes::Cancel_Train_Slot == ct && (!isTraining() || this->getTrainingQueue().size() <= (unsigned int)c.extra) )
      return false;

    // cancel morph
    if ( UnitCommandTypes::Cancel_Morph == ct && !isMorphing() )
      return false;

    // cancel research
    if ( UnitCommandTypes::Cancel_Research == ct && this->getOrder() != Orders::ResearchTech )
      return false;

    // cancel upgrade
    if ( UnitCommandTypes::Cancel_Upgrade == ct && this->getOrder() != Orders::Upgrade )
      return false;


    return true;
  }
};

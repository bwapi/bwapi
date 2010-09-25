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
      , animState(0)
      , lastGroundWeaponCooldown(0)
      , lastAirWeaponCooldown(0)
      , startingAttack(false)
  {
    self = &data;
    id = -1;
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
    return getType().getRace()==Races::Terran &&
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
    return this->animState == BW::Image::Anims::GndAttkInit || this->animState == BW::Image::Anims::AirAttkInit;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    userSelected = selectedState;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  double UnitImpl::getDistance(Unit* target) const
  {
    if (!this->attemptAccess()) return std::numeric_limits<double>::infinity();
    if (!((UnitImpl*)target)->attemptAccess())
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
  //-------------------------------------------- ORDER Issue Command -----------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    //call the appropriate command function based on the command type
    if      (command.type == UnitCommandTypes::Attack_Move)
      return attackMove(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Attack_Unit)
      return attackUnit(command.target);
    else if (command.type == UnitCommandTypes::Build)
        return build(TilePosition(command.x,command.y),UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Build_Addon)
        return buildAddon(UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Train)
        return train(UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Morph)
        return morph(UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Research)
        return research(TechType(command.extra));
    else if (command.type == UnitCommandTypes::Upgrade)
        return upgrade(UpgradeType(command.extra));
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
        return setRallyPosition(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
        return setRallyUnit(command.target);
    else if (command.type == UnitCommandTypes::Move)
        return move(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Patrol)
        return patrol(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Hold_Position)
        return holdPosition();
    else if (command.type == UnitCommandTypes::Stop)
        return stop();
    else if (command.type == UnitCommandTypes::Follow)
        return follow(command.target);
    else if (command.type == UnitCommandTypes::Gather)
        return gather(command.target);
    else if (command.type == UnitCommandTypes::Return_Cargo)
        return returnCargo();
    else if (command.type == UnitCommandTypes::Repair)
        return repair(command.target);
    else if (command.type == UnitCommandTypes::Burrow)
        return burrow();
    else if (command.type == UnitCommandTypes::Unburrow)
        return unburrow();
    else if (command.type == UnitCommandTypes::Cloak)
        return cloak();
    else if (command.type == UnitCommandTypes::Decloak)
        return decloak();
    else if (command.type == UnitCommandTypes::Siege)
        return siege();
    else if (command.type == UnitCommandTypes::Unsiege)
        return unsiege();
    else if (command.type == UnitCommandTypes::Lift)
        return lift();
    else if (command.type == UnitCommandTypes::Land)
        return land(TilePosition(command.x,command.y));
    else if (command.type == UnitCommandTypes::Load)
        return load(command.target);
    else if (command.type == UnitCommandTypes::Unload)
        return unload(command.target);
    else if (command.type == UnitCommandTypes::Unload_All)
        return unloadAll();
    else if (command.type == UnitCommandTypes::Unload_All_Position)
        return unloadAll(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Right_Click_Position)
        return rightClick(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
        return rightClick(command.target);
    else if (command.type == UnitCommandTypes::Halt_Construction)
        return haltConstruction();
    else if (command.type == UnitCommandTypes::Cancel_Construction)
        return cancelConstruction();
    else if (command.type == UnitCommandTypes::Cancel_Addon)
        return cancelAddon();
    else if (command.type == UnitCommandTypes::Cancel_Train)
        return cancelTrain();
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
        return cancelTrain(command.extra);
    else if (command.type == UnitCommandTypes::Cancel_Morph)
        return cancelMorph();
    else if (command.type == UnitCommandTypes::Cancel_Research)
        return cancelResearch();
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
        return cancelUpgrade();
    else if (command.type == UnitCommandTypes::Use_Tech)
        return useTech(TechType(command.extra));
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
        return useTech(TechType(command.extra),Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
        return useTech(TechType(command.extra),command.target);
    BroodwarImpl.setLastError(Errors::Unknown);
    return false;
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool UnitImpl::attackMove(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::AttackMove), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::attackMove(this,target)));
    return true;
  }
  //--------------------------------------------- ATTACK UNIT ------------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    WeaponType weapon = this->getType().groundWeapon();
    bool targetInAir = (target->isLifted() || target->getType().isFlyer());
    if (targetInAir)
      weapon=this->getType().airWeapon();

    bool canAttack = (weapon!=WeaponTypes::None);
    if (getType() == UnitTypes::Protoss_Reaver && getScarabCount()>0 && !targetInAir)
      canAttack = true;
    if (getType() == UnitTypes::Protoss_Carrier && getInterceptorCount() > 0)
      canAttack = true;
    if (!canAttack)
    {
      BroodwarImpl.setLastError(Errors::Unable_To_Hit);
      return false;
    }
    if (!this->getType().canMove())
    {
      if (this->getDistance(target)>weapon.maxRange() ||
          this->getDistance(target)<weapon.minRange())
      {
        BroodwarImpl.setLastError(Errors::Out_Of_Range);
        return false;
      }
    }
    this->orderSelect();
    switch ( getType().getID() )
    {
    case BW::UnitID::Protoss_Carrier:
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CarrierAttack1), sizeof(BW::Orders::Attack));
      break;
    case BW::UnitID::Protoss_Reaver:
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::ReaverAttack1), sizeof(BW::Orders::Attack));
      break;
    default:
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Attack1), sizeof(BW::Orders::Attack));
      break;
    }
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::attackUnit(this,target)));
    return true;
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool UnitImpl::build(TilePosition target, UnitType type1)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canMake(this,type1))
      return false;

    if (!type1.isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isConstructing() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }
    if (!type1.isAddon() && !Broodwar->canBuildHere(this,target,type1))
      return false;

    BW::UnitType type((u16)type1.getID());
    this->orderSelect();
    if (!type.isAddon())
      QueueGameCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeBuilding));
    else
      QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeAddon));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::build(this,target,type1)));
    return true;
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canMake(this,type1))
      return false;

    if (!type1.isAddon())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isConstructing() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }
    TilePosition target(getTilePosition().x()+4,getTilePosition().y()+1);
    target.makeValid();
    BW::UnitType type((u16)type1.getID());
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeAddon));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::buildAddon(this,type1)));
    return true;
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canMake(this,type1))
      return false;

    if (type1.isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isLifted() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    BW::UnitType type((u16)type1.getID());
    this->orderSelect();
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::train(this,type1)));
    switch ( this->_getType.getID() )
    {
    case BW::UnitID::Zerg_Larva:
    case BW::UnitID::Zerg_Mutalisk:
    case BW::UnitID::Zerg_Hydralisk:
      QueueGameCommand((PBYTE)&BW::Orders::UnitMorph(type), 3);
      break;
    case BW::UnitID::Zerg_Hatchery:
    case BW::UnitID::Zerg_Lair:
    case BW::UnitID::Zerg_Spire:
    case BW::UnitID::Zerg_CreepColony:
      QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph(type), 3);
      break;
    case BW::UnitID::Protoss_Carrier:
    case BW::UnitID::Protoss_Hero_Gantrithor:
    case BW::UnitID::Protoss_Reaver:
    case BW::UnitID::Protoss_Hero_Warbringer:
      QueueGameCommand((PBYTE)&BW::Orders::TrainFighter(), 1);
      break;
    default:
      QueueGameCommand((PBYTE)&BW::Orders::TrainUnit(type), 3);
      break;
    }
    return true;
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canMake(this,type))
      return false;

    if (type.isBuilding() != this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    BW::UnitType rawtype((u16)type.getID());
    if(type.isBuilding())
      QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph(rawtype), sizeof(BW::Orders::BuildingMorph));
    else
      QueueGameCommand((PBYTE)&BW::Orders::UnitMorph(rawtype), sizeof(BW::Orders::UnitMorph));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::morph(this,type)));
    return true;
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canResearch(this,tech))
      return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    u8 techenum = (u8)tech.getID();
    QueueGameCommand((PBYTE)&BW::Orders::Invent(BW::TechType(techenum)), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::research(this,tech)));
    return true;
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canUpgrade(this,upgrade))
      return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    u8 upgradeenum = (u8)upgrade.getID();
    QueueGameCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType(upgradeenum)), sizeof(BW::Orders::Upgrade));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::upgrade(this,upgrade)));
    return true;
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPosition(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::RallyPointTile), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::setRallyPosition(this,target)));
    return true;
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPointUnit), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::setRallyUnit(this,target)));
    return true;
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (this->getType().isBuilding() && !isLifted())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Move), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::move(this,target)));
    return true;
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (this->getType().isBuilding() && !isLifted())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::patrol(this,target)));
    return true;
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::holdPosition(this)));
    return true;
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    this->orderSelect();
    switch ( _getType.getID() )
    {
    case BW::UnitID::Protoss_Reaver:
    case BW::UnitID::Protoss_Hero_Warbringer:
      QueueGameCommand((PBYTE)&BW::Orders::ReaverStop(), sizeof(BW::Orders::ReaverStop));
      break;
    case BW::UnitID::Protoss_Carrier:
    case BW::UnitID::Protoss_Hero_Gantrithor:
      QueueGameCommand((PBYTE)&BW::Orders::CarrierStop(), sizeof(BW::Orders::CarrierStop));
      break;
    default:
      QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
      break;
    }
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::stop(this)));
    return true;
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::follow(this,target)));
    return true;
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (!this->getType().isWorker() || (!target->getType().isRefinery() && target->getType() != UnitTypes::Resource_Mineral_Field))
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Harvest1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::gather(this,target)));
    return true;
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isWorker())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!this->isCarryingGas() && !this->isCarryingMinerals())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::ReturnCargo(0), sizeof(BW::Orders::ReturnCargo));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::returnCargo(this)));
    return true;
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getType() != UnitTypes::Terran_SCV || target->getType().isOrganic())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::repair(this,target)));
    return true;
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType() != UnitTypes::Zerg_Lurker && !Broodwar->self()->hasResearched(TechTypes::Burrowing))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(!this->isBurrowed())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::burrow(this)));
    }
    return true;
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }

    if(this->isBurrowed())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unburrow(this)));
    }
    return true;
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    BWAPI::TechType tech;
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_Wraith:
    case BW::UnitID::Terran_Hero_TomKazansky:
      tech = TechTypes::Cloaking_Field;
      break;
    case BW::UnitID::Terran_Ghost:
    case BW::UnitID::Terran_Hero_AlexeiStukov:
    case BW::UnitID::Terran_Hero_SamirDuran:
    case BW::UnitID::Terran_Hero_SarahKerrigan:
    case BW::UnitID::Zerg_Hero_InfestedDuran:
    case BW::UnitID::Zerg_Hero_InfestedKerrigan:
      tech = TechTypes::Personnel_Cloaking;
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }

    if ( !Broodwar->self()->hasResearched(tech) )
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if ( this->getEnergy() < tech.energyUsed() )
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if(!this->isCloaked())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cloak(this)));
    }
    return true;
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_Wraith:
    case BW::UnitID::Terran_Hero_TomKazansky:
    case BW::UnitID::Terran_Ghost:
    case BW::UnitID::Terran_Hero_AlexeiStukov:
    case BW::UnitID::Terran_Hero_SamirDuran:
    case BW::UnitID::Terran_Hero_SarahKerrigan:
    case BW::UnitID::Zerg_Hero_InfestedDuran:
    case BW::UnitID::Zerg_Hero_InfestedKerrigan:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if ( this->isCloaked() )
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::decloak(this)));
    }
    return true;
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_SiegeTankTankMode:
    case BW::UnitID::Terran_Hero_EdmundDukeT:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!Broodwar->self()->hasResearched(TechTypes::Tank_Siege_Mode))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if ( !this->isSieged() )
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::siege(this)));
    }
    return true;
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_SiegeTankSiegeMode:
    case BW::UnitID::Terran_Hero_EdmundDukeS:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isSieged())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unsiege(this)));
    }
    return true;
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(!this->isLifted())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::lift(this)));
    }
    return true;
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(this->isLifted())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Land(BW::TilePosition((u16)target.x(), (u16)target.y()), BW::UnitType((u16)this->self->type)), sizeof(BW::Orders::Land));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::land(this,target)));
    }
    return true;
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    this->orderSelect();
    bool loaded = false;
    if (this->getType() == UnitTypes::Terran_Bunker)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::PickupBunker), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::PickupTransport), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (target->getType() == UnitTypes::Terran_Bunker || target->getType() == UnitTypes::Terran_Dropship || target->getType() == UnitTypes::Protoss_Shuttle || target->getType() == UnitTypes::Zerg_Overlord)
    {
      this->rightClick(target);
      loaded = true;
    }
    if (loaded)
    {
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::load(this,target)));
      return true;
    }
    //if neither this unit nor the target unit is a bunker, dropship, shuttle, or overlord, return false.
    BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    return false;
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::UnloadUnit((UnitImpl*)target), sizeof(BW::Orders::UnloadUnit));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unload(this,target)));
    return true;
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_Dropship:
    case BW::UnitID::Protoss_Shuttle:
    case BW::UnitID::Zerg_Overlord:
      return this->unloadAll(this->getPosition());
    case BW::UnitID::Terran_Bunker:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unloadAll(this)));
    return true;
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch( this->getType().getID() )
    {
    case BW::UnitID::Terran_Dropship:
    case BW::UnitID::Protoss_Shuttle:
    case BW::UnitID::Zerg_Overlord:
      break;
    case BW::UnitID::Terran_Bunker:
      return this->unloadAll();
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unloadAll(this,target)));
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::RightClick(BW::Position((u16)target.x(), (u16)target.y())), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::rightClick(this,target)));
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (!target->getPlayer()->isNeutral() && this->getPlayer()->isEnemy(target->getPlayer()))
    {
      WeaponType weapon = this->getType().groundWeapon();
      if (target->isLifted() || target->getType().isFlyer())
        weapon=this->getType().airWeapon();

      if (weapon == WeaponTypes::None)
      {
        BroodwarImpl.setLastError(Errors::Unable_To_Hit);
        return false;
      }
      if (!this->getType().canMove())
      {
        if (this->getDistance(target)>weapon.maxRange() ||
            this->getDistance(target)<weapon.minRange())
        {
          BroodwarImpl.setLastError(Errors::Out_Of_Range);
          return false;
        }
      }
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)target), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::rightClick(this,target)));
    return true;
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (this->getOrder() != Orders::ConstructingBuilding)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::haltConstruction(this)));
    return true;
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    if (this->isCompleted())
      return false;

    if (!this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelConstruction(this)));
    return true;
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !getAddon() || getAddon()->isCompleted() )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelAddon(), sizeof(BW::Orders::CancelAddon));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelAddon(this)));
    return true;
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!isTraining())
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelTrainLast(), sizeof(BW::Orders::CancelTrainLast));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelTrain(this)));
    return true;
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!isTraining() || (int)(this->getTrainingQueue().size()) <= slot)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelTrain((u8)slot), sizeof(BW::Orders::CancelTrain));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelTrain(this,slot)));
    return true;
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!isMorphing())
      return false;
    this->orderSelect();
    if (this->getType().isBuilding())
      QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    else
      QueueGameCommand((PBYTE)&BW::Orders::CancelUnitMorph(), sizeof(BW::Orders::CancelUnitMorph));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelMorph(this)));
    return true;
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (self->order != BW::OrderID::ResearchTech)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelResearch(), sizeof(BW::Orders::CancelResearch));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelResearch(this)));
    return true;
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (self->order != BW::OrderID::Upgrade)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelUpgrade(), sizeof(BW::Orders::CancelUpgrade));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelUpgrade(this)));
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->self()->hasResearched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy() < tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if (tech.whatUses().find(this->getType()) == tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Stimpacks:
        QueueGameCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech)));
        break;
      case BW::TechID::TankSiegeMode:
        if (this->isSieged())
          this->unsiege();
        else
          this->siege();
        break;
      case BW::TechID::PersonnelCloaking:
      case BW::TechID::CloakingField:
        if(this->isCloaked())
          this->decloak();
        else
          this->cloak();
        break;
      case BW::TechID::Burrowing:
        if(this->isBurrowed())
          this->unburrow();
        else
          this->burrow();
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    position.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->self()->hasResearched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy() < tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if (tech.whatUses().find(this->getType()) == tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    u8 order;
    switch (tech.getID())
    {
      case BW::TechID::DarkSwarm:
        order = BW::OrderID::DarkSwarm;
        break;
      case BW::TechID::DisruptionWeb:
        order = BW::OrderID::CastDisruptionWeb;
        break;
      case BW::TechID::EMPShockwave:
        order = BW::OrderID::EmpShockwave;
        break;
      case BW::TechID::Ensnare:
        order = BW::OrderID::Ensnare;
        break;
      case BW::TechID::NuclearStrike:
        order = BW::OrderID::NukePaint;
        break;
      case BW::TechID::Plague:
        order = BW::OrderID::Plague;
        break;
      case BW::TechID::PsionicStorm:
        order = BW::OrderID::PsiStorm;
        break;
      case BW::TechID::Recall:
        order = BW::OrderID::Teleport;
        break;
      case BW::TechID::ScannerSweep:
        order = BW::OrderID::PlaceScanner;
        break;
      case BW::TechID::SpiderMines:
        if ( this->getSpiderMineCount() <= 0 )
        {
          BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
          return false;
        }
        order = BW::OrderID::PlaceMine;
        break;
      case BW::TechID::StasisField:
        order = BW::OrderID::StasisField;
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), order), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,position)));
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->self()->hasResearched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy() < tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if (tech.whatUses().find(this->getType()) == tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    u8 order;
    switch (tech.getID())
    {
      case BW::TechID::Consume:
        order = BW::OrderID::Consume;
        break;
      case BW::TechID::DefensiveMatrix:
        order = BW::OrderID::DefensiveMatrix;
        break;
      case BW::TechID::Feedback:
        order = BW::OrderID::CastFeedback;
        break;
      case BW::TechID::Hallucination:
        order = BW::OrderID::Hallucination1;
        break;
      case BW::TechID::Healing:
        order = BW::OrderID::MedicHeal1;
        break;
      case BW::TechID::Infestation:
        order = BW::OrderID::InfestMine2;
        break;
      case BW::TechID::Irradiate:
        order = BW::OrderID::Irradiate;
        break;
      case BW::TechID::Lockdown:
        order = BW::OrderID::MagnaPulse;
        break;
      case BW::TechID::Maelstorm:
        order = BW::OrderID::CastMaelstrom;
        break;
      case BW::TechID::MindControl:
        order = BW::OrderID::CastMindControl;
        break;
      case BW::TechID::OpticalFlare:
        order = BW::OrderID::CastOpticalFlare;
        break;
      case BW::TechID::Parasite:
        order = BW::OrderID::CastParasite;
        break;
      case BW::TechID::Restoration:
        order = BW::OrderID::Restoration;
        break;
      case BW::TechID::SpawnBroodlings:
        order = BW::OrderID::SummonBroodlings;
        break;
      case BW::TechID::YamatoGun:
        order = BW::OrderID::FireYamatoGun1;
        break;
      case BW::TechID::ArchonWarp:
        QueueGameCommand((PBYTE)&BW::Orders::ShiftSelectSingle((UnitImpl*)target), sizeof(BW::Orders::ShiftSelectSingle));
        QueueGameCommand((PBYTE)&BW::Orders::MergeArchon(), sizeof(BW::Orders::MergeArchon));
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,target)));
        return true;
      case BW::TechID::DarkArchonMeld:
        QueueGameCommand((PBYTE)&BW::Orders::ShiftSelectSingle((UnitImpl*)target), sizeof(BW::Orders::ShiftSelectSingle));
        QueueGameCommand((PBYTE)&BW::Orders::MergeDarkArchon(), sizeof(BW::Orders::MergeDarkArchon));
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,target)));
        return true;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, order), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,target)));
    return true;
  }
  //---------------------------------------------- REGION CHECK ----------------------------------------------
  bool UnitImpl::hasPath(Position target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    BWAPI::TilePosition srcPos = this->getTilePosition();
    BWAPI::TilePosition dstPos = BWAPI::TilePosition(target);

    if ( BW::BWDATA_SAIPathing )
    {
      BW::region *srcRgn = getRegion(BW::BWDATA_SAIPathing->mapTileRegionId[srcPos.y()][srcPos.x()]);
      if ( srcRgn->isConnectedTo(BW::BWDATA_SAIPathing->mapTileRegionId[dstPos.y()][dstPos.x()]) )
        return true;
    }
    BroodwarImpl.setLastError(Errors::Out_Of_Range);
    return false;
  }
  //---------------------------------------------- ORDER SELECT ----------------------------------------------
  void UnitImpl::orderSelect()
  {
    BW::Unit* select[2];
    select[0] = this->getOriginalRawData;
    select[1] = NULL; //in case some piece of starcraft code assumes this array is null-terminated.
    BW::selectUnits(1, (BW::Unit**)(&select));
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
    BroodwarImpl.setLastError(Errors::Unit_Not_Visible);
    return false;
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
};

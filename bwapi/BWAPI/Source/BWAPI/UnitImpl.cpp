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
#include "TemplatesImpl.h"

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Order.h>
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
      , lastCommandFrame(0)
      , lastCommand()
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
  //--------------------------------------------- GET RESOURCE GROUP -----------------------------------------
  int UnitImpl::getResourceGroup() const
  {
    return self->resourceGroup;
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  double UnitImpl::getDistance(Unit* target) const
  {
    if ( !this->exists() || !target || !target->exists() )
      return std::numeric_limits<double>::infinity();

    if (this == target)
      return 0;

    return (double)computeDistance<UnitImpl>(this,target);
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  double UnitImpl::getDistance(Position target) const
  {
    if (!this->exists())
      return std::numeric_limits<double>::infinity();
    return (double)computeDistance<UnitImpl>(this,target);
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Unit* target) const
  {
    if ( !target )
      return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
    return hasPath(target->getPosition());
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Position target) const
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    BWAPI::Position srcPos = this->getPosition();

    if ( srcPos.x() >= Broodwar->mapWidth()*32 ||
         srcPos.y() >= Broodwar->mapHeight()*32 ||
         target.x() >= Broodwar->mapWidth()*32 ||
         target.y() >= Broodwar->mapHeight()*32 )
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
  //--------------------------------------------- GET LAST COMMAND FRAME -------------------------------------
  int UnitImpl::getLastCommandFrame() const
  {
    return this->lastCommandFrame;
  }
  //--------------------------------------------- GET LAST COMMAND -------------------------------------------
  UnitCommand UnitImpl::getLastCommand() const
  {
    return this->lastCommand;
  }
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
  int UnitImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    if (getPlayer()==NULL ||
        getPlayer()->getUpgradeLevel(upgrade) == 0 ||
        upgrade.whatUses().find(getType()) == upgrade.whatUses().end())
      return 0;
    return getPlayer()->getUpgradeLevel(upgrade);
  }
  //--------------------------------------------- GET INITIAL TYPE -------------------------------------------
  UnitType UnitImpl::getInitialType() const
  {
    if (this->staticInformation)
      return this->staticType;
    return UnitTypes::Unknown;
  }
  //--------------------------------------------- GET INITIAL POSITION ---------------------------------------
  Position UnitImpl::getInitialPosition() const
  {
    if (this->staticInformation)
      return this->staticPosition;
    return Positions::Unknown;
  }
  //--------------------------------------------- GET INITIAL TILE POSITION ----------------------------------
  TilePosition UnitImpl::getInitialTilePosition() const
  {
    if (this->staticInformation)
      return TilePosition(Position(staticPosition.x() - staticType.tileWidth() * TILE_SIZE / 2,
                                   staticPosition.y() - staticType.tileHeight() * TILE_SIZE / 2));
    return TilePositions::Unknown;
  }
  //--------------------------------------------- GET INITIAL HIT POINTS -------------------------------------
  int UnitImpl::getInitialHitPoints() const
  {
    if (this->staticInformation)
      return this->staticHitPoints;
    return 0;
  }
  //--------------------------------------------- GET INITIAL RESOURCES --------------------------------------
  int UnitImpl::getInitialResources() const
  {
    if (this->staticInformation)
      return this->staticResources;
    return 0;
  }
  //--------------------------------------------- GET KILL COUNT ---------------------------------------------
  int UnitImpl::getKillCount() const
  {
    return self->killCount;
  }
  //--------------------------------------------- GET ACID SPORE COUNT ---------------------------------------
  int UnitImpl::getAcidSporeCount() const
  {
    return self->acidSporeCount;
  }
  //--------------------------------------------- GET INTERCEPTOR COUNT --------------------------------------
  int UnitImpl::getInterceptorCount() const
  {
    if ( this->getType() != UnitTypes::Protoss_Carrier && this->getType() != UnitTypes::Hero_Gantrithor )
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
  //--------------------------------------------- GET NYDUS EXIT ---------------------------------------------
  Unit* UnitImpl::getPowerUp() const
  {
    return Broodwar->getUnit(self->powerUp);
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
    if ( getType() != UnitTypes::Protoss_Carrier && getType() != UnitTypes::Hero_Gantrithor )
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
  //--------------------------------------------- IS ATTACK FRAME --------------------------------------------
  bool UnitImpl::isAttackFrame() const
  {
    return self->isAttackFrame;
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
           self->hitPoints > self->lastHitPoints;
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
  //--------------------------------------------- IS INTERRUPTIBLE -------------------------------------------
  bool UnitImpl::isInterruptible() const
  {
    return self->isInterruptible;
  }
  //--------------------------------------------- IS INVINCIBLE ----------------------------------------------
  bool UnitImpl::isInvincible() const
  {
    return self->isInvincible;
  }
  //--------------------------------------------- IS IN WEAPON RANGE -----------------------------------------
  bool UnitImpl::isInWeaponRange(Unit *target) const
  {
    if ( !exists() || !target || !target->exists() || this == target )
      return false;

    UnitType thisType = this->getType();
    UnitType targType = target->getType();

    WeaponType wpn = thisType.groundWeapon();
    int minRange = wpn.minRange();
    int maxRange = getPlayer()->groundWeaponMaxRange(thisType);
    if ( targType.isFlyer() || target->isLifted() )
    {
      wpn = thisType.airWeapon();
      minRange = wpn.minRange();
      maxRange = getPlayer()->airWeaponMaxRange(thisType);
    }
    if ( wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown )
      return false;

    int distance = computeDistance<UnitImpl>(this,target);
    return minRange < distance && maxRange >= distance;
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
    return self->type  == BW::UnitID::Terran_SiegeTankSiegeMode ||
           self->type  == BW::UnitID::Terran_Hero_EdmundDukeS;
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
  //--------------------------------------------- IS STUCK ---------------------------------------------------
  bool UnitImpl::isStuck() const
  {
    return self->isStuck;
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
  //------------------------------------------ SET/GET CLIENT INFO -------------------------------------------
  void UnitImpl::setClientInfo(void* clientinfo)
  {
    this->clientInfo = clientinfo;
  }
  void* UnitImpl::getClientInfo() const
  {
    return clientInfo;
  }
};

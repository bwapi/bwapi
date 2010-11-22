#include <BWAPI.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "TemplatesImpl.h"
#include "Command.h"
#include <Util\Foreach.h>

#include <string>
namespace BWAPI
{
  UnitImpl::UnitImpl(int id)
    : clientInfo(NULL)
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
    lastOrderFrame = 0;
    clientInfo = NULL;
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
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Unit *target) const
  {
    if ( !target )
      return ((GameImpl*)Broodwar)->setLastError(Errors::Unit_Does_Not_Exist);
    return hasPath(target->getPosition());
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Position target) const
  {
    ((GameImpl*)Broodwar)->setLastError(Errors::None);
    if (!exists()) return false;

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    const GameData* gameData = ((GameImpl*)Broodwar)->getGameData();

    BWAPI::Position srcPos = this->getPosition();

    if ( srcPos.x() >= Broodwar->mapWidth()*32 ||
         srcPos.y() >= Broodwar->mapHeight()*32 ||
         target.x() >= Broodwar->mapWidth()*32 ||
         target.y() >= Broodwar->mapHeight()*32 )
      return ((GameImpl*)Broodwar)->setLastError(Errors::Unknown);

    if ( gameData )
    {
      unsigned short srcIdx = gameData->mapTileRegionId[srcPos.x()/32][srcPos.y()/32];
      unsigned short dstIdx = gameData->mapTileRegionId[target.x()/32][target.y()/32];

      unsigned short srcGroup = 0;
      unsigned short dstGroup = 0;
      if ( srcIdx & 0x2000 )
      {
        int minitilePosX = (srcPos.x()&0x1F)/8;
        int minitilePosY = (srcPos.y()&0x1F)/8;
        int minitileShift = minitilePosX + minitilePosY * 4;
        unsigned short miniTileMask = gameData->mapSplitTilesMiniTileMask[srcIdx&0x1FFF];
        unsigned short rgn1         = gameData->mapSplitTilesRegion1[srcIdx&0x1FFF];
        unsigned short rgn2         = gameData->mapSplitTilesRegion2[srcIdx&0x1FFF];
        if ( (miniTileMask >> minitileShift) & 1 )
          srcGroup = gameData->regionGroupIndex[rgn2];
        else
          srcGroup = gameData->regionGroupIndex[rgn1];
      }
      else
      {
        srcGroup = gameData->regionGroupIndex[srcIdx];
      }

      if ( dstIdx & 0x2000 )
      {
        int minitilePosX = (target.x()&0x1F)/8;
        int minitilePosY = (target.y()&0x1F)/8;
        int minitileShift = minitilePosX + minitilePosY * 4;

        unsigned short miniTileMask = gameData->mapSplitTilesMiniTileMask[dstIdx&0x1FFF];
        unsigned short rgn1         = gameData->mapSplitTilesRegion1[dstIdx&0x1FFF];
        unsigned short rgn2         = gameData->mapSplitTilesRegion2[dstIdx&0x1FFF];

        if ( (miniTileMask >> minitileShift) & 1 )
          dstGroup = gameData->regionGroupIndex[rgn2];
        else
          dstGroup = gameData->regionGroupIndex[rgn1];
      }
      else
      {
        dstGroup = gameData->regionGroupIndex[dstIdx];
      }

      if ( srcGroup == dstGroup )
        return true;
    }
    return ((GameImpl*)Broodwar)->setLastError(Errors::Out_Of_Range);
  }
  //--------------------------------------------- GET LAST ORDER FRAME ---------------------------------------
  int UnitImpl::getLastOrderFrame() const
  {
    return this->lastOrderFrame;
  }
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
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
  //--------------------------------------------- GET POWER UP -----------------------------------------------
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
    if (getTarget() != NULL &&
        getTarget()->exists() && 
        getTarget()->isCompleted() &&
        getTarget()->getPlayer() == getPlayer() &&
        getTarget()->getType() != UnitTypes::Resource_Vespene_Geyser &&
        (getTarget()->getType().isRefinery() || getTarget()->getType().isResourceDepot()))
      return true;
    if (getOrderTarget() != NULL &&
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
    if (getTarget() != NULL &&
        getTarget()->exists() &&
        (getTarget()->getType() == UnitTypes::Resource_Mineral_Field ||
            (getTarget()->isCompleted() &&
             getTarget()->getPlayer() == getPlayer() &&
             getTarget()->getType().isResourceDepot())))
      return true;
    if (getOrderTarget() != NULL &&
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
    if (Broodwar->self()==NULL)
    {
      for(int i=0;i<9;i++)
      {
        if (self->isVisible[i]) return true;
      }
      return false;
    }
    return self->isVisible[Broodwar->self()->getID()];
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible(Player* player) const
  {
    if (player==NULL) return false;
    return self->isVisible[player->getID()];
  }
  //--------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
  bool UnitImpl::canIssueCommand(UnitCommand command) const
  {
    return Templates::canIssueCommand<class GameImpl, class PlayerImpl, class UnitImpl>(this,command);
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    if (!canIssueCommand(command))
      return false;

    command.unit = this;

    if (command.type == UnitCommandTypes::Train ||
        command.type == UnitCommandTypes::Morph)
      if (getType().producesLarva() && UnitType(command.extra).whatBuilds().first == UnitTypes::Zerg_Larva )
        command.unit = (UnitImpl*)(*getLarva().begin());

    BWAPIC::UnitCommand c;
    c.type=command.type;
    c.unitIndex=command.unit->getID();
    if (command.target!=NULL)
      c.targetIndex=command.target->getID();
    else
      c.targetIndex=-1;
    c.x=command.x;
    c.y=command.y;
    c.extra=command.extra;
    Command(command).execute(0);
    ((GameImpl*)Broodwar)->addUnitCommand(c);
    lastOrderFrame = Broodwar->getFrameCount();
    return true;
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool UnitImpl::attackMove(Position target)
  {
    return issueCommand(UnitCommand::attackMove(this, target));
  }
  //--------------------------------------------- ATTACK UNIT ------------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    return issueCommand(UnitCommand::attackUnit(this, target));
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool UnitImpl::build(TilePosition target, UnitType type)
  {
    return issueCommand(UnitCommand::build(this, target, type));
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type)
  {
    return issueCommand(UnitCommand::buildAddon(this,type));
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type)
  {
    return issueCommand(UnitCommand::train(this,type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    return issueCommand(UnitCommand::morph(this,type));
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    return issueCommand(UnitCommand::research(this,tech));
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    return issueCommand(UnitCommand::upgrade(this,upgrade));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPoint(Position target)
  {
    return issueCommand(UnitCommand::setRallyPosition(this,target));
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyPoint(Unit* target)
  {
    return issueCommand(UnitCommand::setRallyUnit(this,target));
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target)
  {
    return issueCommand(UnitCommand::move(this,target));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target)
  {
    return issueCommand(UnitCommand::patrol(this,target));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition()
  {
    return issueCommand(UnitCommand::holdPosition(this));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop()
  {
    return issueCommand(UnitCommand::stop(this));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    return issueCommand(UnitCommand::follow(this,target));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target)
  {
    return issueCommand(UnitCommand::gather(this,target));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo()
  {
    return issueCommand(UnitCommand::returnCargo(this));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    return issueCommand(UnitCommand::repair(this,target));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    return issueCommand(UnitCommand::burrow(this));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    return issueCommand(UnitCommand::unburrow(this));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    return issueCommand(UnitCommand::cloak(this));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    return issueCommand(UnitCommand::decloak(this));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    return issueCommand(UnitCommand::siege(this));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    return issueCommand(UnitCommand::unsiege(this));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    return issueCommand(UnitCommand::lift(this));
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    return issueCommand(UnitCommand::land(this,target));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    return issueCommand(UnitCommand::load(this,target));
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    return issueCommand(UnitCommand::unload(this,target));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll()
  {
    return issueCommand(UnitCommand::unloadAll(this));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target)
  {
    return issueCommand(UnitCommand::unloadAll(this,target));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target)
  {
    return issueCommand(UnitCommand::rightClick(this,target));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    return issueCommand(UnitCommand::rightClick(this,target));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    return issueCommand(UnitCommand::haltConstruction(this));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    return issueCommand(UnitCommand::cancelConstruction(this));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    return issueCommand(UnitCommand::cancelAddon(this));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    return issueCommand(UnitCommand::cancelTrain(this, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    return issueCommand(UnitCommand::cancelMorph(this));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    return issueCommand(UnitCommand::cancelResearch(this));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    return issueCommand(UnitCommand::cancelUpgrade(this));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    return issueCommand(UnitCommand::useTech(this,tech));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position target)
  {
    return issueCommand(UnitCommand::useTech(this,tech,target));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    return issueCommand(UnitCommand::useTech(this,tech,target));
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
  //--------------------------------------------- IN WPN RANGE -----------------------------------------------
  bool UnitImpl::isInWeaponRange(Unit *target) const
  {
    if ( !target || this == target )
      return false;

    UnitType thisType = this->getType();
    UnitType targType = target->getType();

    WeaponType wpn = targType.isFlyer() ? thisType.airWeapon() : thisType.groundWeapon();
    if ( wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown )
      return false;

    int ux = this->getPosition().x();
    int uy = this->getPosition().y();
    int tx = target->getPosition().x();
    int ty = target->getPosition().y();
    
    int uLeft       = ux - thisType.dimensionLeft();
    int uTop        = uy - thisType.dimensionUp();
    int uRight      = ux + thisType.dimensionRight()  + 1;
    int uBottom     = uy + thisType.dimensionDown() + 1;

    int targLeft    = tx - targType.dimensionLeft();
    int targTop     = ty - targType.dimensionUp();
    int targRight   = tx + targType.dimensionRight()  + 1;
    int targBottom  = ty + targType.dimensionDown() + 1;
    
    int xDist = uLeft - targRight;
    if ( xDist < 0 )
    {
      xDist = targLeft - uRight;
      if ( xDist < 0 )
        xDist = 0;
    }
    int yDist = uTop - targBottom;
    if ( yDist < 0 )
    {
      yDist = targTop - uBottom;
      if ( yDist < 0 )
        yDist = 0;
    }

    int distance = Position(0, 0).getApproxDistance(Position(xDist, yDist));
    return wpn.minRange() < distance && wpn.maxRange() >= distance;
  }
}

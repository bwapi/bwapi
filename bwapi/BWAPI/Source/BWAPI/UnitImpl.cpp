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
#include "CommandTrain.h"
#include "CommandBuild.h"
#include "CommandAttackMove.h"
#include "CommandAttackUnit.h"
#include "CommandPatrol.h"
#include "CommandHoldPosition.h"
#include "CommandStop.h"
#include "CommandRightClick.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"
#include "CommandRepair.h"
#include "CommandMorphUnit.h"
#include "CommandMorphBuilding.h"
#include "CommandBurrow.h"
#include "CommandUnburrow.h"
#include "CommandCloak.h"
#include "CommandDecloak.h"
#include "CommandSiege.h"
#include "CommandUnsiege.h"
#include "CommandLift.h"
#include "CommandLand.h"
#include "CommandLoad.h"
#include "CommandUnload.h"
#include "CommandUnloadAll.h"
#include "CommandCancelResearch.h"
#include "CommandCancelUpgrade.h"
#include "CommandCancelConstruction.h"
#include "CommandCancelTrain.h"
#include "CommandCancelMorph.h"
#include "CommandCancelAddon.h"
#include "CommandFollow.h"
#include "CommandSetRally.h"
#include "CommandReturnCargo.h"
#include "CommandUseTech.h"

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>

namespace BWAPI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  UnitImpl::UnitImpl(BW::Unit* originalUnit,
                     BW::Unit* unitLocal,
                     u16 index)
      : getOriginalRawData(originalUnit)
      , getRawDataLocal(unitLocal)
      , index(index)
      , userSelected(false)
      , _exists(false) //_exists is true while the unit exists
      , dead(false) //dead only set to true once the unit has died
      , savedPlayer(NULL)
      , savedUnitType(UnitTypes::None)
      , staticInformation(false)
      , lastVisible(false)
      , makeVisible(false)
      , lastType(UnitTypes::Unknown)
      , lastPlayer(NULL)
      , nukeDetected(false)
      , animState(0)
      , lastGroundWeaponCooldown(0)
      , lastAirWeaponCooldown(0)
      , startingAttack(false)
  {
    self = &data;
    id = (int)this;
  }
  //--------------------------------------------- DESTRUCTOR -------------------------------------------------
  UnitImpl::~UnitImpl()
  {
  }
  //--------------------------------------------- GET ID -----------------------------------------------------
  int UnitImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET PLAYER -------------------------------------------------
  Player* UnitImpl::getPlayer() const
  {
    return BroodwarImpl.server.getPlayer(self->player);
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
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool UnitImpl::exists() const
  {
    return self->exists;
  }
  //--------------------------------------------- IS ACCELERATING --------------------------------------------
  bool UnitImpl::isAccelerating() const
  {
    checkAccessBool();
    return getRawDataLocal->movementFlags.getBit(BW::MovementFlags::Accelerating);
  }
  //-------------------------------------------- IS BEING CONSTRUCTED ----------------------------------------
  bool UnitImpl::isBeingConstructed() const
  {
    checkAccessBool();
    if (self->isMorphing)
      return true;//all morphing units/buildings are being constructed
    if (self->isCompleted)
      return false; //no completed non-morphing units are being constructed
    if (_getType.getRace()!=Races::Terran)
      return true;//all incomplete non-terran units are being constructed
    return this->self->buildUnit != -1;//incomplete terran units are being constructed only if they have a unit building them
  }
  //------------------------------------------- IS BEING GATHERED --------------------------------------------
  bool UnitImpl::isBeingGathered() const
  {
    checkAccessBool();
    if (!getType().isResourceContainer())
      return false;

    return getRawDataLocal->unitUnion1.unitUnion1Sub.resourceUnitUnionSub.isBeingGathered != 0;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const
  {
    checkAccessBool();
    return getRawDataLocal->isBeingHealed != 0;
  }
  //------------------------------------------------ IS BLIND ------------------------------------------------
  bool UnitImpl::isBlind() const
  {
    checkAccessBool();
    return getRawDataLocal->isBlind != 0;
  }
  //----------------------------------------------- IS BRAKING -----------------------------------------------
  bool UnitImpl::isBraking() const
  {
    checkAccessBool();
    return getRawDataLocal->movementFlags.getBit(BW::MovementFlags::Braking);
  }
  //----------------------------------------------- IS BURROWED ----------------------------------------------
  bool UnitImpl::isBurrowed() const
  {
    checkAccessBool();
    return getRawDataLocal->status.getBit(BW::StatusFlags::Burrowed);
  }
  //--------------------------------------------- IS CARRYING GAS --------------------------------------------
  bool UnitImpl::isCarryingGas() const
  {
    checkAccessBool();
    if (!this->getType().isWorker())
      return false;
    return getRawDataLocal->resourceType == 1;
  }
  //------------------------------------------- IS CARRYING MINERALS -----------------------------------------
  bool UnitImpl::isCarryingMinerals() const
  {
    checkAccessBool();
    if (!this->getType().isWorker())
      return false;
    return getRawDataLocal->resourceType == 2;
  }
  //------------------------------------------------ IS CLOAKED ----------------------------------------------
  bool UnitImpl::isCloaked() const
  {
    checkAccessBool();
    return getRawDataLocal->status.getBit(BW::StatusFlags::Cloaked);
  }
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    return self->isCompleted;
  }
  //--------------------------------------------- IS CONSTRUCTING --------------------------------------------
  bool UnitImpl::isConstructing() const
  {
    return self->isConstructing;
  }
  //------------------------------------------- IS DEFENSE MATRIXED ------------------------------------------
  bool UnitImpl::isDefenseMatrixed() const
  {
    checkAccessBool();
    return getRawDataLocal->defenseMatrixTimer > 0;
  }
  //----------------------------------------------- IS ENSNARED ----------------------------------------------
  bool UnitImpl::isEnsnared() const
  {
    checkAccessBool();
    return getRawDataLocal->ensnareTimer > 0;
  }
  //---------------------------------------------- IS FOLLOWING ----------------------------------------------
  bool UnitImpl::isFollowing() const
  {
    checkAccessBool();
    return self->order == BW::OrderID::Follow;
  }
  //-------------------------------------------- IS GATHERING GAS --------------------------------------------
  bool UnitImpl::isGatheringGas() const
  {
    checkAccessBool();
    if (!_getType.isWorker())
      return false;

    if (!getRawDataLocal->status.getBit(BW::StatusFlags::IsGathering))
      return false;

    int tOrderID = self->order;
    if (tOrderID != BW::OrderID::MoveToGas  &&
        tOrderID != BW::OrderID::WaitForGas &&
        tOrderID != BW::OrderID::HarvestGas &&
        tOrderID != BW::OrderID::ReturnGas  &&
        tOrderID != BW::OrderID::ResetCollision2)
      return false;

    if (tOrderID == BW::OrderID::ResetCollision2)
      return this->isCarryingGas();

    //return true if BWOrder is WaitForGas, HarvestGas, or ReturnGas
    if (tOrderID != BW::OrderID::MoveToGas)
      return true;

    //if BWOrder is MoveToGas, we need to do some additional checks to make sure the unit is really gathering
    if (getTarget() != NULL)
    {
      if (getTarget()->getType() == UnitTypes::Resource_Vespene_Geyser)
        return false;
      if (((BWAPI::UnitImpl*)getTarget())->_getPlayer != _getPlayer)
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
      if (((BWAPI::UnitImpl*)getOrderTarget())->_getPlayer != _getPlayer)
        return false;
      if (!this->getOrderTarget()->isCompleted() && !getOrderTarget()->getType().isResourceDepot())
        return false;
      if (this->getOrderTarget()->getType().isRefinery() || getOrderTarget()->getType().isResourceDepot())
        return true;
    }
    return false;
  }
  //----------------------------------------- IS GATHERING MINERALS ------------------------------------------
  bool UnitImpl::isGatheringMinerals() const
  {
    checkAccessBool();
    if (!_getType.isWorker())
      return false;

    if (!getRawDataLocal->status.getBit(BW::StatusFlags::IsGathering))
      return false;

    int tOrderID = self->order;
    if (tOrderID != BW::OrderID::MoveToMinerals &&
        tOrderID != BW::OrderID::WaitForMinerals &&
        tOrderID != BW::OrderID::MiningMinerals &&
        tOrderID != BW::OrderID::ReturnMinerals &&
        tOrderID != BW::OrderID::ResetCollision2)
      return false;

    if (tOrderID == BW::OrderID::ResetCollision2)
      return isCarryingMinerals();
    return true;
  }
  //-------------------------------------------- IS HALLUCINATION --------------------------------------------
  bool UnitImpl::isHallucination() const
  {
    if (!attemptAccessInside())
      return false;
    return getRawDataLocal->status.getBit(BW::StatusFlags::IsHallucination);
  }
  //---------------------------------------------- IS IDLE ---------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    return self->isIdle;
  }
  //---------------------------------------------- IS IRRADIATED ---------------------------------------------
  bool UnitImpl::isIrradiated() const
  {
    checkAccessBool();
    return getRawDataLocal->irradiateTimer > 0;
  }
  //------------------------------------------------ IS LIFTED -----------------------------------------------
  bool UnitImpl::isLifted() const
  {
    checkAccessBool();
    return getRawDataLocal->status.getBit(BW::StatusFlags::InAir) &&
           getRawDataLocal->unitID.isBuilding();
  }
  //------------------------------------------------ IS LOADED -----------------------------------------------
  bool UnitImpl::isLoaded() const
  {
    if (!attemptAccessInside())
      return false;

    return _getTransport!=NULL;
  }
  //---------------------------------------------- IS LOCKED DOWN --------------------------------------------
  bool UnitImpl::isLockedDown() const
  {
    checkAccessBool();
    return getLockdownTimer() > 0;
  }
  //---------------------------------------------- IS MAELSTROMED --------------------------------------------
  bool UnitImpl::isMaelstrommed() const
  {
    checkAccessBool();
    return getRawDataLocal->maelstromTimer > 0;
  }
  //----------------------------------------------- IS MORPHING ----------------------------------------------
  bool UnitImpl::isMorphing() const
  {
    return self->isMorphing;
  }
  //------------------------------------------------ IS MOVING -----------------------------------------------
  bool UnitImpl::isMoving() const
  {
    checkAccessBool();
    return getRawDataLocal->movementFlags.getBit(BW::MovementFlags::Moving);
  }
  //----------------------------------------------- IS PARASITED ---------------------------------------------
  bool UnitImpl::isParasited() const
  {
    checkAccessBool();
    return getRawDataLocal->parasiteFlags.value != 0;
  }
  //---------------------------------------------- IS PATROLLING ---------------------------------------------
  bool UnitImpl::isPatrolling() const
  {
    checkAccessBool();
    return self->order == BW::OrderID::Patrol;
  }
  //----------------------------------------------- IS PLAGUED -----------------------------------------------
  bool UnitImpl::isPlagued() const
  {
    checkAccessBool();
    return getRawDataLocal->plagueTimer > 0;
  }
  //----------------------------------------------- IS REPAIRING ---------------------------------------------
  bool UnitImpl::isRepairing() const
  {
    checkAccessBool();
    return self->order == BW::OrderID::Repair1 || self->order == BW::OrderID::Repair2;
  }
  //---------------------------------------------- IS RESEARCHING --------------------------------------------
  bool UnitImpl::isResearching() const
  {
    return self->order == BW::OrderID::ResearchTech;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSelected() const
  {
    checkAccessBool();
    if (BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return false;
    return this->userSelected;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSieged() const
  {
    checkAccessBool();
    return this->_getType == UnitTypes::Terran_Siege_Tank_Siege_Mode;
  }
  //------------------------------------------- IS STARTING ATTACK -------------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    checkAccessBool();
    return startingAttack;
  }
  //-------------------------------------- IS STARTING ATTACK SEQUENCE ---------------------------------------
  bool UnitImpl::isStartingAttackSequence() const
  {
    checkAccessBool();
    return this->animState == BW::Image::Anims::GndAttkInit || this->animState == BW::Image::Anims::AirAttkInit;
  }
  //----------------------------------------------- IS ATTACKING ---------------------------------------------
  bool UnitImpl::isAttacking() const
  {
    checkAccessBool();
    return (this->animState == BW::Image::Anims::GndAttkRpt  ||
            this->animState == BW::Image::Anims::AirAttkRpt  || 
            this->animState == BW::Image::Anims::GndAttkInit ||
            this->animState == BW::Image::Anims::AirAttkInit);
  }
  //----------------------------------------------- IS STASISED ----------------------------------------------
  bool UnitImpl::isStasised() const
  {
    checkAccessBool();
    return getStasisTimer() > 0;
  }
  //----------------------------------------------- IS STIMMED -----------------------------------------------
  bool UnitImpl::isStimmed() const
  {
    checkAccessBool();
    return getStimTimer() > 0;
  }
  //---------------------------------------------- IS TRAINING -----------------------------------------------
  bool UnitImpl::isTraining() const
  {
    return self->isTraining;
  }
  //---------------------------------------------- IS UNDER STORM --------------------------------------------
  bool UnitImpl::isUnderStorm() const
  {
    checkAccessBool();
    return getRawDataLocal->isUnderStorm != 0;
  }
  //------------------------------------------------ IS UNPOWERED --------------------------------------------
  bool UnitImpl::isUnpowered() const
  {
    checkAccessBool();
    if (_getType.getRace() == Races::Protoss && _getType.isBuilding())
      return getRawDataLocal->status.getBit(BW::StatusFlags::DoodadStatesThing);
    return false;
  }
  //----------------------------------------------- IS UPGRADING ---------------------------------------------
  bool UnitImpl::isUpgrading() const
  {
    return self->order == BW::OrderID::Upgrade;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool UnitImpl::isVisible() const
  {
    return self->isVisible[BroodwarImpl.server.getPlayerID(Broodwar->self())];
  }
  bool UnitImpl::isVisible(Player* player) const
  {
    if (player==NULL) return false;
    return self->isVisible[BroodwarImpl.server.getPlayerID(player)];
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    if (!this->_exists)
      return;
    this->userSelected = selectedState;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  double UnitImpl::getDistance(Unit* target) const
  {
    checkAccessDouble();
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
    checkAccessDouble();
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
  //----------------------------------------------- GET TARGET -----------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    checkAccessPointer();
    return UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->targetUnit);
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  Unit* UnitImpl::getOrderTarget() const
  {
    checkAccessPointer();
    return this->_getOrderTarget();
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  Unit* UnitImpl::_getOrderTarget() const
  {
    if (!_exists)
      return NULL;
    return UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->orderTargetUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getBuildUnit() const
  {
    return BroodwarImpl.server.getUnit(self->buildUnit);
  }
  //--------------------------------------------- GET BUILD TYPE ---------------------------------------------
  UnitType UnitImpl::getBuildType() const
  {
    return UnitType(self->buildType);
  }
  //----------------------------------------------- GET CHILD ------------------------------------------------
  Unit* UnitImpl::getChild() const
  {
    checkAccessPointer();
    return UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->childInfoUnion.childUnit1);
  }
  //------------------------------------------ GET TARGET POSITION -------------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    checkAccessPosition();
    return BWAPI::Position(getRawDataLocal->moveToPos.x, getRawDataLocal->moveToPos.y);
  }
  //------------------------------------------- GET UPGRADE LEVEL --------------------------------------------
  int UnitImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    checkAccessInt();
    if (_getPlayer->getUpgradeLevel(upgrade) == 0)
      return 0;

    if (upgrade.whatUses().find(_getType) != upgrade.whatUses().end())
      return this->_getPlayer->getUpgradeLevel(upgrade);
    return 0;
  }
  //------------------------------------------------ HAS NUKE ------------------------------------------------
  bool UnitImpl::hasNuke() const
  {
    if (!attemptAccessInside())
      return false;
    if (getType()!=UnitTypes::Terran_Nuclear_Silo) //not sure if this check is needed, but just to be safe
      return false;
    return getRawDataLocal->hasNuke!=0;
  }
  //------------------------------------------------ GET ORDER -----------------------------------------------
  Order UnitImpl::getOrder() const
  {
    return Order(self->order);
  }
  //----------------------------------------- GET SECONDARY ORDER ID -----------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return Order(self->secondaryOrder);
  }
  //------------------------------------------ GET TRAINING QUEUE --------------------------------------------
  std::list<UnitType > UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType > trainingQueue;
    for (int i=0;i<self->trainingQueueCount;i++)
    {
      trainingQueue.push_back(self->trainingQueue[i]);
    }
    return trainingQueue;
  }
  //-------------------------------------------- GET TRANSPORT -----------------------------------------------
  Unit* UnitImpl::getTransport() const
  {
    if (!this->attemptAccessInside())
      return NULL;
    return _getTransport;
  }
  //------------------------------------------- GET LOADED UNITS ---------------------------------------------
  std::set<Unit*> UnitImpl::getLoadedUnits() const
  {
    std::set<Unit*> nothing;
    if (!attemptAccessInside())
      return nothing;
    return loadedUnits;
  }
  //--------------------------------------------- GET CARRIER ------------------------------------------------
  Unit* UnitImpl::getCarrier() const
  {
    checkAccessPointer();
    if (getType() != UnitTypes::Protoss_Interceptor)
      return NULL;
    return (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->childInfoUnion.parentUnit));
  }
  //------------------------------------------- GET INTERCEPTORS ---------------------------------------------
  std::set<Unit*> UnitImpl::getInterceptors() const
  {
    std::set<Unit*> nothing;
    if (!attemptAccessInside())
      return nothing;
    if (getType() != UnitTypes::Protoss_Carrier)
      return nothing;
    return this->connectedUnits;
  }
  //---------------------------------------------- GET HATCHERY ----------------------------------------------
  Unit* UnitImpl::getHatchery() const
  {
    checkAccessPointer();
    if (getType()==UnitTypes::Zerg_Larva)
      return (Unit*)UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->connectedUnit);
    return NULL;
  }
  //----------------------------------------------- GET LARVA ------------------------------------------------
  std::set<Unit*> UnitImpl::getLarva() const
  {
    std::set<Unit*> nothing;
    if (!this->attemptAccess())
      return nothing;
    if (this->getType() != UnitTypes::Zerg_Hatchery &&
        this->getType() != UnitTypes::Zerg_Lair &&
        this->getType() != UnitTypes::Zerg_Hive)
      return nothing;
    return this->connectedUnits;
  }
  //----------------------------------------------- GET TECH -------------------------------------------------
  TechType UnitImpl::getTech() const
  {
    return TechType(self->tech);
  }
  //---------------------------------------------- GET UPGRADE -----------------------------------------------
  UpgradeType UnitImpl::getUpgrade() const
  {
    return UpgradeType(self->upgrade);
  }
  //-------------------------------------- GET REMAINING RESEARCH TIME ---------------------------------------
  int UnitImpl::getRemainingResearchTime() const
  {
    return self->remainingResearchTime;
  }
  //-------------------------------------- GET REMAINING UPGRADE TIME ----------------------------------------
  int UnitImpl::getRemainingUpgradeTime() const
  {
    return self->remainingUpgradeTime;
  }
  //------------------------------------------ GET RALLY POSITION --------------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    if (!this->attemptAccessInside())
      return BWAPI::Positions::None;

    if (this->_getType.canProduce())
      return Position(getRawDataLocal->rallyPsiProviderUnion.rally.rallyX,
                      getRawDataLocal->rallyPsiProviderUnion.rally.rallyY);
    return Positions::None;
  }
  //-------------------------------------------- GET RALLY UNIT ----------------------------------------------
  Unit* UnitImpl::getRallyUnit() const
  {
    if (!this->attemptAccessInside())
      return NULL;

    if (this->_getType.canProduce())
      return (Unit*)UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->rallyPsiProviderUnion.rally.rallyUnit);
    return NULL;
  }
  //----------------------------------------------- GET ADDON ------------------------------------------------
  Unit* UnitImpl::getAddon() const
  {
    checkAccessPointer();
    if (this->getType().isBuilding())
    {
      Unit* addon = UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->currentBuildUnit);
      if (addon != NULL && addon->getType().isAddon())
        return addon;
      addon = (Unit*)UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->childInfoUnion.childUnit1);
      if (addon!=NULL && addon->exists() && addon->getType().isAddon())
        return addon;
    }
    return NULL;
  }
  //--------------------------------------------- GET NYDUS EXIT ---------------------------------------------
  Unit* UnitImpl::getNydusExit() const
  {
    checkAccessPointer();
    if (getType()!=UnitTypes::Zerg_Nydus_Canal)
      return NULL;
    Unit* nydus = UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->currentBuildUnit);
    if (nydus != NULL && nydus->getType()==UnitTypes::Zerg_Nydus_Canal)
      return nydus;
    nydus = (Unit*)UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->childInfoUnion.childUnit1);
    if (nydus != NULL && nydus->getType()==UnitTypes::Zerg_Nydus_Canal)
      return nydus;
    return NULL;
  }
  //-------------------------------------------- ORDER Issue Command -----------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    //call the appropriate command function based on the command type
    switch(command.type.getID())
    {
      case 0:
        return attackMove(Position(command.x,command.y));
      case 1:
        return attackUnit(command.target);
      case 2:
        return rightClick(Position(command.x,command.y));
      case 3:
        return rightClick(command.target);
      case 4:
        return train(UnitType(command.extra));
      case 5:
        return build(TilePosition(command.x,command.y),UnitType(command.extra));
      case 6:
        return buildAddon(UnitType(command.extra));
      case 7:
        return research(TechType(command.extra));
      case 8:
        return upgrade(UpgradeType(command.extra));
      case 9:
        return stop();
      case 10:
        return holdPosition();
      case 11:
        return patrol(Position(command.x,command.y));
      case 12:
        return follow(command.target);
      case 13:
        return setRallyPosition(Position(command.x,command.y));
      case 14:
        return setRallyUnit(command.target);
      case 15:
        return repair(command.target);
      case 16:
        return returnCargo();
      case 17:
        return morph(UnitType(command.extra));
      case 18:
        return burrow();
      case 19:
        return unburrow();
      case 20:
        return siege();
      case 21:
        return unsiege();
      case 22:
        return cloak();
      case 23:
        return decloak();
      case 24:
        return lift();
      case 25:
        return land(TilePosition(command.x,command.y));
      case 26:
        return load(command.target);
      case 27:
        return unload(command.target);
      case 28:
        return unloadAll();
      case 29:
        return unloadAll(Position(command.x,command.y));
      case 30:
        return cancelConstruction();
      case 31:
        return haltConstruction();
      case 32:
        return cancelMorph();
      case 33:
        return cancelTrain();
      case 34:
        return cancelTrain(command.extra);
      case 35:
        return cancelAddon();
      case 36:
        return cancelResearch();
      case 37:
        return cancelUpgrade();
      case 38:
        return useTech(TechType(command.extra));
      case 39:
        return useTech(TechType(command.extra),Position(command.x,command.y));
      case 40:
        return useTech(TechType(command.extra),command.target);
      default:
        break;
    }
    BroodwarImpl.setLastError(Errors::Unknown);
    return false;
  }
  //------------------------------------------- ORDER Attack Location ----------------------------------------
  bool UnitImpl::attackMove(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::AttackMove), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandAttackMove(this, BW::Position((u16)position.x(), (u16)position.y())));
    return true;
  }
  //--------------------------------------------- ORDER Attack Unit ------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (target == NULL)
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }

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
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::AttackUnit), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandAttackUnit(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick(BW::Position((u16)position.x(), (u16)position.y())), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, BW::Position((u16)position.x(), (u16)position.y())));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (target == NULL)
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
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
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)target), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, (UnitImpl*)target));
    return true;
  }
  //----------------------------------------------- TRAIN UNIT -----------------------------------------------
  bool UnitImpl::train(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
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
    int tUnitType = this->_getType.getID();
    BroodwarImpl.addToCommandBuffer(new CommandTrain(this, type));
    if (tUnitType == BW::UnitID::Zerg_Larva ||
        tUnitType == BW::UnitID::Zerg_Mutalisk ||
        tUnitType == BW::UnitID::Zerg_Hydralisk)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnitMorph(type), 0x3);
    }
    else if (tUnitType == BW::UnitID::Zerg_Hatchery ||
             tUnitType == BW::UnitID::Zerg_Lair ||
             tUnitType == BW::UnitID::Zerg_Spire ||
             tUnitType == BW::UnitID::Zerg_CreepColony)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::BuildingMorph(type), 0x3);
    }
    else if (tUnitType == BW::UnitID::Protoss_Carrier ||
             tUnitType == BW::UnitID::Protoss_Hero_Gantrithor ||
             tUnitType == BW::UnitID::Protoss_Reaver ||
             tUnitType == BW::UnitID::Protoss_Hero_Warbringer)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::TrainFighter(), 0x1);
    }
    else
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type), 0x3);
    }
    return true;
  }
  //------------------------------------------------- BUILD --------------------------------------------------
  bool UnitImpl::build(TilePosition position, UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
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
    if (!type1.isAddon() && !Broodwar->canBuildHere(this,position,type1))
      return false;

    BW::UnitType type((u16)type1.getID());
    this->orderSelect();
    if (!type.isAddon())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition((u16)position.x(), (u16)position.y()), type), sizeof(BW::Orders::MakeBuilding));
    else
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)position.x(), (u16)position.y()), type), sizeof(BW::Orders::MakeAddon));
    BroodwarImpl.addToCommandBuffer(new CommandBuild(this, type, BW::TilePosition((u16)position.x(), (u16)position.y())));
    return true;
  }
  //----------------------------------------------- BUILD ADDON ----------------------------------------------
  bool UnitImpl::buildAddon(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (!type1.isAddon())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    return this->build(TilePosition(this->getTilePosition().x()+4,this->getTilePosition().y()+1),type1);    
  }
  //------------------------------------------------ RESEARCH ------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (!Broodwar->canResearch(this,tech))
      return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    u8 techenum = (u8)tech.getID();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Invent(BW::TechType(techenum)), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new CommandInvent(this, BW::TechType(techenum)));
    return true;
  }
  //------------------------------------------------- UPGRADE ------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (!Broodwar->canUpgrade(this,upgrade))
      return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    u8 upgradeenum = (u8)upgrade.getID();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType(upgradeenum)), sizeof(BW::Orders::Upgrade));
    BroodwarImpl.addToCommandBuffer(new CommandUpgrade(this, BW::UpgradeType(upgradeenum)));
    return true;
  }
  //-------------------------------------------------- STOP --------------------------------------------------
  bool UnitImpl::stop()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    int tUnitType = _getType.getID();
    if (tUnitType == BW::UnitID::Protoss_Reaver ||
        tUnitType == BW::UnitID::Protoss_Hero_Warbringer)
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::ReaverStop(), sizeof(BW::Orders::Stop));
    else if (tUnitType == BW::UnitID::Protoss_Carrier ||
             tUnitType == BW::UnitID::Protoss_Hero_Gantrithor)
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CarrierStop(), sizeof(BW::Orders::Stop));
    else
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    BroodwarImpl.addToCommandBuffer(new CommandStop(this));
    return true;
  }
  //---------------------------------------------- HOLD POSITION ---------------------------------------------
  bool UnitImpl::holdPosition()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
    switch (this->_getType.getID())
    {
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        getRawDataLocal->orderID = BW::OrderID::CarrierHoldPosition;
        break;
      case BW::UnitID::Zerg_Queen:
      case BW::UnitID::Zerg_Hero_Matriarch:
        getRawDataLocal->orderID = BW::OrderID::QueenHoldPosition;
        break;
      case BW::UnitID::Zerg_InfestedTerran:
      case BW::UnitID::Zerg_Scourge:
        getRawDataLocal->orderID = BW::OrderID::SuicideHoldPosition;
        break;
      case BW::UnitID::Terran_Medic:
        getRawDataLocal->orderID = BW::OrderID::MedicHoldPosition;
        break;
      case BW::UnitID::Protoss_Reaver:
        getRawDataLocal->orderID = BW::OrderID::ReaverHoldPosition;
        break;
      default:
        getRawDataLocal->orderID = BW::OrderID::HoldPosition;
    }
    BroodwarImpl.addToCommandBuffer(new CommandHoldPosition(this));
    return true;
  }
  //-------------------------------------------------- PATROL ------------------------------------------------
  bool UnitImpl::patrol(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandPatrol(this, BW::Position((u16)position.x(), (u16)position.y())));
    return true;
  }
  //-------------------------------------------------- FOLLOW ------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (target == NULL)
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandFollow(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------------- SET RALLY ----------------------------------------------
  bool UnitImpl::setRallyPosition(Position target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::RallyPointTile), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandSetRally(this, BW::Position((u16)target.x(), (u16)target.y())));
    return true;
  }
  //------------------------------------------------- SET RALLY ----------------------------------------------
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (target == NULL)
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPointUnit), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandSetRally(this, (UnitImpl*)target));
    return true;
  }
  //-------------------------------------------------- REPAIR ------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (target == NULL)
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_SCV || target->getType().isOrganic())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandRepair(this, (UnitImpl*)target));
    return true;
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
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
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::ReturnCargo(0), sizeof(BW::Orders::ReturnCargo));
    BroodwarImpl.addToCommandBuffer(new CommandReturnCargo(this));
    return true;
  }
  //-------------------------------------------------- MORPH -------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
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
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::BuildingMorph(rawtype), sizeof(BW::Orders::BuildingMorph));
      BroodwarImpl.addToCommandBuffer(new CommandMorphBuilding(this, rawtype));
    }
    else
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnitMorph(rawtype), sizeof(BW::Orders::UnitMorph));
      BroodwarImpl.addToCommandBuffer(new CommandMorphUnit(this, rawtype));
    }
    return true;
  }
  //-------------------------------------------------- BURROW ------------------------------------------------
  bool UnitImpl::burrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType()!=UnitTypes::Zerg_Lurker && !Broodwar->self()->hasResearched(TechTypes::Burrowing))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(!this->isBurrowed())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
      BroodwarImpl.addToCommandBuffer(new CommandBurrow(this));
    }
    return true;
  }
  //------------------------------------------------- UNBURROW -----------------------------------------------
  bool UnitImpl::unburrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType()!=UnitTypes::Zerg_Lurker && !Broodwar->self()->hasResearched(TechTypes::Burrowing))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(this->isBurrowed())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
      BroodwarImpl.addToCommandBuffer(new CommandUnburrow(this));
    }
    return true;
  }
  //-------------------------------------------------- SIEGE -------------------------------------------------
  bool UnitImpl::siege()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_Siege_Tank_Tank_Mode &&
        this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!Broodwar->self()->hasResearched(TechTypes::Tank_Siege_Mode))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if (!this->isSieged())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
      BroodwarImpl.addToCommandBuffer(new CommandSiege(this));
    }
    return true;
  }
  //------------------------------------------------- UNSIEGE ------------------------------------------------
  bool UnitImpl::unsiege()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_Siege_Tank_Tank_Mode &&
        this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!Broodwar->self()->hasResearched(TechTypes::Tank_Siege_Mode))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if (this->isSieged())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
      BroodwarImpl.addToCommandBuffer(new CommandUnsiege(this));
    }
    return true;
  }
  //-------------------------------------------------- CLOAK -------------------------------------------------
  bool UnitImpl::cloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType()!=UnitTypes::Terran_Wraith && this->getType()!=UnitTypes::Terran_Ghost)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType()==UnitTypes::Terran_Wraith)
    {
      if (!Broodwar->self()->hasResearched(TechTypes::Cloaking_Field))
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Tech);
        return false;
      }
      if (this->getEnergy()<TechTypes::Cloaking_Field.energyUsed())
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Energy);
        return false;
      }
    }
    if (this->getType()==UnitTypes::Terran_Ghost)
    {
      if (!Broodwar->self()->hasResearched(TechTypes::Personnel_Cloaking))
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Tech);
        return false;
      }
      if (this->getEnergy()<TechTypes::Personnel_Cloaking.energyUsed())
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Energy);
        return false;
      }
    }
    if(!this->isCloaked())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
      BroodwarImpl.addToCommandBuffer(new CommandCloak(this));
    }
    return true;
  }
  //------------------------------------------------- DECLOAK ------------------------------------------------
  bool UnitImpl::decloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType()!=UnitTypes::Terran_Wraith && this->getType()!=UnitTypes::Terran_Ghost)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if ((this->getType()==UnitTypes::Terran_Wraith && !Broodwar->self()->hasResearched(TechTypes::Cloaking_Field))
      || (this->getType()==UnitTypes::Terran_Ghost && !Broodwar->self()->hasResearched(TechTypes::Personnel_Cloaking)))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(this->isCloaked())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
      BroodwarImpl.addToCommandBuffer(new CommandDecloak(this));
    }
    return true;
  }
  //--------------------------------------------------- LIFT -------------------------------------------------
  bool UnitImpl::lift()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(!this->isLifted())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
      BroodwarImpl.addToCommandBuffer(new CommandLift(this));
    }
    return true;
  }
  //--------------------------------------------------- LAND -------------------------------------------------
  bool UnitImpl::land(TilePosition position)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(this->isLifted())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Land(BW::TilePosition((u16)position.x(), (u16)position.y()), BW::UnitType((u16)this->_getType.getID())), sizeof(BW::Orders::Land));
      BroodwarImpl.addToCommandBuffer(new CommandLand(this, BW::TilePosition((u16)position.x(), (u16)position.y())));
    }
    return true;
  }
  //--------------------------------------------------- LOAD -------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (target == NULL)
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    bool loaded = false;
    if (this->getType() == UnitTypes::Terran_Bunker)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::PickupBunker), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::PickupTransport), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (target->getType() == UnitTypes::Terran_Bunker || target->getType() == UnitTypes::Terran_Dropship || target->getType() == UnitTypes::Protoss_Shuttle || target->getType() == UnitTypes::Zerg_Overlord)
    {
      this->rightClick(target);
      loaded = true;
    }
    if (loaded)
    {
      BroodwarImpl.addToCommandBuffer(new CommandLoad(this, (UnitImpl*)target));
      return true;
    }
    //if neither this unit nor the target unit is a bunker, dropship, shuttle, or overlord, return false.
    BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    return false;
  }
  //-------------------------------------------------- UNLOAD ------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (target == NULL)
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnloadUnit((UnitImpl*)target), sizeof(BW::Orders::UnloadUnit));
    BroodwarImpl.addToCommandBuffer(new CommandUnload(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      return this->unloadAll(this->getPosition());
    }
    if (this->getType() != UnitTypes::Terran_Bunker)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
    BroodwarImpl.addToCommandBuffer(new CommandUnloadAll(this));
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() == UnitTypes::Terran_Bunker)
      this->unloadAll();

    if (this->getType() != UnitTypes::Terran_Dropship && this->getType() != UnitTypes::Protoss_Shuttle && this->getType() != UnitTypes::Zerg_Overlord)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandUnloadAll(this, BW::Position((u16)position.x(), (u16)position.y())));
    return true;
  }
  //-------------------------------------------- CANCEL CONSTRUCTION -----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if (this->isCompleted())
      return false;

    if (!this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    BroodwarImpl.addToCommandBuffer(new CommandCancelConstruction(this));
    return true;
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getOrder() != Orders::ConstructingBuilding)
      return false;
    return this->stop();
  }
  //----------------------------------------------- CANCEL MORPH ---------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      return this->cancelConstruction();
    }
    if (this->isMorphing())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelUnitMorph(), sizeof(BW::Orders::CancelUnitMorph));
      BroodwarImpl.addToCommandBuffer(new CommandCancelMorph(this));
    }
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isTraining())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelTrainLast(), sizeof(BW::Orders::CancelTrainLast));
      BroodwarImpl.addToCommandBuffer(new CommandCancelTrain(this));
    }
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isTraining() && (int)(this->getTrainingQueue().size()) > slot)
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelTrain((u8)slot), sizeof(BW::Orders::CancelTrain));
      BroodwarImpl.addToCommandBuffer(new CommandCancelTrain(this, slot));
    }
    return true;
  }
  //----------------------------------------------- CANCEL ADDON ---------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelAddon(), sizeof(BW::Orders::CancelAddon));
    BroodwarImpl.addToCommandBuffer(new CommandCancelAddon(this));
    return true;
  }
  //---------------------------------------------- CANCEL RESEARCH -------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (self->order == BW::OrderID::ResearchTech)
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelResearch(), sizeof(BW::Orders::CancelResearch));
      BroodwarImpl.addToCommandBuffer(new CommandCancelResearch(this));
    }
    return true;
  }
  //---------------------------------------------- CANCEL UPGRADE --------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (self->order == BW::OrderID::Upgrade)
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelUpgrade(), sizeof(BW::Orders::CancelUpgrade));
      BroodwarImpl.addToCommandBuffer(new CommandCancelUpgrade(this));
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
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
    if (tech.whatUses().find(this->getType())==tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Stimpacks:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
        BroodwarImpl.addToCommandBuffer(new CommandUseTech(this,tech));
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
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!Broodwar->self()->hasResearched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy()<tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if (tech.whatUses().find(this->getType())==tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (tech == TechTypes::Spider_Mines && this->getSpiderMineCount()<=0)
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::DarkSwarm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::DarkSwarm), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::DisruptionWeb:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::CastDisruptionWeb), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::EMPShockwave:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::EmpShockwave), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Ensnare:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::Ensnare), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Maelstorm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::CastMaelstrom), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::NuclearStrike:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::NukePaint), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Plague:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::Plague), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::PsionicStorm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::PsiStorm), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Recall:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::Teleport), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::ScannerSweep:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::PlaceScanner), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::SpiderMines:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::PlaceMine), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::StasisField:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), BW::OrderID::StasisField), sizeof(BW::Orders::Attack));
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
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
    if (tech.whatUses().find(this->getType())==tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Consume:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Consume), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::DefensiveMatrix:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::DefensiveMatrix), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Feedback:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastFeedback), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Hallucination:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Hallucination1), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Healing:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::MedicHeal1), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Infestation:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::InfestMine2), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Irradiate:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Irradiate), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Lockdown:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::MagnaPulse), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::MindControl:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastMindControl), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::OpticalFlare:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastOpticalFlare), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Parasite:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastParasite), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Restoration:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Restoration), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::SpawnBroodlings:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::SummonBroodlings), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::YamatoGun:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::FireYamatoGun1), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::ArchonWarp:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::ShiftSelectSingle((UnitImpl*)target), sizeof(BW::Orders::ShiftSelectSingle));
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MergeArchon(), sizeof(BW::Orders::MergeArchon));
        break;
      case BW::TechID::DarkArchonMeld:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::ShiftSelectSingle((UnitImpl*)target), sizeof(BW::Orders::ShiftSelectSingle));
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MergeDarkArchon(), sizeof(BW::Orders::MergeDarkArchon));
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
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
    if (unit == NULL)
      return NULL;

    u16 index = (u16)( ((u32)unit - (u32)BW::BWDATA_UnitNodeTable) / 336) & 0x7FF;
    if (index > BW::UNIT_ARRAY_MAX_LENGTH)
    {
      if (BroodwarImpl.invalidIndices.find(index) == BroodwarImpl.invalidIndices.end())
      {
        BroodwarImpl.newUnitLog->log("Error: Found new invalid unit index: %d, broodwar address: 0x%x",index,unit);
        BroodwarImpl.invalidIndices.insert(index);
      }
      return NULL;
    }
    return BroodwarImpl.getUnit(index);
  }

  void UnitImpl::die()
  {
    //if the unit already does exist, don't kill it again
    if (!this->_exists)
      return;

    //save player and unit type
    this->savedPlayer    = this->_getPlayer;
    this->savedUnitType  = this->_getType;

    //set pointers to null so we don't read information from unit table anymore
    this->getRawDataLocal    = NULL;
    this->getOriginalRawData = NULL;
    this->index              = 0xFFFF;
    this->userSelected       = false;
    this->_exists            = false;
    this->dead               = true;
    this->lastType           = UnitTypes::Unknown;
    this->lastPlayer         = NULL;
    this->nukeDetected       = false;
  }

  /* canAccess returns true if the AI module is allowed to access the unit.
    If the unit is visible, returns true.
    If the unit is does not exist, returns false.
    If the unit is not visible and exists, returns true only if CompleteMapInformation is enabled.
  */
  bool UnitImpl::canAccess() const
  {
    if (!this->_exists)  return false;
    if (this->isVisible()) return true;

    //if we get here, the unit exists but is not visible

    if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return true;

    /* neutral units visible during AIModule::onStart */
    if (Broodwar->getFrameCount()==0)
      if (this->_getType.isNeutral())
        return true;

    if (BroodwarImpl.inUpdate == true)
      return true;

    return false;
  }

  //returns true if canAccess() is true or the unit is owned by self
  //always returns true for units owned by self, even dead units
  bool UnitImpl::canAccessSpecial() const
  {
    return (this->savedPlayer == BroodwarImpl.self() || canAccess());
  }

  //returns true if canAccess() is true and the unit is owned by self (or complete map info is turned on)
  bool UnitImpl::canAccessInside() const
  {
    if (!canAccess())
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
    if (!BroodwarImpl.inUpdate)
    {
      BroodwarImpl.setLastError(Errors::None);
      if (this->canAccess())
        return true;
      if (!this->_exists && this->savedPlayer == BroodwarImpl.self())
      {
        BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
        return false;
      }
      BroodwarImpl.setLastError(Errors::Unit_Not_Visible);
      return false;
    }
    else
    {
      return this->canAccess();
    }
  }

  // calls canAccessSpecial, setting error codes as needed
  bool UnitImpl::attemptAccessSpecial() const
  {
    if (!BroodwarImpl.inUpdate)
    {
      BroodwarImpl.setLastError(Errors::None);
      if (this->canAccessSpecial())
        return true;
      BroodwarImpl.setLastError(Errors::Unit_Not_Visible);
      return false;
    }
    else
    {
      return this->canAccessSpecial();
    }
  }

  // calls canAccessSpecial, setting error codes as needed
  bool UnitImpl::attemptAccessInside() const
  {
    if (!BroodwarImpl.inUpdate)
    {
      BroodwarImpl.setLastError(Errors::None);
      if (this->canAccessInside())
        return true;
      if (!this->_exists && this->savedPlayer == BroodwarImpl.self())
      {
        BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
        return false;
      }
      BroodwarImpl.setLastError(Errors::Unit_Not_Visible);
      return false;
    }
    else
    {
      return this->canAccessInside();
    }
  }
  //----------------------------------------------------------------------------------------------------------
  std::string UnitImpl::getName() const
  {
    return this->getType().getName();
  }
  //---------------------------------------------- UPDATE NEXT -----------------------------------------------
  UnitImpl* UnitImpl::getNext() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->nextUnit);
  }
  //---------------------------------------- GET REMAINING BUILD TIME ----------------------------------------
  int UnitImpl::getRemainingBuildTime() const
  {
    return self->remainingBuildTime;
  }
  //---------------------------------------- GET REMAINING TRAIN TIME ----------------------------------------
  int UnitImpl::getRemainingTrainTime() const
  {
    return self->remainingTrainTime;
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

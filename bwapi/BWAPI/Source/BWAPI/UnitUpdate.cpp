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
#include "BWAPI/PlayerImpl.h"
#include <BWAPI/WeaponType.h>

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>
#include "Server.h"
#include "BWtoBWAPI.h"
namespace BWAPI
{
  void UnitImpl::updateInternalData()
  {
    int selfPlayerID = BroodwarImpl.server.getPlayerID(Broodwar->self());
    if (isAlive)
    {
      //------------------------------------------------------------------------------------------------------
      //_getPlayer
      _getPlayer = (Player*)BroodwarImpl.players[getOriginalRawData->playerID];
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i=0;i<9;i++)
      {
        if (i == selfPlayerID)
          continue;
        PlayerImpl* player = (PlayerImpl*)Broodwar->getPlayer(i);
        if ( !getOriginalRawData->sprite || !player )
          self->isVisible[i] = false;
        else if (!BroodwarImpl._isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          self->isVisible[i] = false;
        else if (_getPlayer == (Player*)player)
          self->isVisible[i] = true;
        else if (player->isNeutral())
          self->isVisible[i] = getOriginalRawData->sprite->visibilityFlags > 0;
        else
          self->isVisible[i] = (getOriginalRawData->sprite->visibilityFlags & (1 << player->getIndex())) != 0;
      }
      if (selfPlayerID > -1)
      {
        if ( !getOriginalRawData->sprite )
        {
          self->isVisible[selfPlayerID] = false;
          self->isDetected = false;
        }
        else if (_getPlayer == BWAPI::BroodwarImpl.self())
        {
          self->isVisible[selfPlayerID] = true;
          self->isDetected = true;
        }
        else
        {
          self->isVisible[selfPlayerID] = (getOriginalRawData->sprite->visibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0;
          if (getOriginalRawData->status.getBit(BW::StatusFlags::RequiresDetection))
          {
            self->isVisible[selfPlayerID] &= ((getOriginalRawData->visibilityStatus == -1) ||
                                             ((getOriginalRawData->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0) ||
                                               getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Moving) ||
                                               getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Accelerating) ||
                                               getOriginalRawData->orderID == BW::OrderID::Move ||
                                               getOriginalRawData->groundWeaponCooldown > 0 ||
                                               getOriginalRawData->airWeaponCooldown > 0 ||
                                              !getOriginalRawData->status.getBit(BW::StatusFlags::Burrowed));
          }
          bool canDetect = !getOriginalRawData->status.getBit(BW::StatusFlags::RequiresDetection) ||
                           (getOriginalRawData->visibilityStatus == -1) ||
                          ((getOriginalRawData->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0);
          self->isDetected = self->isVisible[selfPlayerID] & canDetect;
        }
      }
      else
      {
        self->isDetected = false;
        for(int i = 0; i < 9; ++i)
          if (self->isVisible[i]) self->isDetected = true;
      }
      //------------------------------------------------------------------------------------------------------
      //_getType
      u16 uId = getOriginalRawData->unitType.id;
      if ( uId == BW::UnitID::Resource_MineralPatch1 ||
           uId == BW::UnitID::Resource_MineralPatch2 ||
           uId == BW::UnitID::Resource_MineralPatch3)
      {
        _getType = UnitTypes::Resource_Mineral_Field;
      }
      else
      {
        _getType = UnitType(uId);
      }
      //------------------------------------------------------------------------------------------------------
      //_getTransport
      if (_getType == UnitTypes::Protoss_Interceptor ||
          _getType == UnitTypes::Protoss_Scarab ||
          _getType == UnitTypes::Terran_Vulture_Spider_Mine)
      {
        if (getOriginalRawData->interceptor.inHanger ==0 ||
            getOriginalRawData->status.getBit(BW::StatusFlags::InTransport) ||
            getOriginalRawData->status.getBit(BW::StatusFlags::InBuilding))
          _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->interceptor.parent));
        else
          _getTransport = NULL;
      }
      else if (getOriginalRawData->status.getBit(BW::StatusFlags::InTransport) ||
               getOriginalRawData->status.getBit(BW::StatusFlags::InBuilding))
        _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->connectedUnit));
      else
        _getTransport = NULL;
      //------------------------------------------------------------------------------------------------------
      //_getPosition
      if ( _getTransport )
        _getPosition = Position(((UnitImpl*)_getTransport)->getOriginalRawData->position.x,((UnitImpl*)_getTransport)->getOriginalRawData->position.y);
      else
        _getPosition = Position(getOriginalRawData->position.x, getOriginalRawData->position.y);
      //------------------------------------------------------------------------------------------------------
      //_getHitPoints
      _getHitPoints = (int)ceil(getOriginalRawData->hitPoints / 256.0);
      //------------------------------------------------------------------------------------------------------
      //_getResources
      if (_getType != UnitTypes::Resource_Mineral_Field &&
          _getType != UnitTypes::Resource_Vespene_Geyser &&
          _getType != UnitTypes::Terran_Refinery &&
          _getType != UnitTypes::Protoss_Assimilator &&
          _getType != UnitTypes::Zerg_Extractor)
        _getResources = 0;
      else
        _getResources = getOriginalRawData->resource.resourceContained;
      //------------------------------------------------------------------------------------------------------
      //getBuildQueueSlot
      getBuildQueueSlot = getOriginalRawData->buildQueueSlot;
      //------------------------------------------------------------------------------------------------------
      //getBuildQueue
      getBuildQueue = (BW::UnitType*)getOriginalRawData->buildQueue;
      //------------------------------------------------------------------------------------------------------
      //hasEmptyBuildQueue
      if (getBuildQueueSlot < 5)
        hasEmptyBuildQueue = (getBuildQueue[getBuildQueueSlot] == BW::UnitID::None);
      else
        hasEmptyBuildQueue = false;
      //------------------------------------------------------------------------------------------------------
      //_isCompleted
      _isCompleted = getOriginalRawData->status.getBit(BW::StatusFlags::Completed);
    }
    else
    {
      //------------------------------------------------------------------------------------------------------
      //_getPlayer
      _getPlayer = NULL;
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i=0;i<9;i++)
        self->isVisible[i] = false;
      //------------------------------------------------------------------------------------------------------
      //_getType
      _getType = UnitTypes::Unknown;
      //------------------------------------------------------------------------------------------------------
      //_getTransport
      _getTransport = NULL;
      //------------------------------------------------------------------------------------------------------
      //_getPosition
      _getPosition = Positions::Unknown;
      //------------------------------------------------------------------------------------------------------
      //_getHitPoints
      _getHitPoints = 0;
      //------------------------------------------------------------------------------------------------------
      //_getResources
      _getResources = 0;
      //------------------------------------------------------------------------------------------------------
      //getBuildQueueSlot
      getBuildQueueSlot = 0;
      //------------------------------------------------------------------------------------------------------
      //getBuildQueue
      getBuildQueue = NULL;
      //------------------------------------------------------------------------------------------------------
      //hasEmptyBuildQueue
      hasEmptyBuildQueue = true;
      //------------------------------------------------------------------------------------------------------
      //_isCompleted
      _isCompleted = false;
    }
  }
  void UnitImpl::updateData()
  {
    if (canAccess())
    {
      //------------------------------------------------------------------------------------------------------
      //getPosition
      self->positionX = _getPosition.x();
      self->positionY = _getPosition.y();
      //------------------------------------------------------------------------------------------------------
      //getAngle
      int d = getOriginalRawData->currentDirection1;
      d -= 64;
      if (d < 0)
        d += 256;
      self->angle = (double)d * 3.14159265358979323846 / 128.0;
      //------------------------------------------------------------------------------------------------------
      //getVelocityX
      self->velocityX = (double)getOriginalRawData->current_speedX / 256.0;
      //------------------------------------------------------------------------------------------------------
      //getVelocityY
      self->velocityY = (double)getOriginalRawData->current_speedY / 256.0;
      //------------------------------------------------------------------------------------------------------
      //getGroundWeaponCooldown
      if (_getType == UnitTypes::Protoss_Reaver)
        self->groundWeaponCooldown = getOriginalRawData->mainOrderTimer;
      else if ( getOriginalRawData->subUnit )
        self->groundWeaponCooldown = getOriginalRawData->subUnit->groundWeaponCooldown;
      else
        self->groundWeaponCooldown = getOriginalRawData->groundWeaponCooldown;
      //------------------------------------------------------------------------------------------------------
      //getAirWeaponCooldown
      if ( getOriginalRawData->subUnit )
        self->airWeaponCooldown = getOriginalRawData->subUnit->airWeaponCooldown;
      else
        self->airWeaponCooldown = getOriginalRawData->airWeaponCooldown;
      //------------------------------------------------------------------------------------------------------
      //getSpellCooldown
      self->spellCooldown = getOriginalRawData->spellCooldown;
      //------------------------------------------------------------------------------------------------------
      //isAttacking
      self->isAttacking = (animState == BW::Image::Anims::GndAttkRpt  ||
                           animState == BW::Image::Anims::AirAttkRpt  || 
                           animState == BW::Image::Anims::GndAttkInit ||
                           animState == BW::Image::Anims::AirAttkInit);
      //------------------------------------------------------------------------------------------------------
      //isBurrowed
      self->isBurrowed = getOriginalRawData->status.getBit(BW::StatusFlags::Burrowed);
      //------------------------------------------------------------------------------------------------------
      //isCloaked
      self->isCloaked = getOriginalRawData->status.getBit(BW::StatusFlags::Cloaked) && !getOriginalRawData->status.getBit(BW::StatusFlags::Burrowed);
      //------------------------------------------------------------------------------------------------------
      //isCompleted
      self->isCompleted = _isCompleted;
      //------------------------------------------------------------------------------------------------------
      //isMoving
      self->isMoving = getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Moving) ||
                       getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Accelerating) ||
                       (self->order == Orders::Move.getID());
      //------------------------------------------------------------------------------------------------------
      //isStartingAttack
      self->isStartingAttack = startingAttack;
    }
    else
    {
      //------------------------------------------------------------------------------------------------------
      //getPosition
      self->positionX = BWAPI::Positions::Unknown.x();
      self->positionX = BWAPI::Positions::Unknown.y();
      //------------------------------------------------------------------------------------------------------
      //getAngle
      self->angle = 0;
      //------------------------------------------------------------------------------------------------------
      //getVelocityX
      self->velocityX = 0;
      //------------------------------------------------------------------------------------------------------
      //getVelocityY
      self->velocityY = 0;
      //------------------------------------------------------------------------------------------------------
      //getGroundWeaponCooldown
      self->groundWeaponCooldown = 0;
      //------------------------------------------------------------------------------------------------------
      //getAirWeaponCooldown
      self->airWeaponCooldown = 0;
      //------------------------------------------------------------------------------------------------------
      //getSpellCooldown
      self->spellCooldown = 0;
      //------------------------------------------------------------------------------------------------------
      //isAttacking
      self->isAttacking = false;
      //------------------------------------------------------------------------------------------------------
      //isBurrowed
      self->isBurrowed = false;
      //------------------------------------------------------------------------------------------------------
      //isCloaked
      self->isCloaked = false;
      //------------------------------------------------------------------------------------------------------
      //isCompleted
      self->isCompleted = false;
      //------------------------------------------------------------------------------------------------------
      //isMoving
      self->isMoving = false;
      //------------------------------------------------------------------------------------------------------
      //isStartingAttack
      self->isStartingAttack = false;
    }
    if (canAccessDetected())
    {
      //------------------------------------------------------------------------------------------------------
      //getHitPoints
      if ( !wasAccessible )
        self->lastHitPoints = _getHitPoints;
      else
        self->lastHitPoints = self->hitPoints;
      self->hitPoints = _getHitPoints;
      //------------------------------------------------------------------------------------------------------
      //getShields
      if (this->_getType.maxShields()>0)
        self->shields = (int)ceil(getOriginalRawData->shieldPoints/256.0);
      else
        self->shields = 0;
      //------------------------------------------------------------------------------------------------------
      //getEnergy
      if (this->_getType.isSpellcaster())
        self->energy = (int)ceil(getOriginalRawData->energy/256.0);
      else
        self->energy = 0;
      //------------------------------------------------------------------------------------------------------
      //getResources
      self->resources = _getResources;
      //------------------------------------------------------------------------------------------------------
      //getKillCount
      self->killCount = getOriginalRawData->killCount;
      //------------------------------------------------------------------------------------------------------
      //getDefenseMatrixPoints
      self->defenseMatrixPoints = getOriginalRawData->defenseMatrixDamage/256;
      //------------------------------------------------------------------------------------------------------
      //getDefenseMatrixTimer
      self->defenseMatrixTimer = getOriginalRawData->defenseMatrixTimer;
      //------------------------------------------------------------------------------------------------------
      //getEnsnareTimer
      self->ensnareTimer = getOriginalRawData->ensnareTimer;
      //------------------------------------------------------------------------------------------------------
      //getIrradiateTimer
      self->irradiateTimer = getOriginalRawData->irradiateTimer;
      //------------------------------------------------------------------------------------------------------
      //getLockdownTimer
      self->lockdownTimer = getOriginalRawData->lockdownTimer;
      //------------------------------------------------------------------------------------------------------
      //getMaelstromTimer
      self->maelstromTimer = getOriginalRawData->maelstromTimer;
      //------------------------------------------------------------------------------------------------------
      //getOrderTimer
      self->orderTimer = getOriginalRawData->mainOrderTimer;
      //------------------------------------------------------------------------------------------------------
      //getPlagueTimer
      self->plagueTimer = getOriginalRawData->plagueTimer;
      //------------------------------------------------------------------------------------------------------
      //getRemoveTimer
      self->removeTimer = getOriginalRawData->removeTimer;
      //------------------------------------------------------------------------------------------------------
      //getStasisTimer
      self->stasisTimer = getOriginalRawData->stasisTimer;
      //------------------------------------------------------------------------------------------------------
      //getStimTimer
      self->stimTimer = getOriginalRawData->stimTimer;
      //------------------------------------------------------------------------------------------------------
      //getOrder
      self->order = getOriginalRawData->orderID;
      //------------------------------------------------------------------------------------------------------
      //getSecondaryOrder
      self->secondaryOrder = getOriginalRawData->secondaryOrderID;
      //------------------------------------------------------------------------------------------------------
      //getBuildUnit
      if (getOriginalRawData->currentBuildUnit)
        self->buildUnit = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->currentBuildUnit));
      else
        self->buildUnit = -1;
      //------------------------------------------------------------------------------------------------------
      //isTraining
      if (_getType == UnitTypes::Terran_Nuclear_Silo &&
          getOriginalRawData->secondaryOrderID == BW::OrderID::Train)
        self->isTraining = true;
      else if (!_getType.canProduce())
        self->isTraining = false;
      else if (_getType.getRace()==Races::Zerg && _getType.isResourceDepot())
        self->isTraining = false;
      else
        self->isTraining = !hasEmptyBuildQueue;
      //------------------------------------------------------------------------------------------------------
      //isMorphing
      self->isMorphing = self->order == BW::OrderID::ZergBirth ||
                         self->order == BW::OrderID::ZergBuildingMorph ||
                         self->order == BW::OrderID::ZergUnitMorph ||
                         self->order == BW::OrderID::ZergBuildSelf;

      if (self->isCompleted && self->isMorphing)
      {
        self->isCompleted = false;
        _isCompleted = false;
      }
      //------------------------------------------------------------------------------------------------------
      //isConstructing
      self->isConstructing = self->isMorphing ||
              (self->order == BW::OrderID::ConstructingBuilding) || 
              (self->order == BW::OrderID::BuildTerran) ||
              (self->order == BW::OrderID::DroneBuild) ||
              (self->order == BW::OrderID::DroneStartBuild) ||
              (self->order == BW::OrderID::DroneLand) ||
              (self->order == BW::OrderID::BuildProtoss1) ||
              (self->order == BW::OrderID::BuildProtoss2) ||
              (self->order == BW::OrderID::TerranBuildSelf) ||
              (self->order == BW::OrderID::ProtossBuildSelf) ||
              (self->order == BW::OrderID::ZergBuildSelf) ||
              (self->order == BW::OrderID::BuildNydusExit) ||
              (self->order == BW::OrderID::BuildAddon) ||
              (self->secondaryOrder == BW::OrderID::BuildAddon) ||
              (!self->isCompleted && self->buildUnit != -1);
      //------------------------------------------------------------------------------------------------------
      //isIdle
      if (self->isTraining ||
          self->isConstructing ||
          self->isMorphing ||
         (self->order == BW::OrderID::ResearchTech) ||
         (self->order == BW::OrderID::Upgrade))
        self->isIdle = false;
      else
        self->isIdle = (self->order == BW::OrderID::PlayerGuard) ||
                       (self->order == BW::OrderID::Guard) ||
                       (self->order == BW::OrderID::Stop) ||
                       (self->order == BW::OrderID::PickupIdle) ||
                       (self->order == BW::OrderID::Nothing) ||
                       (self->order == BW::OrderID::Medic) ||
                       (self->order == BW::OrderID::Carrier) ||
                       (self->order == BW::OrderID::Reaver) ||
                       (self->order == BW::OrderID::Critter) ||
                       (self->order == BW::OrderID::Neutral) ||
                       (self->order == BW::OrderID::TowerGuard) ||
                       (self->order == BW::OrderID::Burrowed) ||
                       (self->order == BW::OrderID::NukeTrain) ||
                       (self->order == BW::OrderID::Larva);
      //------------------------------------------------------------------------------------------------------
      //getTarget
      self->target = BroodwarImpl.server.getUnitID((Unit*)UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->targetUnit));
      //------------------------------------------------------------------------------------------------------
      //getTargetPosition
      self->targetPositionX = getOriginalRawData->moveToPos.x;
      self->targetPositionY = getOriginalRawData->moveToPos.y;
      //------------------------------------------------------------------------------------------------------
      //getOrderTarget
      self->orderTarget = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->orderTargetUnit));
      //------------------------------------------------------------------------------------------------------
      //getAddon
      if (_getType.isBuilding())
      {
        UnitImpl* addon = UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->currentBuildUnit);
        if ( addon && addon->isAlive && UnitType(addon->getOriginalRawData->unitType.id).isAddon() )
          self->addon = BroodwarImpl.server.getUnitID(addon);
        else
        {
          addon = UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->building.addon);
          if ( addon && addon->isAlive && UnitType(addon->getOriginalRawData->unitType.id).isAddon() )
            self->addon = BroodwarImpl.server.getUnitID(addon);
          else
            self->addon = -1;
        }
      }
      else
        self->addon = -1;
      //------------------------------------------------------------------------------------------------------
      //getNydusExit
      if (_getType!=UnitTypes::Zerg_Nydus_Canal)
        self->nydusExit = -1;
      else
      {
        UnitImpl* nydus = UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->nydus.exit);
        if ( nydus && nydus->isAlive && nydus->getOriginalRawData->unitType == BW::UnitID::Zerg_NydusCanal )
          self->nydusExit = BroodwarImpl.server.getUnitID(nydus);
        else
        {
          self->nydusExit = -1;
        }
      }
      //------------------------------------------------------------------------------------------------------
      //isAccelerating
      self->isAccelerating = getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Accelerating);
      //------------------------------------------------------------------------------------------------------
      //isBeingGathered
      self->isBeingGathered = _getType.isResourceContainer() && getOriginalRawData->resource.isBeingGathered != 0;
      //------------------------------------------------------------------------------------------------------
      //isBlind
      self->isBlind = getOriginalRawData->isBlind != 0;
      //------------------------------------------------------------------------------------------------------
      //isBraking
      self->isBraking = getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Braking);
      //------------------------------------------------------------------------------------------------------
      //isCarryingGas
      //isCarryingMinerals
      if (_getType.isWorker())
        self->carryResourceType = getOriginalRawData->resourceType;
      else
        self->carryResourceType = 0;
      //------------------------------------------------------------------------------------------------------
      //isGatheringGas
      //isGatheringMinerals
      self->isGathering = _getType.isWorker() && getOriginalRawData->status.getBit(BW::StatusFlags::IsGathering);
      //------------------------------------------------------------------------------------------------------
      //isLifted
      self->isLifted = getOriginalRawData->status.getBit(BW::StatusFlags::InAir) &&
                       getOriginalRawData->unitType.isBuilding();
      //------------------------------------------------------------------------------------------------------
      //isParasited
      self->isParasited = getOriginalRawData->parasiteFlags.value != 0;
      //------------------------------------------------------------------------------------------------------
      //isSelected
      self->isSelected = BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) && userSelected;
      //------------------------------------------------------------------------------------------------------
      //isUnderStorm
      self->isUnderStorm = getOriginalRawData->isUnderStorm != 0;
      //------------------------------------------------------------------------------------------------------
      //isUnpowered
      self->isUnpowered = _getType.getRace() == Races::Protoss && _getType.isBuilding() && getOriginalRawData->status.getBit(BW::StatusFlags::DoodadStatesThing);
    }
    else
    {
      self->lastHitPoints       = 0;      //getHitPoints
      self->hitPoints           = 0;      //getHitPoints
      self->shields             = 0;      //getShields
      self->energy              = 0;      //getEnergy
      self->resources           = 0;      //getResources
      self->killCount           = 0;      //getKillCount
      self->defenseMatrixPoints = 0;      //getDefenseMatrixPoints
      self->defenseMatrixTimer  = 0;      //getDefenseMatrixTimer
      self->ensnareTimer        = 0;      //getEnsnareTimer
      self->irradiateTimer      = 0;      //getIrradiateTimer
      self->lockdownTimer       = 0;      //getLockdownTimer
      self->maelstromTimer      = 0;      //getMaelstromTimer
      self->orderTimer          = 0;      //getOrderTimer
      self->plagueTimer         = 0;      //getPlagueTimer
      self->removeTimer         = 0;      //getRemoveTimer
      self->stasisTimer         = 0;      //getStasisTimer
      self->stimTimer           = 0;      //getStimTimer
      self->order               = BW::OrderID::None;  //getOrder
      self->secondaryOrder      = BW::OrderID::None;  //getSecondaryOrder
      self->buildUnit           = -1;     //getBuildUnit
      self->isTraining          = false;  //isTraining
      self->isMorphing          = false;  //isMorphing
      self->isConstructing      = false;  //isConstructing
      self->isIdle              = false;  //isIdle
      self->target              = -1;     //getTarget
      self->targetPositionX     = Positions::Unknown.x(); //getTargetPosition
      self->targetPositionY     = Positions::Unknown.y(); //getTargetPosition
      self->orderTarget         = -1;     //getOrderTarget
      self->addon               = -1;     //getAddon
      self->nydusExit           = -1;     //getNydusExit
      self->isAccelerating      = false;  //isAccelerating
      self->isBeingGathered     = false;  //isBeingGathered
      self->isBlind             = false;  //isBlind
      self->isBraking           = false;  //isBraking
      self->carryResourceType   = 0;      //isCarryingMinerals;isCarryingGas
      self->isLifted            = false;  //isLifted
      self->isParasited         = false;  //isParasited
      self->isSelected          = false;  //isSelected
      self->isUnderStorm        = false;  //isUnderStorm
      self->isUnpowered         = false;  //isUnpowered

    }
    if (canAccess())
    {
      self->exists = true;
      self->player = BroodwarImpl.server.getPlayerID(_getPlayer);
      self->type   = _getType.getID();
    }
    else
    {
      self->exists = false;
      self->player = BroodwarImpl.server.getPlayerID((Player*)BroodwarImpl.players[11]);
      self->type   = UnitTypes::Unknown.getID();
    }
    if (canAccessInside())
    {

      // Default assignments
      self->scarabCount        = 0;
      self->spiderMineCount    = 0;
      self->trainingQueueCount = 0;
      self->remainingTrainTime = 0;
      self->carrier            = -1;
      self->hatchery           = -1;
      self->hasNuke            = false;

      //------------------------------------------------------------------------------------------------------
      // getTrainingQueue
      if ( !hasEmptyBuildQueue )
      {
        for(int i = getBuildQueueSlot % 5; getBuildQueue[i] != BW::UnitID::None && self->trainingQueueCount < 5; i = (i + 1) % 5)
        {
          self->trainingQueue[self->trainingQueueCount] = getBuildQueue[i].id;
          self->trainingQueueCount++;
        }
      }
      //------------------------------------------------------------------------------------------------------
      // getRemainingTrainTime
      if ( getOriginalRawData->currentBuildUnit )
        self->remainingTrainTime = getOriginalRawData->currentBuildUnit->remainingBuildTime;

      // Unit Type switch
      switch ( _getType.getID() )
      {
      case BW::UnitID::Protoss_Reaver:
        self->scarabCount = getOriginalRawData->carrier.inHangerCount;
        break;
      case BW::UnitID::Terran_Vulture:
        self->spiderMineCount = getOriginalRawData->vulture.spiderMineCount;
        break;
      case BW::UnitID::Terran_NuclearSilo:
        if (getOriginalRawData->secondaryOrderID == BW::OrderID::Train)
        {
          self->trainingQueue[0]   = BW::UnitID::Terran_NuclearMissile;
          self->trainingQueueCount = 1;
        }
        self->hasNuke = (getOriginalRawData->hasNuke != 0);
        break;
      case BW::UnitID::Zerg_Hatchery:
      case BW::UnitID::Zerg_Lair:
      case BW::UnitID::Zerg_Hive:
        if ( !self->isCompleted && self->buildType == BW::UnitID::Zerg_Hatchery )
          self->remainingTrainTime = self->remainingBuildTime;
        else
          self->remainingTrainTime = getOriginalRawData->building.larvaTimer * 9 + ((getOriginalRawData->orderQueueTimer + 8) % 9);
        break;
      case BW::UnitID::Protoss_Interceptor:
        self->carrier = BroodwarImpl.server.getUnitID((Unit*)(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->interceptor.parent)));
        break;
      case BW::UnitID::Zerg_Larva:
        self->hatchery = BroodwarImpl.server.getUnitID((Unit*)(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->connectedUnit)));
        break;
      default:
        break;
      }
      //------------------------------------------------------------------------------------------------------
      //getBuildType
      if (self->order == BW::OrderID::TerranBuildSelf ||
          self->order == BW::OrderID::ProtossBuildSelf)
        self->buildType = self->type;
      else if (self->order == BW::OrderID::ZergBuildSelf)
      {
        int i = getBuildQueueSlot % 5;
        int type = getBuildQueue[i].id;
        if (type != UnitTypes::None.getID())
          self->buildType = type;
        else
          self->buildType = self->type;
      }
      else if (self->order == BW::OrderID::ConstructingBuilding && self->buildUnit != -1)
        self->buildType = ((UnitImpl*)getBuildUnit())->getOriginalRawData->unitType.id;
      else if (hasEmptyBuildQueue || self->isIdle)
        self->buildType = UnitTypes::None.getID();
      else if (self->order == BW::OrderID::BuildTerran ||
               self->order == BW::OrderID::BuildProtoss1 ||
               self->order == BW::OrderID::ZergUnitMorph ||
               self->order == BW::OrderID::ZergBuildingMorph ||
               self->order == BW::OrderID::DroneLand ||
               self->order == BW::OrderID::ZergBuildSelf ||
               self->secondaryOrder == BW::OrderID::BuildAddon)
      {
        self->buildType = getBuildQueue[(getBuildQueueSlot % 5)].id;
      }
      else
        self->buildType = UnitTypes::None.getID();
      //------------------------------------------------------------------------------------------------------
      //getTech
      //getRemainingResearchTime
      if (self->order == BW::OrderID::ResearchTech)
      {
        self->tech = getOriginalRawData->building.techType;
        self->remainingResearchTime = getOriginalRawData->building.upgradeResearchTime;
      }
      else
      {
        self->tech = BW::TechID::None;
        self->remainingResearchTime = 0;
      }
      //------------------------------------------------------------------------------------------------------
      //getUpgrade
      //getRemainingUpgradeTime
      if (self->order == BW::OrderID::Upgrade)
      {
        self->upgrade = getOriginalRawData->building.upgradeType;
        self->remainingUpgradeTime = getOriginalRawData->building.upgradeResearchTime;
      }
      else
      {
        self->upgrade = BW::UpgradeID::None;
        self->remainingUpgradeTime = 0;
      }
      //------------------------------------------------------------------------------------------------------
      //getRemainingBuildTime
      if (self->isMorphing && self->buildType == BW::UnitID::None)
        self->remainingBuildTime = 0;
      else
        self->remainingBuildTime = getOriginalRawData->remainingBuildTime;
      //------------------------------------------------------------------------------------------------------
      //getRallyPosition
      if (this->_getType.canProduce())
      {
        self->rallyPositionX = getOriginalRawData->rally.x;
        self->rallyPositionY = getOriginalRawData->rally.y;
      }
      else
      {
        self->rallyPositionX = Positions::None.x();
        self->rallyPositionY = Positions::None.y();
      }
      //------------------------------------------------------------------------------------------------------
      //getRallyUnit
      if (this->_getType.canProduce())
        self->rallyUnit = BroodwarImpl.server.getUnitID((Unit*)UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->rally.unit));
      else
        self->rallyUnit = -1;
      //------------------------------------------------------------------------------------------------------
      //getTransport
      self->transport = BroodwarImpl.server.getUnitID(_getTransport);
      //------------------------------------------------------------------------------------------------------
      //isHallucination
      self->isHallucination = getOriginalRawData->status.getBit(BW::StatusFlags::IsHallucination);
    }
    else
    {
      self->scarabCount           = 0;                    //getScarabCount
      self->spiderMineCount       = 0;                    //getSpiderMineCount
      self->buildType             = BW::UnitID::None;     //getBuildType
      self->trainingQueueCount    = 0;                    //getTrainingQueue
      self->tech                  = BW::TechID::None;     //getTech
      self->upgrade               = BW::UpgradeID::None;  //getUpgrade
      self->remainingBuildTime    = 0;                    //getRemainingBuildTime
      self->remainingTrainTime    = 0;                    //getRemainingTrainTime
      self->remainingResearchTime = 0;                    //getRemainingResearchTime
      self->remainingUpgradeTime  = 0;                    //getRemainingUpgradeTime
      self->rallyPositionX        = Positions::None.x();  //getRallyPosition
      self->rallyPositionY        = Positions::None.y();  //getRallyPosition
      self->rallyUnit             = -1;                   //getRallyUnit
      self->transport             = -1;                   //getTransport
      self->carrier               = -1;                   //getCarrier
      self->hatchery              = -1;                   //getHatchery
      self->hasNuke               = false;                //hasNuke
      self->isHallucination       = false;                //isHallucination
    }
    if (self->order >= 0)
      self->order = BWtoBWAPI_Order[self->order];
    if (self->secondaryOrder >= 0)
      self->secondaryOrder = BWtoBWAPI_Order[self->secondaryOrder];
  }
}
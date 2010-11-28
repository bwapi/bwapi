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
#include <BW/Path.h>
#include "Server.h"
#include "BWtoBWAPI.h"
namespace BWAPI
{
  void UnitImpl::updateInternalData()
  {
    int selfPlayerID = BroodwarImpl.server.getPlayerID(Broodwar->self());
    if (isAlive)
    {
      _getPlayer = (Player*)BroodwarImpl.players[getOriginalRawData->playerID]; //_getPlayer
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i = 0; i < 9; ++i)
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
      if (selfPlayerID >= 0)
      {
        if ( !getOriginalRawData->sprite )
        {
          self->isVisible[selfPlayerID] = false;
          self->isDetected              = false;
        }
        else if (_getPlayer == BWAPI::BroodwarImpl.self())
        {
          self->isVisible[selfPlayerID] = true;
          self->isDetected              = true;
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
      _getType = UnitType(uId);
      if ( uId == BW::UnitID::Resource_MineralPatch1 ||
           uId == BW::UnitID::Resource_MineralPatch2 ||
           uId == BW::UnitID::Resource_MineralPatch3)
        _getType = UnitTypes::Resource_Mineral_Field;

      getBuildQueueSlot = getOriginalRawData->buildQueueSlot; //getBuildQueueSlot
      getBuildQueue = (BW::UnitType*)getOriginalRawData->buildQueue;  //getBuildQueue

      if (_getType.isBuilding())
      {
        if (getOriginalRawData->orderID == BW::OrderID::ZergBirth ||
            getOriginalRawData->orderID == BW::OrderID::ZergBuildingMorph ||
            getOriginalRawData->orderID == BW::OrderID::ZergUnitMorph ||
            getOriginalRawData->orderID == BW::OrderID::ZergBuildSelf)
        {
          //if we have a morphing building, set unit type to the build type (what it is morphing to)
          u16 uId2 = getBuildQueue[(getBuildQueueSlot % 5)].id;
          if (uId2 != BW::UnitID::None)
            _getType = UnitType(uId2);
        }
      }

      //------------------------------------------------------------------------------------------------------
      //_getTransport
      _getTransport = NULL;
      if (_getType == UnitTypes::Protoss_Interceptor  ||
          _getType == UnitTypes::Protoss_Scarab       ||
          _getType == UnitTypes::Terran_Vulture_Spider_Mine)
      {
        if (getOriginalRawData->interceptor.inHanger == 0 ||
            getOriginalRawData->status.getBit(BW::StatusFlags::InTransport) ||
            getOriginalRawData->status.getBit(BW::StatusFlags::InBuilding))
          _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->interceptor.parent));
      }
      else if (getOriginalRawData->status.getBit(BW::StatusFlags::InTransport) ||
               getOriginalRawData->status.getBit(BW::StatusFlags::InBuilding))
        _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->connectedUnit));

      //------------------------------------------------------------------------------------------------------
      //_getPosition
      if ( _getTransport )
        _getPosition = Position(((UnitImpl*)_getTransport)->getOriginalRawData->position.x,((UnitImpl*)_getTransport)->getOriginalRawData->position.y);
      else
        _getPosition = Position(getOriginalRawData->position.x, getOriginalRawData->position.y);

      _getHitPoints = (int)ceil(getOriginalRawData->hitPoints / 256.0); //_getHitPoints
      //------------------------------------------------------------------------------------------------------
      //_getResources
      _getResources = 0;
      if (_getType == UnitTypes::Resource_Mineral_Field   ||
          _getType == UnitTypes::Resource_Vespene_Geyser  ||
          _getType == UnitTypes::Terran_Refinery          ||
          _getType == UnitTypes::Protoss_Assimilator      ||
          _getType == UnitTypes::Zerg_Extractor)
        _getResources = getOriginalRawData->building.resource.resourceCount;

      hasEmptyBuildQueue = getBuildQueueSlot < 5 ? (getBuildQueue[getBuildQueueSlot] == BW::UnitID::None) : false;  //hasEmptyBuildQueue
      _isCompleted = getOriginalRawData->status.getBit(BW::StatusFlags::Completed); //_isCompleted
    }
    else
    {
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i = 0; i < 9; ++i)
        self->isVisible[i] = false;

      _getPlayer          = NULL;               //_getPlayer
      _getType            = UnitTypes::Unknown; //_getType
      _getTransport       = NULL;               //_getTransport
      _getPosition        = Positions::Unknown; //_getPosition
      _getHitPoints       = 0;                  //_getHitPoints
      _getResources       = 0;                  //_getResources
      getBuildQueueSlot   = 0;                  //getBuildQueueSlot
      getBuildQueue       = NULL;               //getBuildQueue
      hasEmptyBuildQueue  = true;               //hasEmptyBuildQueue
      _isCompleted        = false;              //_isCompleted
    }
  }
  void UnitImpl::updateData()
  {
    if (canAccess())
    {
      self->positionX = _getPosition.x(); //getPosition
      self->positionY = _getPosition.y(); //getPosition
      //------------------------------------------------------------------------------------------------------
      //getAngle
      int d = getOriginalRawData->currentDirection1;
      d -= 64;
      if (d < 0)
        d += 256;

      self->angle     = (double)d * 3.14159265358979323846 / 128.0;
      self->velocityX = (double)getOriginalRawData->current_speedX / 256.0; //getVelocityX
      self->velocityY = (double)getOriginalRawData->current_speedY / 256.0; //getVelocityY
      //------------------------------------------------------------------------------------------------------
      //getGroundWeaponCooldown
      if ( _getType == UnitTypes::Protoss_Reaver || _getType == UnitTypes::Hero_Warbringer )
        self->groundWeaponCooldown = getOriginalRawData->mainOrderTimer;
      else if ( getOriginalRawData->subUnit )
        self->groundWeaponCooldown = getOriginalRawData->subUnit->groundWeaponCooldown;
      else
        self->groundWeaponCooldown = getOriginalRawData->groundWeaponCooldown;

      self->airWeaponCooldown = getOriginalRawData->subUnit ? getOriginalRawData->subUnit->airWeaponCooldown : getOriginalRawData->airWeaponCooldown; //getAirWeaponCooldown
      self->spellCooldown = getOriginalRawData->spellCooldown;  //getSpellCooldown
      u8 animState = 0;
      if ( getOriginalRawData->sprite && getOriginalRawData->sprite->mainGraphic )
        animState = getOriginalRawData->sprite->mainGraphic->anim;
      self->isAttacking = (animState == BW::Image::Anims::GndAttkRpt  ||  //isAttacking
                           animState == BW::Image::Anims::AirAttkRpt  || 
                           animState == BW::Image::Anims::GndAttkInit ||
                           animState == BW::Image::Anims::AirAttkInit);
      //isAttackFrame
      self->isAttackFrame = false;
      if ( getOriginalRawData->sprite && getOriginalRawData->sprite->mainGraphic )
      { 
        self->isAttackFrame = startingAttack || (self->isAttacking && AttackAnimationRestFrame[_getType.getID()]!=10000 && (getOriginalRawData->sprite->mainGraphic->frameSet!=AttackAnimationRestFrame[_getType.getID()] || lastFrameSet!=AttackAnimationRestFrame[_getType.getID()]));
        lastFrameSet = getOriginalRawData->sprite->mainGraphic->frameSet;
      }

      self->isBurrowed = getOriginalRawData->status.getBit(BW::StatusFlags::Burrowed);  //isBurrowed
      self->isCloaked = getOriginalRawData->status.getBit(BW::StatusFlags::Cloaked) && !getOriginalRawData->status.getBit(BW::StatusFlags::Burrowed); //isCloaked
      self->isCompleted = _isCompleted; //isCompleted
      self->isMoving = getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Moving) ||
                       getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Accelerating) ||
                       self->order == BW::OrderID::Move; //isMoving
      self->isStartingAttack = startingAttack;  //isStartingAttack
    }
    else
    {
      self->positionX             = BWAPI::Positions::Unknown.x();  //getPosition
      self->positionX             = BWAPI::Positions::Unknown.y();  //getPosition
      self->angle                 = 0;      //getAngle
      self->velocityX             = 0;      //getVelocityX
      self->velocityY             = 0;      //getVelocityY
      self->groundWeaponCooldown  = 0;      //getGroundWeaponCooldown
      self->airWeaponCooldown     = 0;      //getAirWeaponCooldown
      self->spellCooldown         = 0;      //getSpellCooldown
      self->isAttacking           = false;  //isAttacking
      self->isBurrowed            = false;  //isBurrowed
      self->isCloaked             = false;  //isCloaked
      self->isCompleted           = false;  //isCompleted
      self->isMoving              = false;  //isMoving
      self->isStartingAttack      = false;  //isStartingAttac
    }
    if (canAccessDetected())
    {
      self->lastHitPoints       = wasAccessible ? self->hitPoints : _getHitPoints;  //getHitPoints
      self->hitPoints           = _getHitPoints;  //getHitPoints
      self->shields             = _getType.maxShields() > 0 ? (int)ceil(getOriginalRawData->shieldPoints/256.0) : 0;  //getShields
      self->energy              = _getType.isSpellcaster()  ? (int)ceil(getOriginalRawData->energy/256.0)       : 0;  //getEnergy
      self->resources           = _getResources;                        //getResources
      self->resourceGroup       = _getType.isResourceContainer() ? getOriginalRawData->building.resource.resourceGroup : 0; //getResourceGroup
      self->killCount           = getOriginalRawData->killCount;        //getKillCount
      self->defenseMatrixPoints = getOriginalRawData->defenseMatrixDamage/256;  //getDefenseMatrixPoints
      self->defenseMatrixTimer  = getOriginalRawData->defenseMatrixTimer; //getDefenseMatrixTimer
      self->ensnareTimer        = getOriginalRawData->ensnareTimer;     //getEnsnareTimer
      self->irradiateTimer      = getOriginalRawData->irradiateTimer;   //getIrradiateTimer
      self->lockdownTimer       = getOriginalRawData->lockdownTimer;    //getLockdownTimer
      self->maelstromTimer      = getOriginalRawData->maelstromTimer;   //getMaelstromTimer
      self->orderTimer          = getOriginalRawData->mainOrderTimer;   //getOrderTimer
      self->plagueTimer         = getOriginalRawData->plagueTimer;      //getPlagueTimer
      self->removeTimer         = getOriginalRawData->removeTimer;      //getRemoveTimer
      self->stasisTimer         = getOriginalRawData->stasisTimer;      //getStasisTimer
      self->stimTimer           = getOriginalRawData->stimTimer;        //getStimTimer
      self->order               = getOriginalRawData->orderID;          //getOrder
      self->secondaryOrder      = getOriginalRawData->secondaryOrderID; //getSecondaryOrder
      self->buildUnit           = getOriginalRawData->currentBuildUnit ? BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->currentBuildUnit)) : -1; //getBuildUnit
      //------------------------------------------------------------------------------------------------------
      //isTraining
      if (_getType == UnitTypes::Terran_Nuclear_Silo &&
          getOriginalRawData->secondaryOrderID == BW::OrderID::Train)
        self->isTraining = true;
      else if (!_getType.canProduce())
        self->isTraining = false;
      else if (_getType.getRace() == Races::Zerg && _getType.isResourceDepot())
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
        _isCompleted      = false;
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
      self->target          = BroodwarImpl.server.getUnitID((Unit*)UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->targetUnit)); //getTarget
      self->targetPositionX = getOriginalRawData->moveToPos.x;  //getTargetPosition
      self->targetPositionY = getOriginalRawData->moveToPos.y;  //getTargetPosition
      self->orderTarget     = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->orderTargetUnit));  //getOrderTarget
      //------------------------------------------------------------------------------------------------------
      //getAddon
      self->addon = -1;
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
        }
      }
      //------------------------------------------------------------------------------------------------------
      //getNydusExit
      self->nydusExit = -1;
      if ( _getType == UnitTypes::Zerg_Nydus_Canal )
      {
        UnitImpl* nydus = UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->building.nydus.exit);
        if ( nydus && nydus->isAlive && nydus->getOriginalRawData->unitType == BW::UnitID::Zerg_NydusCanal )
          self->nydusExit = BroodwarImpl.server.getUnitID(nydus);
      }
      //------------------------------------------------------------------------------------------------------
      //getPowerUp
      self->powerUp = -1;
      UnitImpl* powerUp = UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->worker.powerup);
      if (powerUp && powerUp->isAlive)
        self->powerUp = BroodwarImpl.server.getUnitID(powerUp);

      self->isAccelerating  = getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Accelerating);  //isAccelerating
      self->isBeingGathered = _getType.isResourceContainer() && (getOriginalRawData->building.resource.gatherQueueCount || getOriginalRawData->building.resource.nextGatherer);  //isBeingGathered
      self->isBlind         = getOriginalRawData->isBlind != 0;   //isBlind
      self->isBraking       = getOriginalRawData->movementFlags.getBit(BW::MovementFlags::Braking);   //isBraking
      //------------------------------------------------------------------------------------------------------
      //isCarryingGas
      //isCarryingMinerals
      if (_getType.isWorker())
        self->carryResourceType = getOriginalRawData->resourceType;
      else
        self->carryResourceType = 0;

      self->isGathering     = _getType.isWorker() && getOriginalRawData->status.getBit(BW::StatusFlags::IsGathering);   //isGatheringMinerals; isGatheringGas
      self->isLifted        = getOriginalRawData->status.getBit(BW::StatusFlags::InAir) &&
                              getOriginalRawData->unitType.isBuilding(); //isLifted
      self->isParasited     = getOriginalRawData->parasiteFlags.value != 0; //isParasited
      self->isSelected      = BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) && userSelected; //isSelected
      self->isUnderStorm    = getOriginalRawData->isUnderStorm != 0; //isUnderStorm
      self->isUnpowered     = _getType.getRace() == Races::Protoss && _getType.isBuilding() && getOriginalRawData->status.getBit(BW::StatusFlags::DoodadStatesThing); //isUnpowered
      self->isStuck         = getOriginalRawData->movementState == UM_MoveToLegal;
      self->isInterruptible = !getOriginalRawData->status.getBit(BW::StatusFlags::CanNotReceiveOrders); //isInterruptible
    }
    else
    {
      self->lastHitPoints       = 0;      //getHitPoints
      self->hitPoints           = 0;      //getHitPoints
      self->shields             = 0;      //getShields
      self->energy              = 0;      //getEnergy
      self->resources           = 0;      //getResources
      self->resourceGroup       = 0;      //getResourceGroup
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
      self->powerUp             = -1;     //getPowerUp
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
      self->isStuck             = false;  //isStuck
      self->isInterruptible     = false;  //isInterruptible
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
      self->scarabCount           = 0;
      self->spiderMineCount       = 0;
      self->trainingQueueCount    = 0;
      self->remainingTrainTime    = 0;
      self->carrier               = -1;
      self->hatchery              = -1;
      self->hasNuke               = false;
      self->buildType             = BW::UnitID::None;
      self->tech                  = BW::TechID::None;
      self->remainingResearchTime = 0;
      self->upgrade               = BW::UpgradeID::None;
      self->remainingUpgradeTime  = 0;
      self->remainingBuildTime    = 0;
      self->rallyUnit             = -1;

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

      //------------------------------------------------------------------------------------------------------
      // Unit Type switch; special cases
      switch ( _getType.getID() )
      {
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        self->scarabCount = getOriginalRawData->carrier.inHangerCount;
        break;
      case BW::UnitID::Terran_Vulture:
      case BW::UnitID::Terran_Hero_JimRaynorV:
        self->spiderMineCount = getOriginalRawData->vulture.spiderMineCount;
        break;
      case BW::UnitID::Terran_NuclearSilo:
        if (getOriginalRawData->secondaryOrderID == BW::OrderID::Train)
        {
          self->trainingQueue[0]   = BW::UnitID::Terran_NuclearMissile;
          self->trainingQueueCount = 1;
        }
        self->hasNuke = (getOriginalRawData->building.silo.hasNuke != 0);
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
      // Order Type switch; special cases
      switch ( self->order )
      {
        case BW::OrderID::TerranBuildSelf:
        case BW::OrderID::ProtossBuildSelf:
          self->buildType = self->type;
          break;
        case BW::OrderID::ConstructingBuilding:
          if ( self->buildUnit != -1 )
            self->buildType = ((UnitImpl*)getBuildUnit())->getOriginalRawData->unitType.id;
          break;
        case BW::OrderID::ZergBuildSelf:
          {
            int type = getBuildQueue[getBuildQueueSlot % 5].id;
            self->buildType = type == BW::UnitID::None ? self->type : type;
          }
          break;
        case BW::OrderID::BuildTerran:
        case BW::OrderID::BuildProtoss1:
        case BW::OrderID::ZergUnitMorph:
        case BW::OrderID::ZergBuildingMorph:
        case BW::OrderID::DroneLand:
          self->buildType = getBuildQueue[(getBuildQueueSlot % 5)].id;
          break;
        case BW::OrderID::ResearchTech:
          self->tech = getOriginalRawData->building.techType;
          self->remainingResearchTime = getOriginalRawData->building.upgradeResearchTime;
          break;
        case BW::OrderID::Upgrade:
          self->upgrade = getOriginalRawData->building.upgradeType;
          self->remainingUpgradeTime = getOriginalRawData->building.upgradeResearchTime;
          break;

      }

      //getBuildType
      if ( !hasEmptyBuildQueue &&
           !self->isIdle       &&
           self->secondaryOrder == BW::OrderID::BuildAddon )
        self->buildType = getBuildQueue[(getBuildQueueSlot % 5)].id;

      //------------------------------------------------------------------------------------------------------
      //getRemainingBuildTime
      if ( !self->isMorphing || self->buildType != BW::UnitID::None)
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
      if ( this->_getType.canProduce() )
        self->rallyUnit = BroodwarImpl.server.getUnitID((Unit*)UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->rally.unit));

      self->transport       = BroodwarImpl.server.getUnitID(_getTransport);   //getTransport
      self->isHallucination = getOriginalRawData->status.getBit(BW::StatusFlags::IsHallucination);  //isHallucination
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
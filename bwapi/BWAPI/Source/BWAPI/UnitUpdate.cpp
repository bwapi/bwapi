#include "UnitImpl.h"

#include <math.h>
#include <limits>
#include <sstream>

#include <Util/Logger.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include <BWAPI/GameImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/WeaponType.h>

#include <BW/Unit.h>
#include <BW/UnitStatusFlags.h>
#include <BW/MovementFlags.h>
#include <BW/UnitID.h>
#include <BW/Offsets.h>
#include <BW/Path.h>
#include "Server.h"
#include "BWtoBWAPI.h"

#include "../../Debug.h"

namespace BWAPI
{
  void UnitImpl::updateInternalData()
  {
    BW::Unit *o = getOriginalRawData;
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
        if ( !o->sprite || !player )
          self->isVisible[i] = false;
        else if (!BroodwarImpl._isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          self->isVisible[i] = false;
        else if (_getPlayer == (Player*)player)
          self->isVisible[i] = true;
        else if (player->isNeutral())
          self->isVisible[i] = o->sprite->visibilityFlags > 0;
        else
          self->isVisible[i] = (o->sprite->visibilityFlags & (1 << player->getIndex())) != 0;
      }
      if (selfPlayerID >= 0)
      {
        if ( !o->sprite )
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
          self->isVisible[selfPlayerID] = (o->sprite->visibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0;
          if (o->statusFlag(BW::StatusFlags::RequiresDetection))
          {
            self->isVisible[selfPlayerID] &= ((o->visibilityStatus == -1) ||
                                             ((o->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0) ||
                                               o->movementFlag(BW::MovementFlags::Moving | BW::MovementFlags::Accelerating) ||
                                               o->orderID == BW::OrderID::Move ||
                                               o->groundWeaponCooldown > 0 ||
                                               o->airWeaponCooldown > 0 ||
                                              !o->statusFlag(BW::StatusFlags::Burrowed) );
          }
          bool canDetect = !o->statusFlag(BW::StatusFlags::RequiresDetection) ||
                           o->visibilityStatus == -1 ||
                           ((o->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0);
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
      u16 uId = o->unitType;
      _getType = UnitType(uId);
      if ( uId == BW::UnitID::Resource_MineralPatch1 ||
           uId == BW::UnitID::Resource_MineralPatch2 ||
           uId == BW::UnitID::Resource_MineralPatch3)
        _getType = UnitTypes::Resource_Mineral_Field;

      getBuildQueueSlot = o->buildQueueSlot; //getBuildQueueSlot
      for ( int i = 0; i < 5; ++i )
        getBuildQueue[i] = BWAPI::UnitType(o->buildQueue[i]);  //getBuildQueue

      if (_getType.isBuilding())
      {
        if (o->orderID == BW::OrderID::ZergBirth          ||
            o->orderID == BW::OrderID::ZergBuildingMorph  ||
            o->orderID == BW::OrderID::ZergUnitMorph      ||
            o->orderID == BW::OrderID::ZergBuildSelf)
        {
          //if we have a morphing building, set unit type to the build type (what it is morphing to)
          if ( getBuildQueue[(getBuildQueueSlot % 5)] != UnitTypes::None )
            _getType = getBuildQueue[(getBuildQueueSlot % 5)];
        }
      }

      //------------------------------------------------------------------------------------------------------
      //_getTransport
      _getTransport = NULL;
      if (_getType == UnitTypes::Protoss_Interceptor  ||
          _getType == UnitTypes::Protoss_Scarab       ||
          _getType == UnitTypes::Terran_Vulture_Spider_Mine)
      {
        if (o->interceptor.inHanger == 0 ||
            o->statusFlag(BW::StatusFlags::InTransport | BW::StatusFlags::InBuilding) )
          _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(o->interceptor.parent));
      }
      else if (o->statusFlag(BW::StatusFlags::InTransport | BW::StatusFlags::InBuilding) )
        _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(o->connectedUnit));

      //------------------------------------------------------------------------------------------------------
      //_getPosition
      if ( _getTransport )
      {
        BW::Position transRawPos = ((UnitImpl*)_getTransport)->getOriginalRawData->position;
        _getPosition = Position(transRawPos.x, transRawPos.y);
      }
      else
        _getPosition = Position(o->position.x, o->position.y);

      _getHitPoints = (int)ceil(o->hitPoints / 256.0); //_getHitPoints
      //------------------------------------------------------------------------------------------------------
      //_getResources
      _getResources = 0;
      if ( _getType.isResourceContainer() )
        _getResources = o->building.resource.resourceCount;

      hasEmptyBuildQueue = getBuildQueueSlot < 5 ? (getBuildQueue[getBuildQueueSlot] == UnitTypes::None) : false;  //hasEmptyBuildQueue
      _isCompleted = o->statusFlag(BW::StatusFlags::Completed); //_isCompleted
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
      for ( int i = 0; i < 5; ++i )
        getBuildQueue[i]  = UnitTypes::None;               //getBuildQueue
      hasEmptyBuildQueue  = true;               //hasEmptyBuildQueue
      _isCompleted        = false;              //_isCompleted
    }
  }
  void UnitImpl::updateData()
  {
    BW::Unit *o = getOriginalRawData;
    self->isUnderDarkSwarm = false;
    self->isUnderDWeb      = false;
    if (canAccess())
    {
      self->positionX = _getPosition.x(); //getPosition
      self->positionY = _getPosition.y(); //getPosition
      //------------------------------------------------------------------------------------------------------
      //getAngle
      int d = o->currentDirection1;
      d -= 64;
      if (d < 0)
        d += 256;

      self->angle     = (double)d * 3.14159265358979323846 / 128.0;
      self->velocityX = (double)o->current_speedX / 256.0; //getVelocityX
      self->velocityY = (double)o->current_speedY / 256.0; //getVelocityY
      //------------------------------------------------------------------------------------------------------
      //getGroundWeaponCooldown
      if ( _getType == UnitTypes::Protoss_Reaver || _getType == UnitTypes::Hero_Warbringer )
        self->groundWeaponCooldown = o->mainOrderTimer;
      else if ( o->subUnit )
        self->groundWeaponCooldown = o->subUnit->groundWeaponCooldown;
      else
        self->groundWeaponCooldown = o->groundWeaponCooldown;

      self->airWeaponCooldown = o->subUnit ? o->subUnit->airWeaponCooldown : o->airWeaponCooldown; //getAirWeaponCooldown
      self->spellCooldown = o->spellCooldown;  //getSpellCooldown
      u8 animState = 0;
      if ( o->sprite && o->sprite->mainGraphic )
        animState = o->sprite->mainGraphic->anim;
      self->isAttacking = (animState == BW::Anims::GndAttkRpt  ||  //isAttacking
                           animState == BW::Anims::AirAttkRpt  || 
                           animState == BW::Anims::GndAttkInit ||
                           animState == BW::Anims::AirAttkInit);
      //isAttackFrame
      self->isAttackFrame = false;
      if ( o->sprite && o->sprite->mainGraphic )
      { 
        self->isAttackFrame = startingAttack || (self->isAttacking && AttackAnimationRestFrame[_getType]!=10000 && (o->sprite->mainGraphic->frameSet!=AttackAnimationRestFrame[_getType] || lastFrameSet != AttackAnimationRestFrame[_getType]));
        lastFrameSet = o->sprite->mainGraphic->frameSet;
      }

      self->isBurrowed  = o->statusFlag(BW::StatusFlags::Burrowed);  //isBurrowed
      self->isCloaked   = o->statusFlag(BW::StatusFlags::Cloaked) && !o->statusFlag(BW::StatusFlags::Burrowed); //isCloaked
      self->isCompleted = _isCompleted; //isCompleted
      self->isMoving    = o->movementFlag(BW::MovementFlags::Moving | BW::MovementFlags::Accelerating) ||
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
      self->shields             = _getType.maxShields() > 0 ? (int)ceil(o->shieldPoints/256.0) : 0;  //getShields
      self->energy              = _getType.isSpellcaster()  ? (int)ceil(o->energy/256.0)       : 0;  //getEnergy
      self->resources           = _getResources;                        //getResources
      self->resourceGroup       = _getType.isResourceContainer() ? o->building.resource.resourceGroup : 0; //getResourceGroup
      self->killCount           = o->killCount;        //getKillCount
      self->acidSporeCount      = o->acidSporeCount;   //getAcidSporeCount
      self->defenseMatrixPoints = o->defenseMatrixDamage/256;  //getDefenseMatrixPoints
      self->defenseMatrixTimer  = o->defenseMatrixTimer; //getDefenseMatrixTimer
      self->ensnareTimer        = o->ensnareTimer;     //getEnsnareTimer
      self->irradiateTimer      = o->irradiateTimer;   //getIrradiateTimer
      self->lockdownTimer       = o->lockdownTimer;    //getLockdownTimer
      self->maelstromTimer      = o->maelstromTimer;   //getMaelstromTimer
      self->orderTimer          = o->mainOrderTimer;   //getOrderTimer
      self->plagueTimer         = o->plagueTimer;      //getPlagueTimer
      self->removeTimer         = o->removeTimer;      //getRemoveTimer
      self->stasisTimer         = o->stasisTimer;      //getStasisTimer
      self->stimTimer           = o->stimTimer;        //getStimTimer
      self->order               = o->orderID;          //getOrder
      self->secondaryOrder      = o->secondaryOrderID; //getSecondaryOrder
      self->buildUnit           = o->currentBuildUnit ? BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->currentBuildUnit)) : -1; //getBuildUnit
      //------------------------------------------------------------------------------------------------------
      //isTraining
      if (_getType == UnitTypes::Terran_Nuclear_Silo &&
          o->secondaryOrderID == BW::OrderID::Train)
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
      self->isConstructing =  self->isMorphing                                  ||
                              self->order == BW::OrderID::ConstructingBuilding  ||
                              self->order == BW::OrderID::BuildTerran           ||
                              self->order == BW::OrderID::DroneBuild            ||
                              self->order == BW::OrderID::DroneStartBuild       ||
                              self->order == BW::OrderID::DroneLand             ||
                              self->order == BW::OrderID::BuildProtoss1         ||
                              self->order == BW::OrderID::BuildProtoss2         ||
                              self->order == BW::OrderID::TerranBuildSelf       ||
                              self->order == BW::OrderID::ProtossBuildSelf      ||
                              self->order == BW::OrderID::ZergBuildSelf         ||
                              self->order == BW::OrderID::BuildNydusExit        ||
                              self->order == BW::OrderID::BuildAddon            ||
                              self->secondaryOrder == BW::OrderID::BuildAddon   ||
                              (!self->isCompleted && self->buildUnit != -1);
      //------------------------------------------------------------------------------------------------------
      //isIdle
      if (self->isTraining ||
          self->isConstructing ||
          self->isMorphing ||
          self->order == BW::OrderID::ResearchTech ||
          self->order == BW::OrderID::Upgrade )
        self->isIdle = false;
      else
        self->isIdle = self->order == BW::OrderID::PlayerGuard  ||
                       self->order == BW::OrderID::Guard        ||
                       self->order == BW::OrderID::Stop         ||
                       self->order == BW::OrderID::PickupIdle   ||
                       self->order == BW::OrderID::Nothing      ||
                       self->order == BW::OrderID::Medic        ||
                       self->order == BW::OrderID::Carrier      ||
                       self->order == BW::OrderID::Reaver       ||
                       self->order == BW::OrderID::Critter      ||
                       self->order == BW::OrderID::Neutral      ||
                       self->order == BW::OrderID::TowerGuard   ||
                       self->order == BW::OrderID::Burrowed     ||
                       self->order == BW::OrderID::NukeTrain    ||
                       self->order == BW::OrderID::Larva;
      self->target          = BroodwarImpl.server.getUnitID((Unit*)UnitImpl::BWUnitToBWAPIUnit(o->moveToUnit)); //getTarget
      self->targetPositionX = o->moveToPos.x;  //getTargetPosition
      self->targetPositionY = o->moveToPos.y;  //getTargetPosition
      self->orderTarget     = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->orderTargetUnit));  //getOrderTarget
      //------------------------------------------------------------------------------------------------------
      //getAddon
      self->addon = -1;
      if (_getType.isBuilding())
      {
        UnitImpl* addon = UnitImpl::BWUnitToBWAPIUnit(o->currentBuildUnit);
        if ( addon && addon->isAlive && addon->getOriginalRawData->type().isAddon() )
          self->addon = BroodwarImpl.server.getUnitID(addon);
        else
        {
          addon = UnitImpl::BWUnitToBWAPIUnit(o->building.addon);
          if ( addon && addon->isAlive && addon->getOriginalRawData->type().isAddon() )
            self->addon = BroodwarImpl.server.getUnitID(addon);
        }
      }
      //------------------------------------------------------------------------------------------------------
      //getNydusExit
      self->nydusExit = -1;
      if ( _getType == UnitTypes::Zerg_Nydus_Canal )
      {
        UnitImpl* nydus = UnitImpl::BWUnitToBWAPIUnit(o->building.nydus.exit);
        if ( nydus && nydus->isAlive && nydus->getOriginalRawData->unitType == BW::UnitID::Zerg_NydusCanal )
          self->nydusExit = BroodwarImpl.server.getUnitID(nydus);
      }
      //------------------------------------------------------------------------------------------------------
      //getPowerUp
      self->powerUp = -1;
      UnitImpl* powerUp = UnitImpl::BWUnitToBWAPIUnit(o->worker.powerup);
      if (powerUp && powerUp->isAlive)
        self->powerUp = BroodwarImpl.server.getUnitID(powerUp);

      self->isAccelerating  = o->movementFlag(BW::MovementFlags::Accelerating);  //isAccelerating
      self->isBeingGathered = _getType.isResourceContainer() && (o->building.resource.gatherQueueCount || o->building.resource.nextGatherer);  //isBeingGathered
      self->isBlind         = o->isBlind != 0;   //isBlind
      self->isBraking       = o->movementFlag(BW::MovementFlags::Braking);   //isBraking
      //------------------------------------------------------------------------------------------------------
      //isCarryingGas, isCarryingMinerals
      if (_getType.isWorker())
        self->carryResourceType = o->resourceType;
      else
        self->carryResourceType = 0;

      self->isGathering     = _getType.isWorker() && o->statusFlag(BW::StatusFlags::IsGathering);   //isGatheringMinerals; isGatheringGas
      self->isLifted        = o->statusFlag(BW::StatusFlags::InAir) &&
                              o->type().isBuilding(); //isLifted
      self->isParasited     = o->parasiteFlags != 0; //isParasited
      self->isSelected      = BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) && userSelected; //isSelected
      self->isUnderStorm    = o->isUnderStorm != 0; //isUnderStorm
      self->isUnpowered     = _getType.getRace() == Races::Protoss && _getType.isBuilding() && o->statusFlag(BW::StatusFlags::DoodadStatesThing); //isUnpowered
      self->isStuck         = o->movementState == UM_MoveToLegal;
      self->isInterruptible = !o->statusFlag(BW::StatusFlags::CanNotReceiveOrders); //isInterruptible
      self->isInvincible    = o->statusFlag(BW::StatusFlags::Invincible); //isInvincible
      self->buttonset       = o->currentButtonSet;
      self->lastAttackerPlayer = o->lastAttackingPlayer;
      self->recentlyAttacked = o->lastEventColor == 174 ? o->lastEventTimer != 0 : false;
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
      self->order               = Orders::None;  //getOrder
      self->secondaryOrder      = Orders::None;  //getSecondaryOrder
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
      self->isUnderDarkSwarm    = false;
      self->isUnderDWeb         = false;
      self->isUnpowered         = false;  //isUnpowered
      self->isStuck             = false;  //isStuck
      self->isInterruptible     = false;  //isInterruptible
      self->buttonset           = 228;
      self->lastAttackerPlayer  = -1;
      self->recentlyAttacked    = false;
    }
    if (canAccess())
    {
      self->exists = true;
      self->player = BroodwarImpl.server.getPlayerID(_getPlayer);
      self->type   = _getType;
    }
    else
    {
      self->exists = false;
      self->player = BroodwarImpl.server.getPlayerID((Player*)BroodwarImpl.players[11]);
      self->type   = UnitTypes::Unknown;
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
      self->buildType             = UnitTypes::None;
      self->tech                  = TechTypes::None;
      self->remainingResearchTime = 0;
      self->upgrade               = UpgradeTypes::None;
      self->remainingUpgradeTime  = 0;
      self->remainingBuildTime    = 0;
      self->rallyUnit             = -1;

      //------------------------------------------------------------------------------------------------------
      // getTrainingQueue
      if ( !hasEmptyBuildQueue )
      {
        for(int i = getBuildQueueSlot % 5; getBuildQueue[i] != UnitTypes::None && self->trainingQueueCount < 5; i = (i + 1) % 5)
        {
          self->trainingQueue[self->trainingQueueCount] = getBuildQueue[i];
          self->trainingQueueCount++;
        }
      }
      //------------------------------------------------------------------------------------------------------
      // getRemainingTrainTime
      if ( o->currentBuildUnit )
        self->remainingTrainTime = o->currentBuildUnit->remainingBuildTime;

      //------------------------------------------------------------------------------------------------------
      // Unit Type switch; special cases
      switch ( _getType )
      {
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        self->scarabCount = o->carrier.inHangerCount;
        break;
      case BW::UnitID::Terran_Vulture:
      case BW::UnitID::Terran_Hero_JimRaynorV:
        self->spiderMineCount = o->vulture.spiderMineCount;
        break;
      case BW::UnitID::Terran_NuclearSilo:
        if (o->secondaryOrderID == BW::OrderID::Train)
        {
          self->trainingQueue[0]   = BW::UnitID::Terran_NuclearMissile;
          self->trainingQueueCount = 1;
        }
        self->hasNuke = (o->building.silo.hasNuke != 0);
        break;
      case BW::UnitID::Zerg_Hatchery:
      case BW::UnitID::Zerg_Lair:
      case BW::UnitID::Zerg_Hive:
        if ( !self->isCompleted && self->buildType == BW::UnitID::Zerg_Hatchery )
          self->remainingTrainTime = self->remainingBuildTime;
        else
          self->remainingTrainTime = o->building.larvaTimer * 9 + ((o->orderQueueTimer + 8) % 9);
        break;
      case BW::UnitID::Protoss_Interceptor:
        self->carrier = BroodwarImpl.server.getUnitID((Unit*)(UnitImpl::BWUnitToBWAPIUnit(o->interceptor.parent)));
        break;
      case BW::UnitID::Zerg_Larva:
        self->hatchery = BroodwarImpl.server.getUnitID((Unit*)(UnitImpl::BWUnitToBWAPIUnit(o->connectedUnit)));
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
            self->buildType = ((UnitImpl*)getBuildUnit())->getOriginalRawData->unitType;
          break;
        case BW::OrderID::ZergBuildSelf:
          {
            UnitType type = getBuildQueue[getBuildQueueSlot % 5];
            self->buildType = type == UnitTypes::None ? self->type : type;
          }
          break;
        case BW::OrderID::BuildTerran:
        case BW::OrderID::BuildProtoss1:
        case BW::OrderID::ZergUnitMorph:
        case BW::OrderID::ZergBuildingMorph:
        case BW::OrderID::DroneLand:
          self->buildType = getBuildQueue[(getBuildQueueSlot % 5)];
          break;
        case BW::OrderID::ResearchTech:
          self->tech = o->building.techType;
          self->remainingResearchTime = o->building.upgradeResearchTime;
          break;
        case BW::OrderID::Upgrade:
          self->upgrade = o->building.upgradeType;
          self->remainingUpgradeTime = o->building.upgradeResearchTime;
          break;

      }

      //getBuildType
      if ( !hasEmptyBuildQueue &&
           !self->isIdle       &&
           self->secondaryOrder == BW::OrderID::BuildAddon )
        self->buildType = getBuildQueue[(getBuildQueueSlot % 5)];

      //------------------------------------------------------------------------------------------------------
      //getRemainingBuildTime
      if ( !self->isMorphing || self->buildType != UnitTypes::None )
        self->remainingBuildTime = o->remainingBuildTime;
      //------------------------------------------------------------------------------------------------------
      //getRallyPosition
      if (this->_getType.canProduce())
      {
        self->rallyPositionX = o->rally.x;
        self->rallyPositionY = o->rally.y;
      }
      else
      {
        self->rallyPositionX = Positions::None.x();
        self->rallyPositionY = Positions::None.y();
      }
      //------------------------------------------------------------------------------------------------------
      //getRallyUnit
      if ( this->_getType.canProduce() )
        self->rallyUnit = BroodwarImpl.server.getUnitID((Unit*)UnitImpl::BWUnitToBWAPIUnit(o->rally.unit));

      self->transport       = BroodwarImpl.server.getUnitID(_getTransport);   //getTransport
      self->isHallucination = o->statusFlag(BW::StatusFlags::IsHallucination);  //isHallucination
    }
    else
    {
      self->scarabCount           = 0;                    //getScarabCount
      self->spiderMineCount       = 0;                    //getSpiderMineCount
      self->buildType             = UnitTypes::None;     //getBuildType
      self->trainingQueueCount    = 0;                    //getTrainingQueue
      self->tech                  = TechTypes::None;     //getTech
      self->upgrade               = UpgradeTypes::None;  //getUpgrade
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
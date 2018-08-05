#include "UnitImpl.h"

#include <cmath>
#include <limits>
#include <sstream>

#include <Util/Convenience.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include <BWAPI/GameImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/WeaponType.h>

#include <BW/CUnit.h>
#include <BW/CSprite.h>
#include <BW/CImage.h>
#include <BW/UnitTarget.h>
#include <BW/UnitStatusFlags.h>
#include <BW/MovementFlags.h>
#include <BW/Path.h>
#include "Server.h"
#include "BWtoBWAPI.h"

#include "../../../Debug.h"

namespace BWAPI
{
  // Just hardcode some values that encompass the majority of the scanner graphic (as a hack for now)
  // Note that scanner sweeps are tricky, since the scanning graphic isn't associated with the scanner unit
  bool isScannerVisible(BW::Position position)
  {
    int left = (position.x - 64) / 32;
    int top = (position.y - 64) / 32;
    int right = (position.x + 64) / 32;
    int bottom = (position.y + 64) / 32;
    for (int x = left; x <= right; ++x)
    {
      for (int y = top; y <= bottom; ++y)
      {
        if (BroodwarImpl.isVisible(x, y))
          return true;
      }
    }
    return false;
  }

  void UnitImpl::updateInternalData()
  {
    BW::CUnit *o = getOriginalRawData;
    if ( !o )
      return;
    int selfPlayerID = BroodwarImpl.server.getPlayerID(BroodwarImpl.self());
    data.replayID   = BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) ? BW::UnitTarget(o).getTarget() : 0;
    if (isAlive)
    {
      _getPlayer = BroodwarImpl._getPlayer(getOriginalRawData->playerID); //_getPlayer
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for ( int i = 0; i < 9; ++i )
      {
        if ( i == selfPlayerID )
          continue;
        PlayerImpl* player = static_cast<PlayerImpl*>(BroodwarImpl.getPlayer(i));
        if ( !o->sprite || !player )
          data.isVisible[i] = false;
        else if (!BroodwarImpl.isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          data.isVisible[i] = false;
        else if ( _getPlayer == player )
          data.isVisible[i] = true;
        else if ( player->isNeutral() )
          data.isVisible[i] = o->sprite->visibilityFlags > 0;
        else
          data.isVisible[i] = (o->sprite->visibilityFlags & (1 << player->getIndex())) != 0;
      }
      if (selfPlayerID >= 0)
      {
        if ( !o->sprite )
        {
          data.isVisible[selfPlayerID] = false;
          data.isDetected              = false;
        }
        else if (_getPlayer == BWAPI::BroodwarImpl.self())
        {
          data.isVisible[selfPlayerID] = true;
          data.isDetected              = true;
        }
        else if (o->unitType == UnitTypes::Spell_Scanner_Sweep)
        {
          data.isVisible[selfPlayerID] = isScannerVisible(o->position);
          data.isDetected = true;
        }
        else
        {
          data.isVisible[selfPlayerID] = (o->sprite->visibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0;
          if (o->statusFlag(BW::StatusFlags::RequiresDetection))
          {
            data.isVisible[selfPlayerID] &= ((o->visibilityStatus == -1) ||
                                             ((o->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0) ||
                                               o->movementFlag(BW::MovementFlags::Moving | BW::MovementFlags::Accelerating) ||
                                               o->orderID == Orders::Move ||
                                               o->groundWeaponCooldown > 0 ||
                                               o->airWeaponCooldown > 0 ||
                                              !o->statusFlag(BW::StatusFlags::Burrowed) );
          }
          bool canDetect = !o->statusFlag(BW::StatusFlags::RequiresDetection) ||
                           o->visibilityStatus == -1 ||
                           ((o->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0);
          data.isDetected = data.isVisible[selfPlayerID] & canDetect;
        }
      }
      else
      {
        data.isDetected = false;
        for(unsigned int i = 0; i < 9; ++i)
        {
          if (data.isVisible[i])
          {
            data.isDetected = true;
            break;
          }
        }
      }
      //------------------------------------------------------------------------------------------------------
      //_getType
      _getType = UnitType(o->unitType);
      if ( _getType.isMineralField() )
        _getType = UnitTypes::Resource_Mineral_Field;

      getBuildQueueSlot = o->buildQueueSlot; //getBuildQueueSlot
      for ( unsigned int i = 0; i < 5; ++i )
        getBuildQueue[i] = BWAPI::UnitType(o->buildQueue[i]);  //getBuildQueue

      if (_getType.isBuilding())
      {
        if (o->orderID == Orders::ZergBirth          ||
            o->orderID == Orders::ZergBuildingMorph  ||
            o->orderID == Orders::ZergUnitMorph      ||
            o->orderID == Orders::Enum::IncompleteMorphing )
        {
          //if we have a morphing building, set unit type to the build type (what it is morphing to)
          if ( getBuildQueue[(getBuildQueueSlot % 5)] != UnitTypes::None )
            _getType = getBuildQueue[(getBuildQueueSlot % 5)];
        }
      }

      //------------------------------------------------------------------------------------------------------
      //_getTransport
      _getTransport = nullptr;
      if (_getType == UnitTypes::Protoss_Interceptor  ||
          _getType == UnitTypes::Protoss_Scarab       ||
          _getType == UnitTypes::Terran_Vulture_Spider_Mine)
      {
        if (o->fighter.inHanger == false ||
            o->statusFlag(BW::StatusFlags::InTransport | BW::StatusFlags::InBuilding) )
          _getTransport = UnitImpl::BWUnitToBWAPIUnit(o->fighter.parent);
      }
      else if (o->statusFlag(BW::StatusFlags::InTransport | BW::StatusFlags::InBuilding) )
        _getTransport = UnitImpl::BWUnitToBWAPIUnit(o->connectedUnit);

      //------------------------------------------------------------------------------------------------------
      //_getPosition
      _getPosition = BWAPI::Position(_getTransport ? static_cast<UnitImpl*>(_getTransport)->getOriginalRawData->position : o->position);

      _getHitPoints = (int)std::ceil(o->hitPoints / 256.0); //_getHitPoints
      //------------------------------------------------------------------------------------------------------
      //_getResources
      _getResources = 0;
      if ( _getType.isResourceContainer() )
        _getResources = o->resource.resourceCount;

      hasEmptyBuildQueue = getBuildQueueSlot < 5 ? (getBuildQueue[getBuildQueueSlot] == UnitTypes::None) : false;  //hasEmptyBuildQueue
      _isCompleted = o->statusFlag(BW::StatusFlags::Completed); //_isCompleted
    }
    else // not alive
    {
      //------------------------------------------------------------------------------------------------------
      //isVisible
      MemZero(data.isVisible);
      data.isDetected = false;

      _getPlayer          = nullptr;               //_getPlayer
      _getType            = UnitTypes::Unknown; //_getType
      _getTransport       = nullptr;               //_getTransport
      _getPosition        = Positions::Unknown; //_getPosition
      _getHitPoints       = 0;                  //_getHitPoints
      _getResources       = 0;                  //_getResources
      getBuildQueueSlot   = 0;                  //getBuildQueueSlot
      for ( unsigned int i = 0; i < 5; ++i )
        getBuildQueue[i]  = UnitTypes::None;               //getBuildQueue
      hasEmptyBuildQueue  = true;               //hasEmptyBuildQueue
      _isCompleted        = false;              //_isCompleted
    }
  }
  /// @todo TODO Refactor this entirely
  void UnitImpl::updateData()
  {
    BW::CUnit *o = getOriginalRawData;
    data.isUnderDarkSwarm = false;
    data.isUnderDWeb      = false;
    if (canAccess())
    {
      data.positionX = _getPosition.x; //getPosition
      data.positionY = _getPosition.y; //getPosition
      //------------------------------------------------------------------------------------------------------
      //getAngle
      int d = o->currentDirection1;
      d -= 64;
      if (d < 0)
        d += 256;

      data.angle     = (double)d * 3.14159265358979323846 / 128.0;
      data.velocityX = (double)o->current_speed.x / 256.0; //getVelocityX
      data.velocityY = (double)o->current_speed.y / 256.0; //getVelocityY
      //------------------------------------------------------------------------------------------------------
      data.groundWeaponCooldown = o->getGroundWeaponCooldown(); //getGroundWeaponCooldown
      data.airWeaponCooldown = o->getAirWeaponCooldown(); //getAirWeaponCooldown
      data.spellCooldown = o->spellCooldown;  //getSpellCooldown

      data.isAttacking = o->isAttacking();
      
      // startingAttack
      int airWeaponCooldown = o->getAirWeaponCooldown();
      int groundWeaponCooldown = o->getGroundWeaponCooldown();
      bool startingAttack = (airWeaponCooldown > lastAirWeaponCooldown || groundWeaponCooldown > lastGroundWeaponCooldown) && o->isAttacking();
      lastAirWeaponCooldown = airWeaponCooldown;
      lastGroundWeaponCooldown = groundWeaponCooldown;

      data.isStartingAttack = startingAttack;  //isStartingAttack

      //isAttackFrame
      data.isAttackFrame = false;
      const BW::CUnit* damageDealer = o->getDamageDealer();
      if (damageDealer->sprite && damageDealer->sprite->pImagePrimary)
      { 
        int restFrame = _getType.isValid() ? AttackAnimationRestFrame[_getType] : -1;
        data.isAttackFrame = startingAttack || 
                             (data.isAttacking && 
                              restFrame != -1 && 
                              (damageDealer->sprite->pImagePrimary->frameSet != restFrame ||
                              lastFrameSet != restFrame) );
        lastFrameSet = damageDealer->sprite->pImagePrimary->frameSet;
      }

      data.isBurrowed  = o->statusFlag(BW::StatusFlags::Burrowed);  //isBurrowed
      data.isCloaked   = o->statusFlag(BW::StatusFlags::Cloaked) && !o->statusFlag(BW::StatusFlags::Burrowed); //isCloaked
      data.isCompleted = _isCompleted; //isCompleted
      data.isMoving    = o->movementFlag(BW::MovementFlags::Moving | BW::MovementFlags::Accelerating) ||
                          data.order == Orders::Move; //isMoving
    }
    else
    {
      data.positionX             = BWAPI::Positions::Unknown.x;  //getPosition
      data.positionY             = BWAPI::Positions::Unknown.y;  //getPosition
      data.angle                 = 0;      //getAngle
      data.velocityX             = 0;      //getVelocityX
      data.velocityY             = 0;      //getVelocityY
      data.groundWeaponCooldown  = 0;      //getGroundWeaponCooldown
      data.airWeaponCooldown     = 0;      //getAirWeaponCooldown
      data.spellCooldown         = 0;      //getSpellCooldown
      data.isAttacking           = false;  //isAttacking
      data.isBurrowed            = false;  //isBurrowed
      data.isCloaked             = false;  //isCloaked
      data.isCompleted           = false;  //isCompleted
      data.isMoving              = false;  //isMoving
      data.isStartingAttack      = false;  //isStartingAttac
    }

    data.scarabCount = 0;
    data.interceptorCount = 0;
    data.spiderMineCount = 0;
    data.carrier = -1;
    data.hatchery = -1;
    if (canAccessDetected())
    {
      data.lastHitPoints       = wasAccessible ? data.hitPoints : _getHitPoints;  //getHitPoints
      data.hitPoints           = _getHitPoints;  //getHitPoints
      data.shields             = _getType.maxShields() > 0 ? (int)std::ceil(o->shieldPoints/256.0) : 0;  //getShields
      data.energy              = _getType.isSpellcaster()  ? (int)std::ceil(o->energy/256.0)       : 0;  //getEnergy
      data.resources           = _getResources;                        //getResources
      data.resourceGroup       = _getType.isResourceContainer() ? o->resource.resourceGroup : 0; //getResourceGroup
      data.killCount           = o->killCount;        //getKillCount
      data.acidSporeCount      = o->status.acidSporeCount;   //getAcidSporeCount
      data.defenseMatrixPoints = o->status.defenseMatrixDamage/256;  //getDefenseMatrixPoints
      data.defenseMatrixTimer  = o->status.defenseMatrixTimer; //getDefenseMatrixTimer
      data.ensnareTimer        = o->status.ensnareTimer;     //getEnsnareTimer
      data.irradiateTimer      = o->status.irradiateTimer;   //getIrradiateTimer
      data.lockdownTimer       = o->status.lockdownTimer;    //getLockdownTimer
      data.maelstromTimer      = o->status.maelstromTimer;   //getMaelstromTimer
      data.orderTimer          = o->mainOrderTimer;   //getOrderTimer
      data.plagueTimer         = o->status.plagueTimer;      //getPlagueTimer
      data.removeTimer         = o->status.removeTimer;      //getRemoveTimer
      data.stasisTimer         = o->status.stasisTimer;      //getStasisTimer
      data.stimTimer           = o->status.stimTimer;        //getStimTimer
      data.order               = o->orderID;          //getOrder
      data.secondaryOrder      = o->secondaryOrderID; //getSecondaryOrder
      data.buildUnit           = o->currentBuildUnit ? BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->currentBuildUnit)) : -1; //getBuildUnit
      //------------------------------------------------------------------------------------------------------
      //isTraining
      if (_getType == UnitTypes::Terran_Nuclear_Silo &&
          o->secondaryOrderID == Orders::Train)
        data.isTraining = true;
      else if (!_getType.canProduce())
        data.isTraining = false;
      else if (_getType.getRace() == Races::Zerg && _getType.isResourceDepot())
        data.isTraining = false;
      else
        data.isTraining = !hasEmptyBuildQueue;
      //------------------------------------------------------------------------------------------------------
      //isMorphing
      data.isMorphing = data.order == Orders::ZergBirth ||
                         data.order == Orders::ZergBuildingMorph ||
                         data.order == Orders::ZergUnitMorph ||
                         data.order == Orders::Enum::IncompleteMorphing;

      if (data.isCompleted && data.isMorphing)
      {
        data.isCompleted = false;
        _isCompleted      = false;
      }
      //------------------------------------------------------------------------------------------------------
      //isConstructing
      data.isConstructing =  data.isMorphing                                    ||
                              data.order == Orders::ConstructingBuilding         ||
                              data.order == Orders::PlaceBuilding                ||
                              data.order == Orders::Enum::DroneBuild             ||
                              data.order == Orders::Enum::DroneStartBuild        ||
                              data.order == Orders::Enum::DroneLand              ||
                              data.order == Orders::Enum::PlaceProtossBuilding   ||
                              data.order == Orders::Enum::CreateProtossBuilding  ||
                              data.order == Orders::Enum::IncompleteBuilding     ||
                              data.order == Orders::Enum::IncompleteWarping      ||
                              data.order == Orders::Enum::IncompleteMorphing     ||
                              data.order == Orders::BuildNydusExit               ||
                              data.order == Orders::BuildAddon                   ||
                              data.secondaryOrder == Orders::BuildAddon          ||
                              (!data.isCompleted && data.buildUnit != -1);
      //------------------------------------------------------------------------------------------------------
      //isIdle
      if (data.isTraining ||
          data.isConstructing ||
          data.isMorphing ||
          data.order == Orders::ResearchTech ||
          data.order == Orders::Upgrade )
        data.isIdle = false;
      else
        data.isIdle = data.order == Orders::PlayerGuard  ||
                       data.order == Orders::Guard        ||
                       data.order == Orders::Stop         ||
                       data.order == Orders::PickupIdle   ||
                       data.order == Orders::Nothing      ||
                       data.order == Orders::Medic        ||
                       data.order == Orders::Carrier      ||
                       data.order == Orders::Reaver       ||
                       data.order == Orders::Critter      ||
                       data.order == Orders::Neutral      ||
                       data.order == Orders::TowerGuard   ||
                       data.order == Orders::Burrowed     ||
                       data.order == Orders::NukeTrain    ||
                       data.order == Orders::Larva;
      data.target               = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->moveTarget.pUnit)); //getTarget
      data.targetPositionX      = o->moveTarget.pt.x;  //getTargetPosition
      data.targetPositionY      = o->moveTarget.pt.y;  //getTargetPosition
      data.orderTargetPositionX = o->orderTarget.pt.x;
      data.orderTargetPositionY = o->orderTarget.pt.y;
      data.orderTarget          = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->orderTarget.pUnit));  //getOrderTarget
      //------------------------------------------------------------------------------------------------------
      //getAddon
      data.addon = -1;
      if (_getType.isBuilding())
      {
        UnitImpl* addon = UnitImpl::BWUnitToBWAPIUnit(o->currentBuildUnit);
        if ( addon && addon->isAlive && addon->getOriginalRawData->type().isAddon() )
          data.addon = BroodwarImpl.server.getUnitID(addon);
        else
        {
          addon = UnitImpl::BWUnitToBWAPIUnit(o->building.addon);
          if ( addon && addon->isAlive && addon->getOriginalRawData->type().isAddon() )
            data.addon = BroodwarImpl.server.getUnitID(addon);
        }
      }
      //------------------------------------------------------------------------------------------------------
      //getNydusExit
      data.nydusExit = -1;
      if ( _getType == UnitTypes::Zerg_Nydus_Canal )
      {
        UnitImpl* nydus = UnitImpl::BWUnitToBWAPIUnit(o->nydus.exit);
        if ( nydus && nydus->isAlive && nydus->getOriginalRawData->unitType == UnitTypes::Zerg_Nydus_Canal )
          data.nydusExit = BroodwarImpl.server.getUnitID(nydus);
      }
      //------------------------------------------------------------------------------------------------------
      //getPowerUp
      data.powerUp = -1;
      UnitImpl* powerUp = UnitImpl::BWUnitToBWAPIUnit(o->worker.pPowerup);
      if (powerUp && powerUp->isAlive)
        data.powerUp = BroodwarImpl.server.getUnitID(powerUp);

      data.isAccelerating  = o->movementFlag(BW::MovementFlags::Accelerating);  //isAccelerating
      data.isBeingGathered = _getType.isResourceContainer() && (o->resource.gatherQueueCount || o->resource.nextGatherer);  //isBeingGathered
      data.isBlind         = o->status.isBlind != 0;   //isBlind
      data.isBraking       = o->movementFlag(BW::MovementFlags::Braking);   //isBraking
      //------------------------------------------------------------------------------------------------------
      //isCarryingGas, isCarryingMinerals
      data.carryResourceType = _getType.isWorker() ? o->resourceType : 0;

      data.isGathering     = _getType.isWorker() && o->statusFlag(BW::StatusFlags::IsGathering);   //isGatheringMinerals; isGatheringGas
      data.isLifted        = o->statusFlag(BW::StatusFlags::InAir) &&
                              o->type().isBuilding(); //isLifted
      data.isParasited     = o->status.parasiteFlags != 0; //isParasited
      data.isSelected      = BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) && userSelected; //isSelected
      data.isUnderStorm    = o->status.stormTimer != 0; //isUnderStorm
      data.isPowered       = !(_getType.getRace() == Races::Protoss && _getType.isBuilding() && o->statusFlag(BW::StatusFlags::DoodadStatesThing)); // !isUnpowered
      data.isStuck         = o->movementState == BW::UM_MoveToLegal;
      data.isInterruptible = !o->statusFlag(BW::StatusFlags::CanNotReceiveOrders); //isInterruptible
      data.isInvincible    = o->statusFlag(BW::StatusFlags::Invincible); //isInvincible
      data.buttonset       = o->currentButtonSet;
      data.lastAttackerPlayer = o->lastAttackingPlayer;
      data.recentlyAttacked = o->lastEventColor == 174 ? o->lastEventTimer != 0 : false;

      switch (_getType)
      {
      case UnitTypes::Enum::Protoss_Reaver:
      case UnitTypes::Enum::Hero_Warbringer:
        data.scarabCount = o->carrier.inHangerCount;
        break;
      case UnitTypes::Enum::Terran_Vulture:
      case UnitTypes::Enum::Hero_Jim_Raynor_Vulture:
        data.spiderMineCount = o->vulture.spiderMineCount;
        break;
      case UnitTypes::Enum::Protoss_Carrier:
      case UnitTypes::Enum::Hero_Gantrithor:
        data.interceptorCount = o->carrier.inHangerCount + o->carrier.outHangerCount;
        break;
      case UnitTypes::Enum::Protoss_Interceptor:
        data.carrier = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->fighter.parent));
        break;
      case UnitTypes::Enum::Zerg_Larva:
        data.hatchery = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->connectedUnit));
        break;
      default:
        break;
      }
    }
    else
    {
      data.lastHitPoints       = 0;      //getHitPoints
      data.hitPoints           = 0;      //getHitPoints
      data.shields             = 0;      //getShields
      data.energy              = 0;      //getEnergy
      //data.resources           = 0;      //getResources
      data.resourceGroup       = 0;      //getResourceGroup
      data.killCount           = 0;      //getKillCount
      data.defenseMatrixPoints = 0;      //getDefenseMatrixPoints
      data.defenseMatrixTimer  = 0;      //getDefenseMatrixTimer
      data.ensnareTimer        = 0;      //getEnsnareTimer
      data.irradiateTimer      = 0;      //getIrradiateTimer
      data.lockdownTimer       = 0;      //getLockdownTimer
      data.maelstromTimer      = 0;      //getMaelstromTimer
      data.orderTimer          = 0;      //getOrderTimer
      data.plagueTimer         = 0;      //getPlagueTimer
      data.removeTimer         = 0;      //getRemoveTimer
      data.stasisTimer         = 0;      //getStasisTimer
      data.stimTimer           = 0;      //getStimTimer
      data.order               = Orders::Unknown;  //getOrder
      data.secondaryOrder      = Orders::Unknown;  //getSecondaryOrder
      data.buildUnit           = -1;     //getBuildUnit
      data.isTraining          = false;  //isTraining
      data.isMorphing          = false;  //isMorphing
      data.isConstructing      = false;  //isConstructing
      data.isIdle              = false;  //isIdle
      data.target              = -1;     //getTarget
      data.targetPositionX     = Positions::Unknown.x; //getTargetPosition
      data.targetPositionY     = Positions::Unknown.y; //getTargetPosition
      data.orderTarget         = -1;     //getOrderTarget
      data.orderTargetPositionX = Positions::Unknown.x;
      data.orderTargetPositionY = Positions::Unknown.y;
      data.addon               = -1;     //getAddon
      data.nydusExit           = -1;     //getNydusExit
      data.powerUp             = -1;     //getPowerUp
      data.isAccelerating      = false;  //isAccelerating
      data.isBeingGathered     = false;  //isBeingGathered
      data.isBlind             = false;  //isBlind
      data.isBraking           = false;  //isBraking
      data.carryResourceType   = 0;      //isCarryingMinerals;isCarryingGas
      data.isLifted            = false;  //isLifted
      data.isParasited         = false;  //isParasited
      data.isSelected          = false;  //isSelected
      data.isUnderStorm        = false;  //isUnderStorm
      data.isUnderDarkSwarm    = false;
      data.isUnderDWeb         = false;
      data.isPowered            = true;   //!isUnpowered
      data.isStuck             = false;  //isStuck
      data.isInterruptible     = false;  //isInterruptible
      data.buttonset           = UnitTypes::None;
      data.lastAttackerPlayer  = -1;
      data.recentlyAttacked    = false;
    }
    if (canAccess())
    {
      data.exists = true;
      data.player = BroodwarImpl.server.getPlayerID(_getPlayer);
      data.type   = _getType;
    }
    else
    {
      data.exists = false;
      data.player = BroodwarImpl.server.getPlayerID(BroodwarImpl._getPlayer(11));
      data.type   = UnitTypes::Unknown;
    }
    if (canAccessInside())
    {
      // Default assignments
      data.trainingQueueCount    = 0;
      data.remainingTrainTime    = 0;
      data.hasNuke               = false;
      data.buildType             = UnitTypes::None;
      data.tech                  = TechTypes::None;
      data.remainingResearchTime = 0;
      data.upgrade               = UpgradeTypes::None;
      data.remainingUpgradeTime  = 0;
      data.remainingBuildTime    = 0;
      data.rallyUnit             = -1;

      //------------------------------------------------------------------------------------------------------
      // getTrainingQueue
      if ( !hasEmptyBuildQueue )
      {
        for(int i = getBuildQueueSlot % 5; getBuildQueue[i] != UnitTypes::None && data.trainingQueueCount < 5; i = (i + 1) % 5)
        {
          data.trainingQueue[data.trainingQueueCount] = getBuildQueue[i];
          data.trainingQueueCount++;
        }
      }
      //------------------------------------------------------------------------------------------------------
      // getRemainingTrainTime
      if ( o->currentBuildUnit )
        data.remainingTrainTime = o->currentBuildUnit->remainingBuildTime;

      //------------------------------------------------------------------------------------------------------
      // Unit Type switch; special cases
      switch ( _getType )
      {
      case UnitTypes::Enum::Terran_Nuclear_Silo:
        if (o->secondaryOrderID == Orders::Train)
        {
          data.trainingQueue[0]   = UnitTypes::Enum::Terran_Nuclear_Missile;
          data.trainingQueueCount = 1;
        }
        data.hasNuke = (o->silo.bReady != 0);
        break;
      case UnitTypes::Enum::Zerg_Hatchery:
      case UnitTypes::Enum::Zerg_Lair:
      case UnitTypes::Enum::Zerg_Hive:
        if ( !data.isCompleted && data.buildType == UnitTypes::Enum::Zerg_Hatchery )
          data.remainingTrainTime = data.remainingBuildTime;
        else
          data.remainingTrainTime = o->building.larvaTimer * 9 + ((o->orderQueueTimer + 8) % 9);
        break;
      default:
        break;
      }

      //------------------------------------------------------------------------------------------------------
      // Order Type switch; special cases
      switch ( data.order )
      {
        case Orders::Enum::IncompleteBuilding:
        case Orders::Enum::IncompleteWarping:
          data.buildType = data.type;
          break;
        case Orders::Enum::ConstructingBuilding:
          if ( data.buildUnit != -1 )
            data.buildType = static_cast<UnitImpl*>(getBuildUnit())->getOriginalRawData->unitType;
          break;
        case Orders::Enum::IncompleteMorphing:
          {
            UnitType type = getBuildQueue[getBuildQueueSlot % 5];
            data.buildType = type == UnitTypes::None ? data.type : type;
          }
          break;
        case Orders::Enum::PlaceBuilding:
        case Orders::Enum::PlaceProtossBuilding:
        case Orders::Enum::ZergUnitMorph:
        case Orders::Enum::ZergBuildingMorph:
        case Orders::Enum::DroneLand:
          data.buildType = getBuildQueue[(getBuildQueueSlot % 5)];
          break;
        case Orders::Enum::ResearchTech:
          data.tech = o->building.techType;
          data.remainingResearchTime = o->building.upgradeResearchTime;
          break;
        case Orders::Enum::Upgrade:
          data.upgrade = o->building.upgradeType;
          data.remainingUpgradeTime = o->building.upgradeResearchTime;
          break;
      }

      //getBuildType
      if ( !hasEmptyBuildQueue &&
           !data.isIdle       &&
           data.secondaryOrder == Orders::BuildAddon )
        data.buildType = getBuildQueue[(getBuildQueueSlot % 5)];

      //------------------------------------------------------------------------------------------------------
      //getRemainingBuildTime
      if ( !data.isCompleted && (!data.isMorphing || data.buildType != UnitTypes::None) )
        data.remainingBuildTime = o->remainingBuildTime;
      //------------------------------------------------------------------------------------------------------
      //getRallyPosition
      if (this->_getType.canProduce())
      {
        data.rallyPositionX = o->rally.position.x;
        data.rallyPositionY = o->rally.position.y;
      }
      else
      {
        data.rallyPositionX = Positions::None.x;
        data.rallyPositionY = Positions::None.y;
      }
      //------------------------------------------------------------------------------------------------------
      //getRallyUnit
      if ( this->_getType.canProduce() )
        data.rallyUnit = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->rally.unit));

      data.transport       = BroodwarImpl.server.getUnitID(_getTransport);   //getTransport
      data.isHallucination = o->statusFlag(BW::StatusFlags::IsHallucination);  //isHallucination
    }
    else
    {
      data.buildType             = UnitTypes::None;     //getBuildType
      data.trainingQueueCount    = 0;                    //getTrainingQueue
      data.tech                  = TechTypes::None;     //getTech
      data.upgrade               = UpgradeTypes::None;  //getUpgrade
      data.remainingBuildTime    = 0;                    //getRemainingBuildTime
      data.remainingTrainTime    = 0;                    //getRemainingTrainTime
      data.remainingResearchTime = 0;                    //getRemainingResearchTime
      data.remainingUpgradeTime  = 0;                    //getRemainingUpgradeTime
      data.rallyPositionX        = Positions::None.x;  //getRallyPosition
      data.rallyPositionY        = Positions::None.y;  //getRallyPosition
      data.rallyUnit             = -1;                   //getRallyUnit
      data.transport             = -1;                   //getTransport
      data.hasNuke               = false;                //hasNuke
      data.isHallucination       = false;                //isHallucination
    }
    if ( data.order >= 0 && data.order < Orders::Enum::MAX )
      data.order = BWtoBWAPI_Order[data.order];
    if ( data.secondaryOrder >= 0 && data.secondaryOrder < Orders::Enum::MAX )
      data.secondaryOrder = BWtoBWAPI_Order[data.secondaryOrder];
  }
}
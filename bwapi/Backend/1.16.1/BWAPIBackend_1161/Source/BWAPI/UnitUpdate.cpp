#include "UnitImpl.h"

#include <cmath>
#include <limits>
#include <sstream>

#include "../Convenience.h"

#include <BWAPI/Order.h>
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BWAPI/WeaponType.h>
#include <BWAPI/Race.h>
#include <BWAPI/Flag.h>

#include "../BW/CUnit.h"
#include "../BW/CSprite.h"
#include "../BW/CImage.h"
#include "../BW/UnitTarget.h"
#include "../BW/UnitStatusFlags.h"
#include "../BW/MovementFlags.h"
#include "../BW/Path.h"
#include "BWtoBWAPI.h"

namespace BWAPI4
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
    self->replayID   = BWAPI4::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) ? BW::UnitTarget(o).getTarget() : 0;
    if (isAlive)
    {
      _getPlayer = BroodwarImpl._getPlayer(getOriginalRawData->playerID); //_getPlayer
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for ( int i = 0; i < 9; ++i )
      {
        if ( i == selfPlayerID )
          continue;
        PlayerImpl* player = BroodwarImpl.getPlayer(i);
        if ( !o->sprite || !player )
          self->isVisible[i] = false;
        else if (!BroodwarImpl.isReplay() && !BWAPI4::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          self->isVisible[i] = false;
        else if ( _getPlayer->getNationId() == player->getNationId() )
          self->isVisible[i] = true;
        else if ( player->isNeutral() )
          self->isVisible[i] = o->sprite->visibilityFlags > 0;
        else
          self->isVisible[i] = (o->sprite->visibilityFlags & (1 << player->getNationId())) != 0;
      }
      if (selfPlayerID >= 0)
      {
        if ( !o->sprite )
        {
          self->isVisible[selfPlayerID] = false;
          self->isDetected              = false;
        }
        else if (_getPlayer->getNationId() == BWAPI4::BroodwarImpl.self()->getNationId())
        {
          self->isVisible[selfPlayerID] = true;
          self->isDetected              = true;
        }
        else if (o->unitType == UnitTypes::Spell_Scanner_Sweep)
        {
          self->isVisible[selfPlayerID] = isScannerVisible(o->position);
          self->isDetected = true;
        }
        else
        {
          self->isVisible[selfPlayerID] = (o->sprite->visibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getNationId())) != 0;
          if (o->statusFlag(BW::StatusFlags::RequiresDetection))
          {
            self->isVisible[selfPlayerID] &= ((o->visibilityStatus == -1) ||
                                             ((o->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getNationId())) != 0) ||
                                               o->movementFlag(BW::MovementFlags::Moving | BW::MovementFlags::Accelerating) ||
                                               o->orderID == Orders::Move ||
                                               o->groundWeaponCooldown > 0 ||
                                               o->airWeaponCooldown > 0 ||
                                              !o->statusFlag(BW::StatusFlags::Burrowed) );
          }
          bool canDetect = !o->statusFlag(BW::StatusFlags::RequiresDetection) ||
                           o->visibilityStatus == -1 ||
                           ((o->visibilityStatus & (1 << BroodwarImpl.BWAPIPlayer->getNationId())) != 0);
          self->isDetected = self->isVisible[selfPlayerID] & canDetect;
        }
      }
      else
      {
        self->isDetected = false;
        for(unsigned int i = 0; i < 9; ++i)
        {
          if (self->isVisible[i])
          {
            self->isDetected = true;
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
      MemZero(self->isVisible);
      self->isDetected = false;

      _getPlayer          = nullptr;            //_getPlayer
      _getType            = UnitTypes::Unknown; //_getType
      _getTransport       = nullptr;            //_getTransport
      _getPosition        = Positions::Unknown; //_getPosition
      _getHitPoints       = 0;                  //_getHitPoints
      _getResources       = 0;                  //_getResources
      getBuildQueueSlot   = 0;                  //getBuildQueueSlot
      for ( unsigned int i = 0; i < 5; ++i )
        getBuildQueue[i]  = UnitTypes::None;    //getBuildQueue
      hasEmptyBuildQueue  = true;               //hasEmptyBuildQueue
      _isCompleted        = false;              //_isCompleted
    }
  }
  /// @todo TODO Refactor this entirely
  void UnitImpl::updateData()
  {
    self->id = id;
    BW::CUnit *o = getOriginalRawData;
    self->isUnderDarkSwarm = false;
    self->isUnderDWeb      = false;
    if (canAccess())
    {
      self->positionX = _getPosition.x; //getPosition
      self->positionY = _getPosition.y; //getPosition
      //------------------------------------------------------------------------------------------------------
      //getAngle
      int d = o->currentDirection1;
      d -= 64;
      if (d < 0)
        d += 256;

      self->angle     = (double)d * 3.14159265358979323846 / 128.0;
      self->velocityX = (double)o->current_speed.x / 256.0; //getVelocityX
      self->velocityY = (double)o->current_speed.y / 256.0; //getVelocityY
      //------------------------------------------------------------------------------------------------------
      self->groundWeaponCooldown = o->getGroundWeaponCooldown(); //getGroundWeaponCooldown
      self->airWeaponCooldown = o->getAirWeaponCooldown(); //getAirWeaponCooldown
      self->spellCooldown = o->spellCooldown;  //getSpellCooldown

      self->isAttacking = o->isAttacking();
      
      // startingAttack
      int airWeaponCooldown = o->getAirWeaponCooldown();
      int groundWeaponCooldown = o->getGroundWeaponCooldown();
      bool startingAttack = (airWeaponCooldown > lastAirWeaponCooldown || groundWeaponCooldown > lastGroundWeaponCooldown) && o->isAttacking();
      lastAirWeaponCooldown = airWeaponCooldown;
      lastGroundWeaponCooldown = groundWeaponCooldown;

      self->isStartingAttack = startingAttack;  //isStartingAttack

      //isAttackFrame
      self->isAttackFrame = false;
      const BW::CUnit* damageDealer = o->getDamageDealer();
      if (damageDealer->sprite && damageDealer->sprite->pImagePrimary)
      { 
        int restFrame = _getType.isValid() ? AttackAnimationRestFrame[_getType] : -1;
        self->isAttackFrame = startingAttack || 
                             (self->isAttacking && 
                              restFrame != -1 && 
                              (damageDealer->sprite->pImagePrimary->frameSet != restFrame ||
                              lastFrameSet != restFrame) );
        lastFrameSet = damageDealer->sprite->pImagePrimary->frameSet;
      }

      self->isBurrowed  = o->statusFlag(BW::StatusFlags::Burrowed);  //isBurrowed
      self->isCloaked   = o->statusFlag(BW::StatusFlags::Cloaked) && !o->statusFlag(BW::StatusFlags::Burrowed); //isCloaked
      self->isCompleted = _isCompleted; //isCompleted
      self->isMoving    = o->movementFlag(BW::MovementFlags::Moving | BW::MovementFlags::Accelerating) ||
                          self->order == Orders::Move; //isMoving
    }
    else
    {
      self->positionX             = BWAPI4::Positions::Unknown.x;  //getPosition
      self->positionY             = BWAPI4::Positions::Unknown.y;  //getPosition
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

    self->scarabCount = 0;
    self->interceptorCount = 0;
    self->spiderMineCount = 0;
    self->carrier = -1;
    self->hatchery = -1;
    if (canAccessDetected())
    {
      self->lastHitPoints       = wasAccessible ? self->hitPoints : _getHitPoints;  //getHitPoints
      self->hitPoints           = _getHitPoints;  //getHitPoints
      self->shields             = _getType.maxShields() > 0 ? (int)std::ceil(o->shieldPoints/256.0) : 0;  //getShields
      self->resourceGroup       = _getType.isResourceContainer() ? o->resource.resourceGroup : 0; //getResourceGroup
      self->killCount           = o->killCount;        //getKillCount
      self->acidSporeCount      = o->status.acidSporeCount;   //getAcidSporeCount
      self->defenseMatrixPoints = o->status.defenseMatrixDamage/256;  //getDefenseMatrixPoints
      self->defenseMatrixTimer  = o->status.defenseMatrixTimer; //getDefenseMatrixTimer
      self->ensnareTimer        = o->status.ensnareTimer;     //getEnsnareTimer
      self->irradiateTimer      = o->status.irradiateTimer;   //getIrradiateTimer
      self->lockdownTimer       = o->status.lockdownTimer;    //getLockdownTimer
      self->maelstromTimer      = o->status.maelstromTimer;   //getMaelstromTimer
      self->orderTimer          = o->mainOrderTimer;   //getOrderTimer
      self->plagueTimer         = o->status.plagueTimer;      //getPlagueTimer
      self->removeTimer         = o->status.removeTimer;      //getRemoveTimer
      self->stasisTimer         = o->status.stasisTimer;      //getStasisTimer
      self->stimTimer           = o->status.stimTimer;        //getStimTimer
      self->order               = o->orderID;          //getOrder
      self->secondaryOrder      = o->secondaryOrderID; //getSecondaryOrder
      self->buildUnit           = o->currentBuildUnit ? BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->currentBuildUnit)) : -1; //getBuildUnit
      //------------------------------------------------------------------------------------------------------
      //isTraining
      if (_getType == UnitTypes::Terran_Nuclear_Silo &&
          o->secondaryOrderID == Orders::Train)
        self->isTraining = true;
      else if (!_getType.canProduce())
        self->isTraining = false;
      else if (_getType.getRace() == Races::Zerg && _getType.isResourceDepot())
        self->isTraining = false;
      else
        self->isTraining = !hasEmptyBuildQueue;
      //------------------------------------------------------------------------------------------------------
      //isMorphing
      self->isMorphing = self->order == Orders::ZergBirth ||
                         self->order == Orders::ZergBuildingMorph ||
                         self->order == Orders::ZergUnitMorph ||
                         self->order == Orders::Enum::IncompleteMorphing;

      if (self->isCompleted && self->isMorphing)
      {
        self->isCompleted = false;
        _isCompleted      = false;
      }
      //------------------------------------------------------------------------------------------------------
      //isConstructing
      self->isConstructing =  self->isMorphing                                    ||
                              self->order == Orders::ConstructingBuilding         ||
                              self->order == Orders::PlaceBuilding                ||
                              self->order == Orders::Enum::DroneBuild             ||
                              self->order == Orders::Enum::DroneStartBuild        ||
                              self->order == Orders::Enum::DroneLand              ||
                              self->order == Orders::Enum::PlaceProtossBuilding   ||
                              self->order == Orders::Enum::CreateProtossBuilding  ||
                              self->order == Orders::Enum::IncompleteBuilding     ||
                              self->order == Orders::Enum::IncompleteWarping      ||
                              self->order == Orders::Enum::IncompleteMorphing     ||
                              self->order == Orders::BuildNydusExit               ||
                              self->order == Orders::BuildAddon                   ||
                              self->secondaryOrder == Orders::BuildAddon          ||
                              (!self->isCompleted && self->buildUnit != -1);
      //------------------------------------------------------------------------------------------------------
      //isIdle
      if (self->isTraining ||
          self->isConstructing ||
          self->isMorphing ||
          self->order == Orders::ResearchTech ||
          self->order == Orders::Upgrade )
        self->isIdle = false;
      else
        self->isIdle = self->order == Orders::PlayerGuard  ||
                       self->order == Orders::Guard        ||
                       self->order == Orders::Stop         ||
                       self->order == Orders::PickupIdle   ||
                       self->order == Orders::Nothing      ||
                       self->order == Orders::Medic        ||
                       self->order == Orders::Carrier      ||
                       self->order == Orders::Reaver       ||
                       self->order == Orders::Critter      ||
                       self->order == Orders::Neutral      ||
                       self->order == Orders::TowerGuard   ||
                       self->order == Orders::Burrowed     ||
                       self->order == Orders::NukeTrain    ||
                       self->order == Orders::Larva;
      self->target               = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->moveTarget.pUnit)); //getTarget
      self->targetPositionX      = o->moveTarget.pt.x;  //getTargetPosition
      self->targetPositionY      = o->moveTarget.pt.y;  //getTargetPosition
      self->orderTargetPositionX = o->orderTarget.pt.x;
      self->orderTargetPositionY = o->orderTarget.pt.y;
      self->orderTarget          = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->orderTarget.pUnit));  //getOrderTarget
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
        UnitImpl* nydus = UnitImpl::BWUnitToBWAPIUnit(o->nydus.exit);
        if ( nydus && nydus->isAlive && nydus->getOriginalRawData->unitType == UnitTypes::Zerg_Nydus_Canal )
          self->nydusExit = BroodwarImpl.server.getUnitID(nydus);
      }
      //------------------------------------------------------------------------------------------------------
      //getPowerUp
      self->powerUp = -1;
      UnitImpl* powerUp = UnitImpl::BWUnitToBWAPIUnit(o->worker.pPowerup);
      if (powerUp && powerUp->isAlive)
        self->powerUp = BroodwarImpl.server.getUnitID(powerUp);

      self->isAccelerating  = o->movementFlag(BW::MovementFlags::Accelerating);  //isAccelerating
      self->isBeingGathered = _getType.isResourceContainer() && (o->resource.gatherQueueCount || o->resource.nextGatherer);  //isBeingGathered
      self->isBeingHealed   = o->isBeingHealed;  //isBeingHealed
      self->isBlind         = o->status.isBlind != 0;   //isBlind
      self->isBraking       = o->movementFlag(BW::MovementFlags::Braking);   //isBraking
      //------------------------------------------------------------------------------------------------------
      //isCarryingGas, isCarryingMinerals
      self->carryResourceType = _getType.isWorker() ? o->resourceType : 0;

      self->isGathering     = _getType.isWorker() && o->statusFlag(BW::StatusFlags::IsGathering);   //isGatheringMinerals; isGatheringGas
      self->isLifted        = o->statusFlag(BW::StatusFlags::InAir) &&
                              o->type().isBuilding(); //isLifted
      self->isParasited     = o->status.parasiteFlags != 0; //isParasited
      self->isSelected      = BWAPI4::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) && userSelected; //isSelected
      self->isUnderStorm    = o->status.stormTimer != 0; //isUnderStorm
      self->isPowered       = !(_getType.getRace() == Races::Protoss && _getType.isBuilding() && o->statusFlag(BW::StatusFlags::DoodadStatesThing)); // !isUnpowered
      self->isStuck         = o->movementState == BW::UM_MoveToLegal;
      self->isInterruptible = !o->statusFlag(BW::StatusFlags::CanNotReceiveOrders); //isInterruptible
      self->isInvincible    = o->statusFlag(BW::StatusFlags::Invincible); //isInvincible
      self->buttonset       = o->currentButtonSet;
      self->recentlyAttacked = o->lastEventColor == 174 ? o->lastEventTimer != 0 : false;

      switch (_getType)
      {
      case UnitTypes::Enum::Protoss_Reaver:
      case UnitTypes::Enum::Hero_Warbringer:
        self->scarabCount = o->carrier.inHangerCount;
        break;
      case UnitTypes::Enum::Terran_Vulture:
      case UnitTypes::Enum::Hero_Jim_Raynor_Vulture:
        self->spiderMineCount = o->vulture.spiderMineCount;
        break;
      case UnitTypes::Enum::Protoss_Carrier:
      case UnitTypes::Enum::Hero_Gantrithor:
        self->interceptorCount = o->carrier.inHangerCount + o->carrier.outHangerCount;
        break;
      case UnitTypes::Enum::Protoss_Interceptor:
        self->carrier = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->fighter.parent));
        break;
      case UnitTypes::Enum::Zerg_Larva:
        self->hatchery = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->connectedUnit));
        break;
      default:
        break;
      }
    }
    else
    {
      self->lastHitPoints       = 0;      //getHitPoints
      self->hitPoints           = 0;      //getHitPoints
      self->shields             = 0;      //getShields
      //self->resources           = 0;      //getResources
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
      self->order               = Orders::Unknown;  //getOrder
      self->secondaryOrder      = Orders::Unknown;  //getSecondaryOrder
      self->buildUnit           = -1;     //getBuildUnit
      self->isTraining          = false;  //isTraining
      self->isMorphing          = false;  //isMorphing
      self->isConstructing      = false;  //isConstructing
      self->isIdle              = false;  //isIdle
      self->target              = -1;     //getTarget
      self->targetPositionX     = Positions::Unknown.x; //getTargetPosition
      self->targetPositionY     = Positions::Unknown.y; //getTargetPosition
      self->orderTarget         = -1;     //getOrderTarget
      self->orderTargetPositionX = Positions::Unknown.x;
      self->orderTargetPositionY = Positions::Unknown.y;
      self->addon               = -1;     //getAddon
      self->nydusExit           = -1;     //getNydusExit
      self->powerUp             = -1;     //getPowerUp
      self->isAccelerating      = false;  //isAccelerating
      self->isBeingGathered     = false;  //isBeingGathered
      self->isBeingHealed       = false;
      self->isBlind             = false;  //isBlind
      self->isBraking           = false;  //isBraking
      self->carryResourceType   = 0;      //isCarryingMinerals;isCarryingGas
      self->isGathering         = false;  //isGathering
      self->isLifted            = false;  //isLifted
      self->isParasited         = false;  //isParasited
      self->isSelected          = false;  //isSelected
      self->isUnderStorm        = false;  //isUnderStorm
      self->isUnderDarkSwarm    = false;
      self->isUnderDWeb         = false;
      self->isPowered            = true;   //!isUnpowered
      self->isStuck             = false;  //isStuck
      self->isInterruptible     = false;  //isInterruptible
      self->isInvincible        = false;  //isInvincible
      self->buttonset           = UnitTypes::None;
      self->recentlyAttacked    = false;
    }
    if (canAccess())
    {
      self->exists = true;
      // Force setting unit owner to self in team melee
      if (BW::BWDATA::g_LocalNationID != BW::BWDATA::g_LocalHumanID &&
        BW::BWDATA::g_LocalNationID == getOriginalRawData->playerID)
      {
        self->player = BroodwarImpl.server.getPlayerID(BroodwarImpl.self());
      }
      else
      {
        self->player = BroodwarImpl.server.getPlayerID(_getPlayer);
      }
      self->type   = _getType;
    }
    else
    {
      self->exists = false;
      self->player = BroodwarImpl.server.getPlayerID(BroodwarImpl._getPlayer(11));
      self->type   = UnitTypes::Unknown;
    }
    if (canAccessInside())
    {
      // Default assignments
      self->trainingQueueCount    = 0;
      self->remainingTrainTime    = 0;
      self->hasNuke               = false;
      self->buildType             = UnitTypes::None;
      self->tech                  = TechTypes::None;
      self->remainingResearchTime = 0;
      self->upgrade               = UpgradeTypes::None;
      self->remainingUpgradeTime  = 0;
      self->remainingBuildTime    = 0;
      self->rallyUnit             = -1;
      
      //------------------------------------------------------------------------------------------------------
      // getResources
      self->resources = _getResources;

      //------------------------------------------------------------------------------------------------------
      // getEnergy
      self->energy = _getType.isSpellcaster() ? (int)std::ceil(o->energy / 256.0) : 0;

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
      case UnitTypes::Enum::Terran_Nuclear_Silo:
        if (o->secondaryOrderID == Orders::Train)
        {
          self->trainingQueue[0]   = UnitTypes::Enum::Terran_Nuclear_Missile;
          self->trainingQueueCount = 1;
        }
        self->hasNuke = (o->silo.bReady != 0);
        break;
      case UnitTypes::Enum::Zerg_Hatchery:
      case UnitTypes::Enum::Zerg_Lair:
      case UnitTypes::Enum::Zerg_Hive:
        if ( !self->isCompleted && self->buildType == UnitTypes::Enum::Zerg_Hatchery )
          self->remainingTrainTime = self->remainingBuildTime;
        else
          self->remainingTrainTime = o->building.larvaTimer * 9 + ((o->orderQueueTimer + 8) % 9);
        break;
      default:
        break;
      }

      //------------------------------------------------------------------------------------------------------
      // Order Type switch; special cases
      switch ( self->order )
      {
        case Orders::Enum::IncompleteBuilding:
        case Orders::Enum::IncompleteWarping:
          self->buildType = self->type;
          break;
        case Orders::Enum::ConstructingBuilding:
          if ( self->buildUnit != -1 )
            self->buildType = static_cast<UnitImpl*>(getBuildUnit())->getOriginalRawData->unitType;
          break;
        case Orders::Enum::IncompleteMorphing:
          {
            UnitType type = getBuildQueue[getBuildQueueSlot % 5];
            self->buildType = type == UnitTypes::None ? self->type : type.getID();
          }
          break;
        case Orders::Enum::PlaceBuilding:
        case Orders::Enum::PlaceProtossBuilding:
        case Orders::Enum::ZergUnitMorph:
        case Orders::Enum::ZergBuildingMorph:
        case Orders::Enum::DroneLand:
          self->buildType = getBuildQueue[(getBuildQueueSlot % 5)];
          break;
        case Orders::Enum::ResearchTech:
          self->tech = o->building.techType;
          self->remainingResearchTime = o->building.upgradeResearchTime;
          break;
        case Orders::Enum::Upgrade:
          self->upgrade = o->building.upgradeType;
          self->remainingUpgradeTime = o->building.upgradeResearchTime;
          break;
      }

      //getBuildType
      if ( !hasEmptyBuildQueue &&
           !self->isIdle       &&
           self->secondaryOrder == Orders::BuildAddon )
        self->buildType = getBuildQueue[(getBuildQueueSlot % 5)];

      //------------------------------------------------------------------------------------------------------
      //getRemainingBuildTime
      if ( !self->isCompleted && (!self->isMorphing || self->buildType != UnitTypes::None) )
        self->remainingBuildTime = o->remainingBuildTime;
      //------------------------------------------------------------------------------------------------------
      //getRallyPosition
      if (this->_getType.canProduce())
      {
        self->rallyPositionX = o->rally.position.x;
        self->rallyPositionY = o->rally.position.y;
      }
      else
      {
        self->rallyPositionX = Positions::None.x;
        self->rallyPositionY = Positions::None.y;
      }
      //------------------------------------------------------------------------------------------------------
      //getRallyUnit
      if ( this->_getType.canProduce() )
        self->rallyUnit = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(o->rally.unit));

      self->transport       = BroodwarImpl.server.getUnitID(_getTransport);   //getTransport
      self->isHallucination = o->statusFlag(BW::StatusFlags::IsHallucination);  //isHallucination
    }
    else
    {
      self->resources = getInitialResources();            //getResources
      self->energy                = 0;                    //getEnergy
      self->buildType             = UnitTypes::None;     //getBuildType
      self->trainingQueueCount    = 0;                    //getTrainingQueue
      self->tech                  = TechTypes::None;     //getTech
      self->upgrade               = UpgradeTypes::None;  //getUpgrade
      self->remainingBuildTime    = 0;                    //getRemainingBuildTime
      self->remainingTrainTime    = 0;                    //getRemainingTrainTime
      self->remainingResearchTime = 0;                    //getRemainingResearchTime
      self->remainingUpgradeTime  = 0;                    //getRemainingUpgradeTime
      self->rallyPositionX        = Positions::None.x;  //getRallyPosition
      self->rallyPositionY        = Positions::None.y;  //getRallyPosition
      self->rallyUnit             = -1;                   //getRallyUnit
      self->transport             = -1;                   //getTransport
      self->hasNuke               = false;                //hasNuke
      self->isHallucination       = false;                //isHallucination
    }
    if ( self->order >= 0 && self->order < Orders::Enum::MAX )
      self->order = BWtoBWAPI_Order[self->order];
    if ( self->secondaryOrder >= 0 && self->secondaryOrder < Orders::Enum::MAX )
      self->secondaryOrder = BWtoBWAPI_Order[self->secondaryOrder];
  }
}

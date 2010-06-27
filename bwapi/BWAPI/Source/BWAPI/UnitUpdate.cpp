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

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>
#include "Server.h"
namespace BWAPI
{
  void UnitImpl::updateUnitData()
  {
    id = (int)this;
    int selfPlayerID = BroodwarImpl.server.getPlayerID(Broodwar->self());
    if (_exists)
    {
      //------------------------------------------------------------------------------------------------------
      //_getPlayer
      _getPlayer = (Player*)BroodwarImpl.players[getRawDataLocal->playerID];
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i=0;i<9;i++)
      {
        if (i==selfPlayerID) continue;
        Player* player = BroodwarImpl.server.getPlayer(i);
        if (getRawDataLocal->sprite == NULL)
          self->isVisible[i]=false;
             //this function is only available when Broodwar is in a replay or the complete map information flag is enabled.
        else if (!BroodwarImpl._isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          self->isVisible[i]=false;
        else if (_getPlayer == player)
          self->isVisible[i]=true;
        else if (player == NULL)
          self->isVisible[i]=false;
        else if (player->getID()==11)
          self->isVisible[i]=getRawDataLocal->sprite->visibilityFlags > 0;
        else
          self->isVisible[i]=(getRawDataLocal->sprite->visibilityFlags & (1 << player->getID())) != 0;
      }
      if (getRawDataLocal->sprite == NULL)
        self->isVisible[selfPlayerID] = false;
      else if (BroodwarImpl._isReplay())
        self->isVisible[selfPlayerID] = getRawDataLocal->sprite->visibilityFlags > 0;
      else if (_getPlayer == BWAPI::BroodwarImpl.self())
        self->isVisible[selfPlayerID] = true;
      else if (makeVisible)
        self->isVisible[selfPlayerID] = true;
      else
        self->isVisible[selfPlayerID] = (getRawDataLocal->sprite->visibilityFlags & (1 << Broodwar->self()->getID())) != 0;
      //------------------------------------------------------------------------------------------------------
      //_getType
      if ( getRawDataLocal->unitID.id == BW::UnitID::Resource_MineralPatch1 ||
         getRawDataLocal->unitID.id == BW::UnitID::Resource_MineralPatch2 ||
         getRawDataLocal->unitID.id == BW::UnitID::Resource_MineralPatch3)
      {
        _getType = UnitTypes::Resource_Mineral_Field;
      }
      else
      {
        _getType = UnitType(getRawDataLocal->unitID.id);
      }
      //------------------------------------------------------------------------------------------------------
      //_getTransport
      if (_getType == UnitTypes::Protoss_Interceptor ||
          _getType == UnitTypes::Protoss_Scarab ||
          _getType == UnitTypes::Terran_Vulture_Spider_Mine)
      {
        if (getRawDataLocal->childUnitUnion3.inHanger==0 ||
            getRawDataLocal->status.getBit(BW::StatusFlags::InTransport) ||
            getRawDataLocal->status.getBit(BW::StatusFlags::InBuilding))
          _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->childInfoUnion.parentUnit));
      }
      else if (getRawDataLocal->status.getBit(BW::StatusFlags::InTransport) ||
               getRawDataLocal->status.getBit(BW::StatusFlags::InBuilding))
        _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->connectedUnit));
      else
        _getTransport = NULL;
      //------------------------------------------------------------------------------------------------------
      //_getPosition
      if (_getTransport!=NULL)
        _getPosition = Position(((UnitImpl*)_getTransport)->getRawDataLocal->position.x,((UnitImpl*)_getTransport)->getRawDataLocal->position.y);
      else
        _getPosition = Position(getRawDataLocal->position.x, getRawDataLocal->position.y);
      //------------------------------------------------------------------------------------------------------
      //_getHitPoints
      _getHitPoints = (int)ceil(getRawDataLocal->hitPoints / 256.0);
      //------------------------------------------------------------------------------------------------------
      //_getResources
      if (_getType != UnitTypes::Resource_Mineral_Field &&
          _getType != UnitTypes::Resource_Vespene_Geyser &&
          _getType != UnitTypes::Terran_Refinery &&
          _getType != UnitTypes::Protoss_Assimilator &&
          _getType != UnitTypes::Zerg_Extractor)
        _getResources = 0;
      else
        _getResources = getRawDataLocal->unitUnion1.unitUnion1Sub.resourceUnitUnionSub.resourceContained;
      //------------------------------------------------------------------------------------------------------
      //getBuildQueueSlot
      getBuildQueueSlot = getOriginalRawData->buildQueueSlot;
      //------------------------------------------------------------------------------------------------------
      //getBuildQueue
      getBuildQueue = (BW::UnitType*)getRawDataLocal->buildQueue;
      //------------------------------------------------------------------------------------------------------
      //hasEmptyBuildQueue
      if (getBuildQueueSlot < 5)
        hasEmptyBuildQueue = (getBuildQueue[getBuildQueueSlot] == BW::UnitID::None);
      else
        hasEmptyBuildQueue = false;
      //------------------------------------------------------------------------------------------------------
      //_isCompleted
      _isCompleted = getRawDataLocal->status.getBit(BW::StatusFlags::Completed);
    }
    else
    {
      //------------------------------------------------------------------------------------------------------
      //_getPlayer
      _getPlayer = savedPlayer;
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i=0;i<9;i++)
        self->isVisible[i] = false;
      //------------------------------------------------------------------------------------------------------
      //_getType
      _getType = savedUnitType;
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
    if (canAccess())
    {
      //------------------------------------------------------------------------------------------------------
      //getPosition
      self->positionX = _getPosition.x();
      self->positionY = _getPosition.y();
      //------------------------------------------------------------------------------------------------------
      //getAngle
      int d = getRawDataLocal->currentDirection;
      d -= 64;
      if (d < 0)
        d += 256;
      self->angle = (double)d * 3.14159265358979323846 / 128.0;
      //------------------------------------------------------------------------------------------------------
      //getVelocityX
      self->velocityX = (double)getRawDataLocal->current_speedX / 256.0;
      //------------------------------------------------------------------------------------------------------
      //getVelocityY
      self->velocityY = (double)getRawDataLocal->current_speedY / 256.0;
      //------------------------------------------------------------------------------------------------------
      //getHitPoints
      self->hitPoints = _getHitPoints;
      //------------------------------------------------------------------------------------------------------
      //getShields
      if (this->_getType.maxShields()>0)
        self->shields = (int)ceil(getRawDataLocal->shieldPoints/256.0);
      else
        self->shields = 0;
      //------------------------------------------------------------------------------------------------------
      //getEnergy
      if (this->_getType.isSpellcaster())
        self->energy = (int)ceil(getRawDataLocal->energy/256.0);
      else
        self->energy = 0;
      //------------------------------------------------------------------------------------------------------
      //getResources
      self->resources = _getResources;
      //------------------------------------------------------------------------------------------------------
      //getKillCount
      self->killCount = getRawDataLocal->killCount;
      //------------------------------------------------------------------------------------------------------
      //getGroundWeaponCooldown
      if (_getType==UnitTypes::Protoss_Reaver)
        self->groundWeaponCooldown = getRawDataLocal->mainOrderTimer;
      else if (getRawDataLocal->subUnit != NULL)
        self->groundWeaponCooldown = getRawDataLocal->subUnit->groundWeaponCooldown;
      else
        self->groundWeaponCooldown = getRawDataLocal->groundWeaponCooldown;
      //------------------------------------------------------------------------------------------------------
      //getAirWeaponCooldown
      if (getRawDataLocal->subUnit != NULL)
        self->airWeaponCooldown = getRawDataLocal->subUnit->airWeaponCooldown;
      else
        self->airWeaponCooldown = getRawDataLocal->airWeaponCooldown;
      //------------------------------------------------------------------------------------------------------
      //getSpellCooldown
      self->spellCooldown = getRawDataLocal->spellCooldown;
      //------------------------------------------------------------------------------------------------------
      //getDefenseMatrixPoints
      self->defenseMatrixPoints = getRawDataLocal->defenseMatrixDamage/256;
      //------------------------------------------------------------------------------------------------------
      //getDefenseMatrixTimer
      self->defenseMatrixTimer = getRawDataLocal->defenseMatrixTimer;
      //------------------------------------------------------------------------------------------------------
      //getEnsnareTimer
      self->ensnareTimer = getRawDataLocal->ensnareTimer;
      //------------------------------------------------------------------------------------------------------
      //getIrradiateTimer
      self->irradiateTimer = getRawDataLocal->irradiateTimer;
      //------------------------------------------------------------------------------------------------------
      //getLockdownTimer
      self->lockdownTimer = getRawDataLocal->lockdownTimer;
      //------------------------------------------------------------------------------------------------------
      //getMaelstromTimer
      self->maelstromTimer = getRawDataLocal->maelstromTimer;
      //------------------------------------------------------------------------------------------------------
      //getOrderTimer
      self->orderTimer = getRawDataLocal->mainOrderTimer;
      //------------------------------------------------------------------------------------------------------
      //getPlagueTimer
      self->plagueTimer = getRawDataLocal->plagueTimer;
      //------------------------------------------------------------------------------------------------------
      //getRemoveTimer
      self->removeTimer = getRawDataLocal->removeTimer;
      //------------------------------------------------------------------------------------------------------
      //getStasisTimer
      self->stasisTimer = getRawDataLocal->stasisTimer;
      //------------------------------------------------------------------------------------------------------
      //getStimTimer
      self->stimTimer = getRawDataLocal->stimTimer;
      //------------------------------------------------------------------------------------------------------
      //getOrder
      self->order = getRawDataLocal->orderID;
      //------------------------------------------------------------------------------------------------------
      //getSecondaryOrder
      self->secondaryOrder = getRawDataLocal->secondaryOrderID;
      //------------------------------------------------------------------------------------------------------
      //getBuildUnit
      if (getRawDataLocal->currentBuildUnit)
        self->buildUnit = BroodwarImpl.server.getUnitID(UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->currentBuildUnit));
      else
        self->buildUnit = -1;
      //------------------------------------------------------------------------------------------------------
      //isTraining
      if (_getType == UnitTypes::Terran_Nuclear_Silo &&
          getRawDataLocal->secondaryOrderID == BW::OrderID::Train)
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
      //------------------------------------------------------------------------------------------------------
      //isCompleted
      self->isCompleted = _isCompleted;
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
              (self->isCompleted==false && self->buildUnit != -1);
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
      //getHitPoints
      self->hitPoints = 0;
      //------------------------------------------------------------------------------------------------------
      //getShields
      self->shields = 0;
      //------------------------------------------------------------------------------------------------------
      //getEnergy
      self->energy = 0;
      //------------------------------------------------------------------------------------------------------
      //getResources
      self->resources = 0;
      //------------------------------------------------------------------------------------------------------
      //getKillCount
      self->killCount = 0;
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
      //getDefenseMatrixPoints
      self->defenseMatrixPoints = 0;
      //------------------------------------------------------------------------------------------------------
      //getDefenseMatrixTimer
      self->defenseMatrixTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getEnsnareTimer
      self->ensnareTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getIrradiateTimer
      self->irradiateTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getLockdownTimer
      self->lockdownTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getMaelstromTimer
      self->maelstromTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getOrderTimer
      self->orderTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getPlagueTimer
      self->plagueTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getRemoveTimer
      self->removeTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getStasisTimer
      self->stasisTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getStimTimer
      self->stimTimer = 0;
      //------------------------------------------------------------------------------------------------------
      //getOrder
      self->order = Orders::None.getID();
      //------------------------------------------------------------------------------------------------------
      //getSecondaryOrder
      self->secondaryOrder = Orders::None.getID();
      //------------------------------------------------------------------------------------------------------
      //getBuildUnit
      self->buildUnit = -1;
      //------------------------------------------------------------------------------------------------------
      //isTraining
      self->isTraining = false;
      //------------------------------------------------------------------------------------------------------
      //isMorphing
      self->isMorphing = false;
      //------------------------------------------------------------------------------------------------------
      //isCompleted
      self->isCompleted = false;
     //------------------------------------------------------------------------------------------------------
      //isConstructing
      self->isConstructing = false;
      //------------------------------------------------------------------------------------------------------
      //isIdle
      self->isIdle = false;
    }
    if (canAccessSpecial())
    {
      self->exists = _exists;
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
      //------------------------------------------------------------------------------------------------------
      //getScarabCount
      if (_getType==UnitTypes::Protoss_Reaver)
        self->scarabCount = getRawDataLocal->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion1.scarabCount;
      else
        self->scarabCount = 0;
      //------------------------------------------------------------------------------------------------------
      //getSpiderMineCount
      if (_getType == UnitTypes::Terran_Vulture)
        self->spiderMineCount = getRawDataLocal->childInfoUnion.vultureBikeMines.spiderMineCount;
      else
        self->spiderMineCount = 0;
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
      else if (self->order == BW::OrderID::ConstructingBuilding && self->buildUnit!=-1)
        self->buildType = ((UnitImpl*)getBuildUnit())->getRawDataLocal->unitID.id;
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
      //getTrainingQueue
      if (_getType == UnitTypes::Terran_Nuclear_Silo)
      {
        if (getRawDataLocal->secondaryOrderID == BW::OrderID::Train)
        {
          self->trainingQueue[0] = UnitTypes::Terran_Nuclear_Missile.getID();
          self->trainingQueueCount = 1;
        }
        else
          self->trainingQueueCount = 0;
      }
      else if (hasEmptyBuildQueue)
        self->trainingQueueCount = 0;
      else
      {
        self->trainingQueueCount = 0;
        int i = getBuildQueueSlot % 5;
        self->trainingQueue[self->trainingQueueCount] = getBuildQueue[i].id;
        self->trainingQueueCount++;
        i = (i + 1) % 5;
        while(getBuildQueue[i] != BW::UnitID::None && self->trainingQueueCount < 5)
        {
          self->trainingQueue[self->trainingQueueCount] = getBuildQueue[i].id;
          self->trainingQueueCount++;
          i = (i + 1) % 5;
        }
      }
      //------------------------------------------------------------------------------------------------------
      //getTech
      self->tech = getRawDataLocal->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion1.techID;
      //------------------------------------------------------------------------------------------------------
      //getUpgrade
      self->upgrade = getRawDataLocal->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion2.upgradeID;
      //------------------------------------------------------------------------------------------------------
      //getRemainingBuildTime
      if (self->isMorphing && self->buildType==UnitTypes::None.getID())
        self->remainingBuildTime = 0;
      else
        self->remainingBuildTime = getRawDataLocal->remainingBuildTime;
      //------------------------------------------------------------------------------------------------------
      //getRemainingTrainTime
      if (_getType == UnitTypes::Zerg_Hatchery ||
          _getType == UnitTypes::Zerg_Lair     ||
          _getType == UnitTypes::Zerg_Hive)
      {
        if (!self->isCompleted && self->buildType == UnitTypes::Zerg_Hatchery.getID())
          self->remainingTrainTime = self->remainingBuildTime;
        else
          self->remainingTrainTime = getRawDataLocal->childUnitUnion2.unitIsNotScarabInterceptor.larvaSpawnTimer * 9 + ((getRawDataLocal->unknownOrderTimer_0x085 + 8) % 9);
      }
      else if (getRawDataLocal->currentBuildUnit != NULL)
        self->remainingTrainTime = getRawDataLocal->currentBuildUnit->remainingBuildTime;
      else
        self->remainingTrainTime = 0;
      //------------------------------------------------------------------------------------------------------
      //getRemainingResearchTime
      if (self->order == BW::OrderID::ResearchTech)
        self->remainingResearchTime = getRawDataLocal->childUnitUnion1.unitIsBuilding.upgradeResearchTime;
      else
        self->remainingResearchTime = 0;
      //------------------------------------------------------------------------------------------------------
      //getRemainingUpgradeTime
      if (self->order == BW::OrderID::Upgrade)
        self->remainingUpgradeTime = getRawDataLocal->childUnitUnion1.unitIsBuilding.upgradeResearchTime;
      else
        self->remainingUpgradeTime = 0;
    }
    else
    {
      //------------------------------------------------------------------------------------------------------
      //getScarabCount
      self->scarabCount = 0;
      //------------------------------------------------------------------------------------------------------
      //getSpiderMineCount
      self->spiderMineCount = 0;
      //------------------------------------------------------------------------------------------------------
      //getBuildType
      self->buildType = UnitTypes::None.getID();
      //------------------------------------------------------------------------------------------------------
      //getTrainingQueue
      self->trainingQueueCount = 0;
      //------------------------------------------------------------------------------------------------------
      //getTech
      self->tech = TechTypes::None.getID();
      //------------------------------------------------------------------------------------------------------
      //getUpgrade
      self->upgrade = UpgradeTypes::None.getID();
      //------------------------------------------------------------------------------------------------------
      //getRemainingBuildTime
      self->remainingBuildTime = 0;
      //------------------------------------------------------------------------------------------------------
      //getRemainingTrainTime
      self->remainingTrainTime = 0;
      //------------------------------------------------------------------------------------------------------
      //getRemainingResearchTime
      self->remainingResearchTime = 0;
      //------------------------------------------------------------------------------------------------------
      //getRemainingUpgradeTime
      self->remainingUpgradeTime = 0;
    }
  }
}
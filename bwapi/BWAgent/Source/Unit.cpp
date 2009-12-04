
#include "Unit.h"
#include "BridgeClient.h"

#include <string>

#include "Util\Version.h"

namespace BWAgent
{
  Unit::Unit()
  {
  }
  int Unit::getID()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getID;
  }
  int Unit::getType()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getType;
  }


  int Unit::getHitPoints()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getHitPoints;
  }

  bool Unit::exists() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->exists;
  }

  bool Unit::isAccelerating() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isAccelerating;
  }
  bool Unit::isBeingConstructed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBeingConstructed;
  }
  bool Unit::isBeingGathered() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBeingGathered;
  }
  bool Unit::isBeingHealed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBeingHealed;
  }
  bool Unit::isBlind() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBlind;
  }
  bool Unit::isBraking() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBraking;
  }
  bool Unit::isBurrowed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBurrowed;
  }
  bool Unit::isCarryingGas() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCarryingGas;
  }
  bool Unit::isCarryingMinerals() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCarryingMinerals;
  }
  bool Unit::isCloaked() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCloaked;
  }
  bool Unit::isCompleted() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCompleted;
  }
  bool Unit::isConstructing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isConstructing;
  }
  bool Unit::isDefenseMatrixed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isDefenseMatrixed;
  }
  bool Unit::isEnsnared() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isEnsnared;
  }
  bool Unit::isFollowing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isFollowing;
  }
  bool Unit::isGatheringGas() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isGatheringGas;
  }
  bool Unit::isGatheringMinerals() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isGatheringMinerals;
  }
  bool Unit::isHallucination() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isHallucination;
  }
  bool Unit::isIdle() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isIdle;
  }
  bool Unit::isIrradiated() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isIrradiated;
  }
  bool Unit::isLifted() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isLifted;
  }
  bool Unit::isLoaded() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isLoaded;
  }
  bool Unit::isLockedDown() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isLockedDown;
  }
  bool Unit::isMaelstrommed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isMaelstrommed;
  }
  bool Unit::isMorphing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isMorphing;
  }
  bool Unit::isMoving() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isMoving;
  }
  bool Unit::isParasited() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isParasited;
  }
  bool Unit::isPatrolling() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isPatrolling;
  }
  bool Unit::isPlagued() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isPlagued;
  }
  bool Unit::isRepairing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isRepairing;
  }
  bool Unit::isResearching() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isResearching;
  }
  bool Unit::isSelected() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isSelected;
  }
  bool Unit::isSieged() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isSieged;
  }
  bool Unit::isStartingAttack() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isStartingAttack;
  }
  bool Unit::isStasised() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isStasised;
  }
  bool Unit::isStimmed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isStimmed;
  }
  bool Unit::isTraining() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isTraining;
  }
  bool Unit::isUnderStorm() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isUnderStorm;
  }
  bool Unit::isUnpowered() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isUnpowered;
  }
  bool Unit::isUpgrading() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isUpgrading;
  }
  bool Unit::isVisible() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isVisible;
  }

  bool Unit::holdPosition()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    BWAPI::UnitCommand* c = &(BridgeClient::sharedCommandData->commandQueue[BridgeClient::sharedCommandData->lastFreeCommandSlot]);
    c->unitID = data->getID;
    c->commandID = BWAPI::CommandID::HoldPosition;
    BridgeClient::sharedCommandData->lastFreeCommandSlot++;
    return true;
  }
  void Unit::_update(BWAPI::ClearanceLevels level,BWAPI::State* data)
  {
    this->level=level;
    this->data=data;
  }
}

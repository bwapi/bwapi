
#include "Unit.h"
#include "BridgeClient.h"

#include <string>

#include "Bridge\UnitCommandEntry.h"

#include "Util\Version.h"

namespace BWAgent
{
//  void pushCommand(BWAPI::CommandID commandID, int unitID, int x=0, int y=0, int targetID=0, int specialID=0);
  Unit::Unit()
  {
  }
  int Unit::getID() const
  {
    return getRespectClearance(&BWAPI::UnitState::id);
  }
  int Unit::getPlayer() const
  {
    return getRespectClearance(&BWAPI::UnitState::player);
  }
  int Unit::getType() const
  {
    return getRespectClearance(&BWAPI::UnitState::type);
  }
  int Unit::getHitPoints() const
  {
    return getRespectClearance(&BWAPI::UnitState::hitPoints);
  }
  int Unit::getShields() const
  {
    return getRespectClearance(&BWAPI::UnitState::shields);
  }
  int Unit::getEnergy() const
  {
    return getRespectClearance(&BWAPI::UnitState::energy);
  }
  int Unit::getResources() const
  {
    return getRespectClearance(&BWAPI::UnitState::resources);
  }
  int Unit::getKillCount() const
  {
    return getRespectClearance(&BWAPI::UnitState::killCount);
  }
  int Unit::getGroundWeaponCooldown() const
  {
    return getRespectClearance(&BWAPI::UnitState::groundWeaponCooldown);
  }
  int Unit::getAirWeaponCooldown() const
  {
    return getRespectClearance(&BWAPI::UnitState::airWeaponCooldown);
  }
  int Unit::getSpellCooldown() const
  {
    return getRespectClearance(&BWAPI::UnitState::spellCooldown);
  }
  int Unit::getDefenseMatrixPoints() const
  {
    return getRespectClearance(&BWAPI::UnitState::defenseMatrixPoints);
  }

  int Unit::getDefenseMatrixTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::defenseMatrixTimer);
  }
  int Unit::getEnsnareTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::ensnareTimer);
  }
  int Unit::getIrradiateTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::irradiateTimer);
  }
  int Unit::getLockdownTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::lockdownTimer);
  }
  int Unit::getMaelstromTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::maelstromTimer);
  }
  int Unit::getPlagueTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::plagueTimer);
  }
  int Unit::getRemoveTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::removeTimer);
  }
  int Unit::getStasisTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::stasisTimer);
  }
  int Unit::getStimTimer() const
  {
    return getRespectClearance(&BWAPI::UnitState::stimTimer);
  }

  bool Unit::isAccelerating() const
  {
    return getRespectClearance(&BWAPI::UnitState::isAccelerating);
  }
  bool Unit::isBeingConstructed() const
  {
    return getRespectClearance(&BWAPI::UnitState::isBeingConstructed);
  }
  bool Unit::isBeingGathered() const
  {
    return getRespectClearance(&BWAPI::UnitState::isBeingGathered);
  }
  bool Unit::isBeingHealed() const
  {
    return getRespectClearance(&BWAPI::UnitState::isBeingHealed);
  }
  bool Unit::isBlind() const
  {
    return getRespectClearance(&BWAPI::UnitState::isBlind);
  }
  bool Unit::isBraking() const
  {
    return getRespectClearance(&BWAPI::UnitState::isBraking);
  }
  bool Unit::isBurrowed() const
  {
    return getRespectClearance(&BWAPI::UnitState::isBurrowed);
  }
  bool Unit::isCarryingGas() const
  {
    return getRespectClearance(&BWAPI::UnitState::isCarryingGas);
  }
  bool Unit::isCarryingMinerals() const
  {
    return getRespectClearance(&BWAPI::UnitState::isCarryingMinerals);
  }
  bool Unit::isCloaked() const
  {
    return getRespectClearance(&BWAPI::UnitState::isCloaked);
  }
  bool Unit::isCompleted() const
  {
    return getRespectClearance(&BWAPI::UnitState::isCompleted);
  }
  bool Unit::isConstructing() const
  {
    return getRespectClearance(&BWAPI::UnitState::isConstructing);
  }
  bool Unit::isDefenseMatrixed() const
  {
    return getRespectClearance(&BWAPI::UnitState::isDefenseMatrixed);
  }
  bool Unit::isEnsnared() const
  {
    return getRespectClearance(&BWAPI::UnitState::isEnsnared);
  }
  bool Unit::isFollowing() const
  {
    return getRespectClearance(&BWAPI::UnitState::isFollowing);
  }
  bool Unit::isGatheringGas() const
  {
    return getRespectClearance(&BWAPI::UnitState::isGatheringGas);
  }
  bool Unit::isGatheringMinerals() const
  {
    return getRespectClearance(&BWAPI::UnitState::isGatheringMinerals);
  }
  bool Unit::isHallucination() const
  {
    return getRespectClearance(&BWAPI::UnitState::isHallucination);
  }
  bool Unit::isIdle() const
  {
    return getRespectClearance(&BWAPI::UnitState::isIdle);
  }
  bool Unit::isIrradiated() const
  {
    return getRespectClearance(&BWAPI::UnitState::isIrradiated);
  }
  bool Unit::isLifted() const
  {
    return getRespectClearance(&BWAPI::UnitState::isLifted);
  }
  bool Unit::isLoaded() const
  {
    return getRespectClearance(&BWAPI::UnitState::isLoaded);
  }
  bool Unit::isLockedDown() const
  {
    return getRespectClearance(&BWAPI::UnitState::isLockedDown);
  }
  bool Unit::isMaelstrommed() const
  {
    return getRespectClearance(&BWAPI::UnitState::isMaelstrommed);
  }
  bool Unit::isMorphing() const
  {
    return getRespectClearance(&BWAPI::UnitState::isMorphing);
  }
  bool Unit::isMoving() const
  {
    return getRespectClearance(&BWAPI::UnitState::isMoving);
  }
  bool Unit::isParasited() const
  {
    return getRespectClearance(&BWAPI::UnitState::isParasited);
  }
  bool Unit::isPatrolling() const
  {
    return getRespectClearance(&BWAPI::UnitState::isPatrolling);
  }
  bool Unit::isPlagued() const
  {
    return getRespectClearance(&BWAPI::UnitState::isPlagued);
  }
  bool Unit::isRepairing() const
  {
    return getRespectClearance(&BWAPI::UnitState::isRepairing);
  }
  bool Unit::isResearching() const
  {
    return getRespectClearance(&BWAPI::UnitState::isResearching);
  }
  bool Unit::isSelected() const
  {
    return getRespectClearance(&BWAPI::UnitState::isSelected);
  }
  bool Unit::isSieged() const
  {
    return getRespectClearance(&BWAPI::UnitState::isSieged);
  }
  bool Unit::isStartingAttack() const
  {
    return getRespectClearance(&BWAPI::UnitState::isStartingAttack);
  }
  bool Unit::isStasised() const
  {
    return getRespectClearance(&BWAPI::UnitState::isStasised);
  }
  bool Unit::isStimmed() const
  {
    return getRespectClearance(&BWAPI::UnitState::isStimmed);
  }
  bool Unit::isTraining() const
  {
    return getRespectClearance(&BWAPI::UnitState::isTraining);
  }
  bool Unit::isUnderStorm() const
  {
    return getRespectClearance(&BWAPI::UnitState::isUnderStorm);
  }
  bool Unit::isUnpowered() const
  {
    return getRespectClearance(&BWAPI::UnitState::isUnpowered);
  }
  bool Unit::isUpgrading() const
  {
    return getRespectClearance(&BWAPI::UnitState::isUpgrading);
  }
  bool Unit::isVisible() const
  {
    if(!this->knownUnit)
      return false;
    return this->knownUnit->clearance > BWAPI::ClearanceLevels::Noticed;
  }
  bool Unit::attackUnit(Unit* target)
  {
//    pushCommand(BWAPI::CommandID::AttackUnit,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::rightClick(Unit* target)
  {
//    pushCommand(BWAPI::CommandID::RightClickUnit,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::stop()
  {
//    pushCommand(BWAPI::CommandID::Stop,data->getID);
    return true;
  }
  bool Unit::holdPosition()
  {
//    pushCommand(BWAPI::CommandID::HoldPosition,data->getID);
    return true;
  }
  bool Unit::follow(Unit* target)
  {
//    pushCommand(BWAPI::CommandID::Follow,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::setRallyUnit(Unit* target)
  {
//    pushCommand(BWAPI::CommandID::SetRallyUnit,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::repair(Unit* target)
  {
//    pushCommand(BWAPI::CommandID::Repair,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::burrow()
  {
//    pushCommand(BWAPI::CommandID::Burrow,data->getID);
    return true;
  }
  bool Unit::unburrow()
  {
//    pushCommand(BWAPI::CommandID::Unburrow,data->getID);
    return true;
  }
  bool Unit::siege()
  {
//    pushCommand(BWAPI::CommandID::Siege,data->getID);
    return true;
  }
  bool Unit::unsiege()
  {
//    pushCommand(BWAPI::CommandID::Unsiege,data->getID);
    return true;
  }
  bool Unit::cloak()
  {
//    pushCommand(BWAPI::CommandID::Cloak,data->getID);
    return true;
  }
  bool Unit::decloak()
  {
//    pushCommand(BWAPI::CommandID::Decloak,data->getID);
    return true;
  }
  bool Unit::lift()
  {
//    pushCommand(BWAPI::CommandID::Lift,data->getID);
    return true;
  }
  bool Unit::load(Unit* target)
  {
//    pushCommand(BWAPI::CommandID::Load,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::unload(Unit* target)
  {
//    pushCommand(BWAPI::CommandID::Unload,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::unloadAll()
  {
//    pushCommand(BWAPI::CommandID::UnloadAll,data->getID);
    return true;
  }
  bool Unit::cancelConstruction()
  {
//    pushCommand(BWAPI::CommandID::CancelConstruction,data->getID);
    return true;
  }
  bool Unit::haltConstruction()
  {
//    pushCommand(BWAPI::CommandID::HaltConstruction,data->getID);
    return true;
  }
  bool Unit::cancelMorph()
  {
//    pushCommand(BWAPI::CommandID::CancelMorph,data->getID);
    return true;
  }
  bool Unit::cancelTrain()
  {
//    pushCommand(BWAPI::CommandID::CancelTrain,data->getID);
    return true;
  }
  bool Unit::cancelTrain(int slot)
  {
//    pushCommand(BWAPI::CommandID::CancelTrain,data->getID,0,0,0,slot);
    return true;
  }
  bool Unit::cancelAddon()
  {
//    pushCommand(BWAPI::CommandID::CancelAddon,data->getID);
    return true;
  }
  bool Unit::cancelResearch()
  {
//    pushCommand(BWAPI::CommandID::CancelResearch,data->getID);
    return true;
  }
  bool Unit::cancelUpgrade()
  {
//    pushCommand(BWAPI::CommandID::CancelUpgrade,data->getID);
    return true;
  }

  void pushCommand(BWAPI::UnitCommandTypeID commandID, int unitID, int x, int y, int targetID, int specialID)
  {
    /*
    BWAPI::UnitCommand* c = &(BridgeClient::sharedCommandData->commandQueue[BridgeClient::sharedCommandData->lastFreeCommandSlot]);
    c->commandID=commandID;
    c->unitID=unitID;
    c->x=x;
    c->y=y;
    c->targetID=targetID;
    c->specialID=specialID;
    BridgeClient::sharedCommandData->lastFreeCommandSlot++;
    */
  }
}

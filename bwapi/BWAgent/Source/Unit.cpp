
#include "Unit.h"
#include "BridgeClient.h"

#include <string>

#include "Util\Version.h"

namespace BWAgent
{
  void pushCommand(BWAPI::CommandID::Enum commandID, int unitID, int x=0, int y=0, int targetID=0, int specialID=0);
  Unit::Unit()
  {
  }
  int Unit::getID() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::id);
  }
  int Unit::getPlayer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::player);
  }
  int Unit::getType() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::type);
  }
  int Unit::getHitPoints() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::hitPoints);
  }
  int Unit::getShields() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::shields);
  }
  int Unit::getEnergy() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::energy);
  }
  int Unit::getResources() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::resources);
  }
  int Unit::getKillCount() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::killCount);
  }
  int Unit::getGroundWeaponCooldown() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::groundWeaponCooldown);
  }
  int Unit::getAirWeaponCooldown() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::airWeaponCooldown);
  }
  int Unit::getSpellCooldown() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::spellCooldown);
  }
  int Unit::getDefenseMatrixPoints() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::defenseMatrixPoints);
  }

  int Unit::getDefenseMatrixTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::defenseMatrixTimer);
  }
  int Unit::getEnsnareTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::ensnareTimer);
  }
  int Unit::getIrradiateTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::irradiateTimer);
  }
  int Unit::getLockdownTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::lockdownTimer);
  }
  int Unit::getMaelstromTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::maelstromTimer);
  }
  int Unit::getPlagueTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::plagueTimer);
  }
  int Unit::getRemoveTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::removeTimer);
  }
  int Unit::getStasisTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::stasisTimer);
  }
  int Unit::getStimTimer() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::stimTimer);
  }

  bool Unit::isAccelerating() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isAccelerating);
  }
  bool Unit::isBeingConstructed() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isBeingConstructed);
  }
  bool Unit::isBeingGathered() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isBeingGathered);
  }
  bool Unit::isBeingHealed() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isBeingHealed);
  }
  bool Unit::isBlind() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isBlind);
  }
  bool Unit::isBraking() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isBraking);
  }
  bool Unit::isBurrowed() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isBurrowed);
  }
  bool Unit::isCarryingGas() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isCarryingGas);
  }
  bool Unit::isCarryingMinerals() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isCarryingMinerals);
  }
  bool Unit::isCloaked() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isCloaked);
  }
  bool Unit::isCompleted() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isCompleted);
  }
  bool Unit::isConstructing() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isConstructing);
  }
  bool Unit::isDefenseMatrixed() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isDefenseMatrixed);
  }
  bool Unit::isEnsnared() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isEnsnared);
  }
  bool Unit::isFollowing() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isFollowing);
  }
  bool Unit::isGatheringGas() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isGatheringGas);
  }
  bool Unit::isGatheringMinerals() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isGatheringMinerals);
  }
  bool Unit::isHallucination() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isHallucination);
  }
  bool Unit::isIdle() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isIdle);
  }
  bool Unit::isIrradiated() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isIrradiated);
  }
  bool Unit::isLifted() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isLifted);
  }
  bool Unit::isLoaded() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isLoaded);
  }
  bool Unit::isLockedDown() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isLockedDown);
  }
  bool Unit::isMaelstrommed() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isMaelstrommed);
  }
  bool Unit::isMorphing() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isMorphing);
  }
  bool Unit::isMoving() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isMoving);
  }
  bool Unit::isParasited() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isParasited);
  }
  bool Unit::isPatrolling() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isPatrolling);
  }
  bool Unit::isPlagued() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isPlagued);
  }
  bool Unit::isRepairing() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isRepairing);
  }
  bool Unit::isResearching() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isResearching);
  }
  bool Unit::isSelected() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isSelected);
  }
  bool Unit::isSieged() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isSieged);
  }
  bool Unit::isStartingAttack() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isStartingAttack);
  }
  bool Unit::isStasised() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isStasised);
  }
  bool Unit::isStimmed() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isStimmed);
  }
  bool Unit::isTraining() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isTraining);
  }
  bool Unit::isUnderStorm() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isUnderStorm);
  }
  bool Unit::isUnpowered() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isUnpowered);
  }
  bool Unit::isUpgrading() const
  {
    return getRespectClearance(this->knownUnit, &BWAPI::UnitState::isUpgrading);
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

  void pushCommand(BWAPI::CommandID::Enum commandID, int unitID, int x, int y, int targetID, int specialID)
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

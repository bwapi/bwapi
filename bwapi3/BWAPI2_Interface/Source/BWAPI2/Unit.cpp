#include "Unit.h"

#include <Util\Version.h>

#include <BWAPI\all.h>
#include "Game.h"
#include <string>

namespace BWAPI2
{
//  void pushCommand(BWAPI::CommandID commandID, int unitID, int x=0, int y=0, int targetID=0, int specialID=0);
  Unit::Unit(int id)
  {
    this->self=BWAPI::getUnit(id);
    this->id;
  }
  int Unit::getID() const
  {
    return id;
  }
  Player* Unit::getPlayer() const
  {
    return Game::getPlayer(self->player);
  }
  UnitType Unit::getType() const
  {
    return UnitType(self->type);
  }
  int Unit::getHitPoints() const
  {
    return self->hitPoints;
  }
  int Unit::getShields() const
  {
    return self->shields;
  }
  int Unit::getEnergy() const
  {
    return self->energy;
  }
  int Unit::getResources() const
  {
    return self->resources;
  }
  int Unit::getKillCount() const
  {
    return self->killCount;
  }
  int Unit::getGroundWeaponCooldown() const
  {
    return self->groundWeaponCooldown;
  }
  int Unit::getAirWeaponCooldown() const
  {
    return self->airWeaponCooldown;
  }
  int Unit::getSpellCooldown() const
  {
    return self->spellCooldown;
  }
  int Unit::getDefenseMatrixPoints() const
  {
    return self->defenseMatrixPoints;
  }
  int Unit::getDefenseMatrixTimer() const
  {
    return self->defenseMatrixTimer;
  }
  int Unit::getEnsnareTimer() const
  {
    return self->ensnareTimer;
  }
  int Unit::getIrradiateTimer() const
  {
    return self->irradiateTimer;
  }
  int Unit::getLockdownTimer() const
  {
    return self->lockdownTimer;
  }
  int Unit::getMaelstromTimer() const
  {
    return self->maelstromTimer;
  }
  int Unit::getPlagueTimer() const
  {
    return self->plagueTimer;
  }
  int Unit::getRemoveTimer() const
  {
    return self->removeTimer;
  }
  int Unit::getStasisTimer() const
  {
    return self->stasisTimer;
  }
  int Unit::getStimTimer() const
  {
    return self->stimTimer;
  }
  bool Unit::isAccelerating() const
  {
    return self->isAccelerating;
  }
  bool Unit::isBeingConstructed() const
  {
    return self->isBeingConstructed;
  }
  bool Unit::isBeingGathered() const
  {
    return self->isBeingGathered;
  }
  bool Unit::isBeingHealed() const
  {
    return self->isBeingHealed;
  }
  bool Unit::isBlind() const
  {
    return self->isBlind;
  }
  bool Unit::isBraking() const
  {
    return self->isBraking;
  }
  bool Unit::isBurrowed() const
  {
    return self->isBurrowed;
  }
  bool Unit::isCarryingGas() const
  {
    return self->isCarryingGas;
  }
  bool Unit::isCarryingMinerals() const
  {
    return self->isCarryingMinerals;
  }
  bool Unit::isCloaked() const
  {
    return self->isCloaked;
  }
  bool Unit::isCompleted() const
  {
    return self->isCompleted;
  }
  bool Unit::isConstructing() const
  {
    return self->isConstructing;
  }
  bool Unit::isDefenseMatrixed() const
  {
    return self->isDefenseMatrixed;
  }
  bool Unit::isEnsnared() const
  {
    return self->isEnsnared;
  }
  bool Unit::isFollowing() const
  {
    return self->isFollowing;
  }
  bool Unit::isGatheringGas() const
  {
    return self->isGatheringGas;
  }
  bool Unit::isGatheringMinerals() const
  {
    return self->isGatheringMinerals;
  }
  bool Unit::isHallucination() const
  {
    return self->isHallucination;
  }
  bool Unit::isIdle() const
  {
    return self->isIdle;
  }
  bool Unit::isIrradiated() const
  {
    return self->isIrradiated;
  }
  bool Unit::isLifted() const
  {
    return self->isLifted;
  }
  bool Unit::isLoaded() const
  {
    return self->isLoaded;
  }
  bool Unit::isLockedDown() const
  {
    return self->isLockedDown;
  }
  bool Unit::isMaelstrommed() const
  {
    return self->isMaelstrommed;
  }
  bool Unit::isMorphing() const
  {
    return self->isMorphing;
  }
  bool Unit::isMoving() const
  {
    return self->isMoving;
  }
  bool Unit::isParasited() const
  {
    return self->isParasited;
  }
  bool Unit::isPatrolling() const
  {
    return self->isPatrolling;
  }
  bool Unit::isPlagued() const
  {
    return self->isPlagued;
  }
  bool Unit::isRepairing() const
  {
    return self->isRepairing;
  }
  bool Unit::isResearching() const
  {
    return self->isResearching;
  }
  bool Unit::isSelected() const
  {
    return self->isSelected;
  }
  bool Unit::isSieged() const
  {
    return self->isSieged;
  }
  bool Unit::isStartingAttack() const
  {
    return false;//FIX FIX FIX self->isStartingAttack;
  }
  bool Unit::isStasised() const
  {
    return self->isStasised;
  }
  bool Unit::isStimmed() const
  {
    return self->isStimmed;
  }
  bool Unit::isTraining() const
  {
    return self->isTraining;
  }
  bool Unit::isUnderStorm() const
  {
    return self->isUnderStorm;
  }
  bool Unit::isUnpowered() const
  {
    return self->isUnpowered;
  }
  bool Unit::isUpgrading() const
  {
    return self->isUpgrading;
  }
  bool Unit::isVisible() const
  {
    return self->clearanceLevel>=3;
  }
  bool Unit::attackUnit(Unit* target)
  {
    BWAPI::orderAttackUnit(this->id,target->id);
    return true;
  }
  bool Unit::rightClick(Unit* target)
  {
    BWAPI::orderRightClickUnit(this->id,target->id);
    return true;
  }
  bool Unit::stop()
  {
    BWAPI::orderStop(this->id);
    return true;
  }
  bool Unit::holdPosition()
  {
    BWAPI::orderHoldPosition(this->id);
    return true;
  }
  bool Unit::follow(Unit* target)
  {
    BWAPI::orderFollow(this->id,target->id);
    return true;
  }
  bool Unit::setRallyUnit(Unit* target)
  {
    BWAPI::orderSetRallyUnit(this->id,target->id);
    return true;
  }
  bool Unit::repair(Unit* target)
  {
    BWAPI::orderRepair(this->id,target->id);
    return true;
  }
  bool Unit::burrow()
  {
    BWAPI::orderBurrow(this->id);
    return true;
  }
  bool Unit::unburrow()
  {
    BWAPI::orderUnburrow(this->id);
    return true;
  }
  bool Unit::siege()
  {
    BWAPI::orderSiege(this->id);
    return true;
  }
  bool Unit::unsiege()
  {
    BWAPI::orderUnsiege(this->id);
    return true;
  }
  bool Unit::cloak()
  {
    BWAPI::orderCloak(this->id);
    return true;
  }
  bool Unit::decloak()
  {
    BWAPI::orderDecloak(this->id);
    return true;
  }
  bool Unit::lift()
  {
    BWAPI::orderLift(this->id);
    return true;
  }
  bool Unit::load(Unit* target)
  {
    BWAPI::orderLoad(this->id,target->id);
    return true;
  }
  bool Unit::unload(Unit* target)
  {
    BWAPI::orderUnload(this->id,target->id);
    return true;
  }
  bool Unit::unloadAll()
  {
    BWAPI::orderUnloadAll(this->id);
    return true;
  }
  bool Unit::cancelConstruction()
  {
    BWAPI::orderCancelConstruction(this->id);
    return true;
  }
  bool Unit::haltConstruction()
  {
    BWAPI::orderHaltConstruction(this->id);
    return true;
  }
  bool Unit::cancelMorph()
  {
    BWAPI::orderCancelMorph(this->id);
    return true;
  }
  bool Unit::cancelTrain()
  {
    BWAPI::orderCancelTrain(this->id);
    return true;
  }
  bool Unit::cancelTrain(int slot)
  {
    BWAPI::orderCancelTrainSlot(this->id,slot);
    return true;
  }
  bool Unit::cancelAddon()
  {
    BWAPI::orderCancelAddon(this->id);
    return true;
  }
  bool Unit::cancelResearch()
  {
    BWAPI::orderCancelResearch(this->id);
    return true;
  }
  bool Unit::cancelUpgrade()
  {
    BWAPI::orderCancelUpgrade(this->id);
    return true;
  }

  /*
  void pushCommand(BWAPI::UnitCommandTypeID commandID, int unitID, int x, int y, int targetID, int specialID)
  {
    BWAPI::UnitCommand* c = &(BridgeClient::sharedCommandData->commandQueue[BridgeClient::sharedCommandData->lastFreeCommandSlot]);
    c->commandID=commandID;
    c->unitID=unitID;
    c->x=x;
    c->y=y;
    c->targetID=targetID;
    c->specialID=specialID;
    BridgeClient::sharedCommandData->lastFreeCommandSlot++;
  }
  */
}

#pragma once

#include <list>

#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>

namespace BWAPI
{
  class Player;
  class Unit
  {
    public:
      virtual Player* __cdecl getPlayer() const = 0;
      virtual UnitType __cdecl getType() const = 0;
      virtual int __cdecl getHitPoints() const = 0;
      virtual int __cdecl getShields() const = 0;
      virtual int __cdecl getEnergy() const = 0;
      virtual int __cdecl getResources() const = 0;
      virtual int __cdecl getKillCount() const = 0;
      virtual int __cdecl getGroundWeaponCooldown() const = 0;
      virtual int __cdecl getAirWeaponCooldown() const = 0;
      virtual int __cdecl getSpellCooldown() const = 0;
      virtual int __cdecl getDefenseMatrixPoints() const = 0;

      virtual int __cdecl getDefenseMatrixTimer() const = 0;
      virtual int __cdecl getEnsnareTimer() const = 0;
      virtual int __cdecl getIrradiateTimer() const = 0;
      virtual int __cdecl getLockdownTimer() const = 0;
      virtual int __cdecl getMaelstromTimer() const = 0;
      virtual int __cdecl getPlagueTimer() const = 0;
      virtual int __cdecl getRemoveTimer() const = 0;
      virtual int __cdecl getStasisTimer() const = 0;
      virtual int __cdecl getStimTimer() const = 0;

      virtual Position __cdecl getPosition() const = 0;
      virtual TilePosition __cdecl getTilePosition() const = 0;
      virtual double __cdecl getDistance(Unit* target) const = 0;
      virtual double __cdecl getDistance(Position target) const = 0;
      virtual int __cdecl getCurrentDirection() const = 0;

      virtual Unit* __cdecl getTarget() const = 0;
      virtual Position __cdecl getTargetPosition() const = 0;
      virtual Order __cdecl getOrder() const = 0;
      virtual Unit* __cdecl getOrderTarget() const = 0;
      virtual int __cdecl getOrderTimer() const = 0;
      virtual Order __cdecl getSecondaryOrder() const = 0;
      virtual Unit* __cdecl getBuildUnit() const = 0;
      virtual int __cdecl getRemainingBuildTime() const = 0;
      virtual int __cdecl getRemainingTrainTime() const = 0;
      virtual Unit* __cdecl getChild() const = 0;
      virtual std::list<UnitType > __cdecl getTrainingQueue() const = 0;
      virtual Unit* __cdecl getTransport() const = 0;
      virtual std::list<Unit*> __cdecl getLoadedUnits() const = 0;
      virtual int __cdecl getInterceptorCount() const = 0;
      virtual int __cdecl getScarabCount() const = 0;
      virtual int __cdecl getSpiderMineCount() const = 0;
      virtual TechType __cdecl getTech() const = 0;
      virtual UpgradeType __cdecl getUpgrade() const = 0;
      virtual int __cdecl getRemainingResearchTime() const = 0;
      virtual int __cdecl getRemainingUpgradeTime() const = 0;
      virtual Position __cdecl getRallyPosition() const = 0;
      virtual Unit* __cdecl getRallyUnit() const = 0;
      virtual Unit* __cdecl getAddon() const = 0;

      virtual bool __cdecl exists() const = 0;
      virtual bool __cdecl isAccelerating() const = 0;
      virtual bool __cdecl isBeingConstructed() const = 0;
      virtual bool __cdecl isBeingHealed() const = 0;
      virtual bool __cdecl isBlind() const = 0;
      virtual bool __cdecl isBraking() const = 0;
      virtual bool __cdecl isBurrowed() const = 0;
      virtual bool __cdecl isCarryingGas() const = 0;
      virtual bool __cdecl isCarryingMinerals() const = 0;
      virtual bool __cdecl isCloaked() const = 0;
      virtual bool __cdecl isCompleted() const = 0;
      virtual bool __cdecl isConstructing() const = 0;
      virtual bool __cdecl isDefenseMatrixed() const = 0;
      virtual bool __cdecl isEnsnared() const = 0;
      virtual bool __cdecl isFollowing() const = 0;
      virtual bool __cdecl isGatheringGas() const = 0;
      virtual bool __cdecl isGatheringMinerals() const = 0;
      virtual bool __cdecl isHallucination() const = 0;
      virtual bool __cdecl isIdle() const = 0;
      virtual bool __cdecl isIrradiated() const = 0;
      virtual bool __cdecl isLifted() const = 0;
      virtual bool __cdecl isLoaded() const = 0;
      virtual bool __cdecl isLockedDown() const = 0;
      virtual bool __cdecl isMaelstrommed() const = 0;
      virtual bool __cdecl isMorphing() const = 0;
      virtual bool __cdecl isMoving() const = 0;
      virtual bool __cdecl isParasited() const = 0;
      virtual bool __cdecl isPatrolling() const = 0;
      virtual bool __cdecl isPlagued() const = 0;
      virtual bool __cdecl isRepairing() const = 0;
      virtual bool __cdecl isResearching() const = 0;
      virtual bool __cdecl isSelected() const = 0;
      virtual bool __cdecl isSieged() const = 0;
      virtual bool __cdecl isStartingAttack() const = 0;
      virtual bool __cdecl isStasised() const = 0;
      virtual bool __cdecl isStimmed() const = 0;
      virtual bool __cdecl isTraining() const = 0;
      virtual bool __cdecl isUnderStorm() const = 0;
      virtual bool __cdecl isUnpowered() const = 0;
      virtual bool __cdecl isUpgrading() const = 0;
      virtual bool __cdecl isVisible() const = 0;

      virtual bool __cdecl attackMove(Position position) = 0;
      virtual bool __cdecl attackUnit(Unit* target) = 0;
      virtual bool __cdecl rightClick(Position position) = 0;
      virtual bool __cdecl rightClick(Unit* target) = 0;
      virtual bool __cdecl train(UnitType type) = 0;
      virtual bool __cdecl build(TilePosition position, UnitType type) = 0;
      virtual bool __cdecl buildAddon(UnitType type) = 0;
      virtual bool __cdecl research(TechType tech) = 0;
      virtual bool __cdecl upgrade(UpgradeType upgrade) = 0;
      virtual bool __cdecl stop() = 0;
      virtual bool __cdecl holdPosition() = 0;
      virtual bool __cdecl patrol(Position position) = 0;
      virtual bool __cdecl follow(Unit* target) = 0;
      virtual bool __cdecl setRallyPosition(Position target) = 0;
      virtual bool __cdecl setRallyUnit(Unit* target) = 0;
      virtual bool __cdecl repair(Unit* target) = 0;
      virtual bool __cdecl morph(UnitType type) = 0;
      virtual bool __cdecl burrow() = 0;
      virtual bool __cdecl unburrow() = 0;
      virtual bool __cdecl siege() = 0;
      virtual bool __cdecl unsiege() = 0;
      virtual bool __cdecl cloak() = 0;
      virtual bool __cdecl decloak() = 0;
      virtual bool __cdecl lift() = 0;
      virtual bool __cdecl land(TilePosition position) = 0;
      virtual bool __cdecl load(Unit* target) = 0;
      virtual bool __cdecl unload(Unit* target) = 0;
      virtual bool __cdecl unloadAll() = 0;
      virtual bool __cdecl unloadAll(Position position) = 0;
      virtual bool __cdecl cancelConstruction() = 0;
      virtual bool __cdecl haltConstruction() = 0;
      virtual bool __cdecl cancelMorph() = 0;
      virtual bool __cdecl cancelTrain() = 0;
      virtual bool __cdecl cancelTrain(int slot) = 0;
      virtual bool __cdecl cancelAddon() = 0;
      virtual bool __cdecl cancelResearch() = 0;
      virtual bool __cdecl cancelUpgrade() = 0;
      virtual bool __cdecl useTech(TechType tech) = 0;
      virtual bool __cdecl useTech(TechType tech, Position position) = 0;
      virtual bool __cdecl useTech(TechType tech, Unit* target) = 0;
  };
}

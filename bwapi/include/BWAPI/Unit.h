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
      virtual Player* getPlayer() const = 0;
      virtual UnitType getType() const = 0;
      virtual int getHitPoints() const = 0;
      virtual int getInitialHitPoints() const = 0;
      virtual int getShields() const = 0;
      virtual int getEnergy() const = 0;
      virtual int getResources() const = 0;
      virtual int getInitialResources() const = 0;
      virtual int getKillCount() const = 0;
      virtual int getGroundWeaponCooldown() const = 0;
      virtual int getAirWeaponCooldown() const = 0;
      virtual int getSpellCooldown() const = 0;
      virtual int getDefenseMatrixPoints() const = 0;

      virtual int getDefenseMatrixTimer() const = 0;
      virtual int getEnsnareTimer() const = 0;
      virtual int getIrradiateTimer() const = 0;
      virtual int getLockdownTimer() const = 0;
      virtual int getMaelstromTimer() const = 0;
      virtual int getPlagueTimer() const = 0;
      virtual int getRemoveTimer() const = 0;
      virtual int getStasisTimer() const = 0;
      virtual int getStimTimer() const = 0;

      virtual Position getPosition() const = 0;
      virtual Position getInitialPosition() const = 0;
      virtual TilePosition getTilePosition() const = 0;
      virtual TilePosition getInitialTilePosition() const = 0;
      virtual double getDistance(Unit* target) const = 0;
      virtual double getDistance(Position target) const = 0;
      virtual int getCurrentDirection() const = 0;

      virtual Unit* getTarget() const = 0;
      virtual Position getTargetPosition() const = 0;
      virtual Order getOrder() const = 0;
      virtual Unit* getOrderTarget() const = 0;
      virtual int getOrderTimer() const = 0;
      virtual Order getSecondaryOrder() const = 0;
      virtual Unit* getBuildUnit() const = 0;
      virtual int getRemainingBuildTime() const = 0;
      virtual int getRemainingTrainTime() const = 0;
      virtual Unit* getChild() const = 0;
      virtual std::list<UnitType > getTrainingQueue() const = 0;
      virtual Unit* getTransport() const = 0;
      virtual std::list<Unit*> getLoadedUnits() const = 0;
      virtual int getInterceptorCount() const = 0;
      virtual int getScarabCount() const = 0;
      virtual int getSpiderMineCount() const = 0;
      virtual TechType getTech() const = 0;
      virtual UpgradeType getUpgrade() const = 0;
      virtual int getRemainingResearchTime() const = 0;
      virtual int getRemainingUpgradeTime() const = 0;
      virtual Position getRallyPosition() const = 0;
      virtual Unit* getRallyUnit() const = 0;
      virtual Unit* getAddon() const = 0;

      virtual bool canAccess() const = 0;
      virtual bool exists() const = 0;
      virtual bool isAccelerating() const = 0;
      virtual bool isBeingConstructed() const = 0;
      virtual bool isBeingHealed() const = 0;
      virtual bool isBlind() const = 0;
      virtual bool isBraking() const = 0;
      virtual bool isBurrowed() const = 0;
      virtual bool isCarryingGas() const = 0;
      virtual bool isCarryingMinerals() const = 0;
      virtual bool isCloaked() const = 0;
      virtual bool isCompleted() const = 0;
      virtual bool isConstructing() const = 0;
      virtual bool isDefenseMatrixed() const = 0;
      virtual bool isEnsnared() const = 0;
      virtual bool isFollowing() const = 0;
      virtual bool isGatheringGas() const = 0;
      virtual bool isGatheringMinerals() const = 0;
      virtual bool isHallucination() const = 0;
      virtual bool isIdle() const = 0;
      virtual bool isIrradiated() const = 0;
      virtual bool isLifted() const = 0;
      virtual bool isLoaded() const = 0;
      virtual bool isLockedDown() const = 0;
      virtual bool isMaelstrommed() const = 0;
      virtual bool isMorphing() const = 0;
      virtual bool isMoving() const = 0;
      virtual bool isParasited() const = 0;
      virtual bool isPatrolling() const = 0;
      virtual bool isPlagued() const = 0;
      virtual bool isRepairing() const = 0;
      virtual bool isResearching() const = 0;
      virtual bool isSelected() const = 0;
      virtual bool isSieged() const = 0;
      virtual bool isStartingAttack() const = 0;
      virtual bool isStasised() const = 0;
      virtual bool isStimmed() const = 0;
      virtual bool isTraining() const = 0;
      virtual bool isUnderStorm() const = 0;
      virtual bool isUnpowered() const = 0;
      virtual bool isUpgrading() const = 0;
      virtual bool isVisible() const = 0;

      virtual bool attackMove(Position position) = 0;
      virtual bool attackUnit(Unit* target) = 0;
      virtual bool rightClick(Position position) = 0;
      virtual bool rightClick(Unit* target) = 0;
      virtual bool train(UnitType type) = 0;
      virtual bool build(TilePosition position, UnitType type) = 0;
      virtual bool buildAddon(UnitType type) = 0;
      virtual bool research(TechType tech) = 0;
      virtual bool upgrade(UpgradeType upgrade) = 0;
      virtual bool stop() = 0;
      virtual bool holdPosition() = 0;
      virtual bool patrol(Position position) = 0;
      virtual bool follow(Unit* target) = 0;
      virtual bool setRallyPosition(Position target) = 0;
      virtual bool setRallyUnit(Unit* target) = 0;
      virtual bool repair(Unit* target) = 0;
      virtual bool morph(UnitType type) = 0;
      virtual bool burrow() = 0;
      virtual bool unburrow() = 0;
      virtual bool siege() = 0;
      virtual bool unsiege() = 0;
      virtual bool cloak() = 0;
      virtual bool decloak() = 0;
      virtual bool lift() = 0;
      virtual bool land(TilePosition position) = 0;
      virtual bool load(Unit* target) = 0;
      virtual bool unload(Unit* target) = 0;
      virtual bool unloadAll() = 0;
      virtual bool unloadAll(Position position) = 0;
      virtual bool cancelConstruction() = 0;
      virtual bool haltConstruction() = 0;
      virtual bool cancelMorph() = 0;
      virtual bool cancelTrain() = 0;
      virtual bool cancelTrain(int slot) = 0;
      virtual bool cancelAddon() = 0;
      virtual bool cancelResearch() = 0;
      virtual bool cancelUpgrade() = 0;
      virtual bool useTech(TechType tech) = 0;
      virtual bool useTech(TechType tech, Position position) = 0;
      virtual bool useTech(TechType tech, Unit* target) = 0;
  };
}

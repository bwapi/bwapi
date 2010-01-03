#pragma once

#include "BWAPI2Interface.h"

IP_STRIP

#include "Order.h"
#include "TechType.h"
#include "UpgradeType.h"
#include "UnitType.h"
#include "Position.h"
#include "TilePosition.h"

#include <list>
#include <BWAPI\all.h>
namespace BWAPI2
{
  class Player;
  class Unit
  {
  private:
    const BWAPI::UnitState* self;
    int id;
  public:
    Unit(int id);
    BWAPI2_EXPORT int getID() const;
    BWAPI2_EXPORT Player* getPlayer() const;
    BWAPI2_EXPORT UnitType getType() const;
    BWAPI2_EXPORT UnitType getInitialType() const;
    BWAPI2_EXPORT int getHitPoints() const;
    BWAPI2_EXPORT int getInitialHitPoints() const;
    BWAPI2_EXPORT int getShields() const;
    BWAPI2_EXPORT int getEnergy() const;
    BWAPI2_EXPORT int getResources() const;
    BWAPI2_EXPORT int getInitialResources() const;
    BWAPI2_EXPORT int getKillCount() const;
    BWAPI2_EXPORT int getGroundWeaponCooldown() const;
    BWAPI2_EXPORT int getAirWeaponCooldown() const;
    BWAPI2_EXPORT int getSpellCooldown() const;
    BWAPI2_EXPORT int getDefenseMatrixPoints() const;

    BWAPI2_EXPORT int getDefenseMatrixTimer() const;
    BWAPI2_EXPORT int getEnsnareTimer() const;
    BWAPI2_EXPORT int getIrradiateTimer() const;
    BWAPI2_EXPORT int getLockdownTimer() const;
    BWAPI2_EXPORT int getMaelstromTimer() const;
    BWAPI2_EXPORT int getPlagueTimer() const;
    BWAPI2_EXPORT int getRemoveTimer() const;
    BWAPI2_EXPORT int getStasisTimer() const;
    BWAPI2_EXPORT int getStimTimer() const;

    BWAPI2_EXPORT Position getPosition() const;
    BWAPI2_EXPORT Position getInitialPosition() const;
    BWAPI2_EXPORT TilePosition getTilePosition() const;
    BWAPI2_EXPORT TilePosition getInitialTilePosition() const;
    BWAPI2_EXPORT double getDistance(Unit* target) const;
    BWAPI2_EXPORT double getDistance(Position target) const;
    BWAPI2_EXPORT double getAngle() const;
    BWAPI2_EXPORT double getVelocityX() const;
    BWAPI2_EXPORT double getVelocityY() const;

    BWAPI2_EXPORT Unit* getTarget() const;
    BWAPI2_EXPORT Position getTargetPosition() const;
    BWAPI2_EXPORT Order getOrder() const;
    BWAPI2_EXPORT Unit* getOrderTarget() const;
    BWAPI2_EXPORT int getOrderTimer() const;
    BWAPI2_EXPORT Order getSecondaryOrder() const;
    BWAPI2_EXPORT Unit* getBuildUnit() const;
    BWAPI2_EXPORT UnitType getBuildType() const;
    BWAPI2_EXPORT int getRemainingBuildTime() const;
    BWAPI2_EXPORT int getRemainingTrainTime() const;
    BWAPI2_EXPORT Unit* getChild() const;
    BWAPI2_EXPORT std::list<UnitType > getTrainingQueue() const;
    BWAPI2_EXPORT Unit* getTransport() const;
    BWAPI2_EXPORT std::list<Unit*> getLoadedUnits() const;
    BWAPI2_EXPORT int getInterceptorCount() const;
    BWAPI2_EXPORT int getScarabCount() const;
    BWAPI2_EXPORT int getSpiderMineCount() const;
    BWAPI2_EXPORT TechType getTech() const;
    BWAPI2_EXPORT UpgradeType getUpgrade() const;
    BWAPI2_EXPORT int getRemainingResearchTime() const;
    BWAPI2_EXPORT int getRemainingUpgradeTime() const;
    BWAPI2_EXPORT Position getRallyPosition() const;
    BWAPI2_EXPORT Unit* getRallyUnit() const;
    BWAPI2_EXPORT Unit* getAddon() const;
    BWAPI2_EXPORT Unit* getHatchery() const;
    BWAPI2_EXPORT std::set<Unit*> getLarva() const;
    BWAPI2_EXPORT int getUpgradeLevel(UpgradeType upgrade) const;

    BWAPI2_EXPORT bool exists() const;
    BWAPI2_EXPORT bool isAccelerating() const;
    BWAPI2_EXPORT bool isBeingConstructed() const;
    BWAPI2_EXPORT bool isBeingHealed() const;
    BWAPI2_EXPORT bool isBlind() const;
    BWAPI2_EXPORT bool isBraking() const;
    BWAPI2_EXPORT bool isBurrowed() const;
    BWAPI2_EXPORT bool isCarryingGas() const;
    BWAPI2_EXPORT bool isCarryingMinerals() const;
    BWAPI2_EXPORT bool isCloaked() const;
    BWAPI2_EXPORT bool isCompleted() const;
    BWAPI2_EXPORT bool isConstructing() const;
    BWAPI2_EXPORT bool isDefenseMatrixed() const;
    BWAPI2_EXPORT bool isEnsnared() const;
    BWAPI2_EXPORT bool isFollowing() const;
    BWAPI2_EXPORT bool isGatheringGas() const;
    BWAPI2_EXPORT bool isGatheringMinerals() const;
    BWAPI2_EXPORT bool isHallucination() const;
    BWAPI2_EXPORT bool isIdle() const;
    BWAPI2_EXPORT bool isIrradiated() const;
    BWAPI2_EXPORT bool isLifted() const;
    BWAPI2_EXPORT bool isLoaded() const;
    BWAPI2_EXPORT bool isLockedDown() const;
    BWAPI2_EXPORT bool isMaelstrommed() const;
    BWAPI2_EXPORT bool isMorphing() const;
    BWAPI2_EXPORT bool isMoving() const;
    BWAPI2_EXPORT bool isParasited() const;
    BWAPI2_EXPORT bool isPatrolling() const;
    BWAPI2_EXPORT bool isPlagued() const;
    BWAPI2_EXPORT bool isRepairing() const;
    BWAPI2_EXPORT bool isResearching() const;
    BWAPI2_EXPORT bool isSelected() const;
    BWAPI2_EXPORT bool isSieged() const;
    BWAPI2_EXPORT bool isStartingAttack() const;
    BWAPI2_EXPORT bool isStasised() const;
    BWAPI2_EXPORT bool isStimmed() const;
    BWAPI2_EXPORT bool isTraining() const;
    BWAPI2_EXPORT bool isUnderStorm() const;
    BWAPI2_EXPORT bool isUnpowered() const;
    BWAPI2_EXPORT bool isUpgrading() const;
    BWAPI2_EXPORT bool isVisible() const;
    BWAPI2_EXPORT bool isBeingGathered() const;

    BWAPI2_EXPORT bool attackMove(Position position);
    BWAPI2_EXPORT bool attackUnit(Unit* target);
    BWAPI2_EXPORT bool rightClick(Position position);
    BWAPI2_EXPORT bool rightClick(Unit* target);
    BWAPI2_EXPORT bool train(UnitType type);
    BWAPI2_EXPORT bool build(TilePosition position, UnitType type);
    BWAPI2_EXPORT bool buildAddon(UnitType type);
    BWAPI2_EXPORT bool research(TechType tech);
    BWAPI2_EXPORT bool upgrade(UpgradeType upgrade);
    BWAPI2_EXPORT bool stop();
    BWAPI2_EXPORT bool holdPosition();
    BWAPI2_EXPORT bool patrol(Position position);
    BWAPI2_EXPORT bool follow(Unit* target);
    BWAPI2_EXPORT bool setRallyPosition(Position target);
    BWAPI2_EXPORT bool setRallyUnit(Unit* target);
    BWAPI2_EXPORT bool repair(Unit* target);
    BWAPI2_EXPORT bool returnCargo();
    BWAPI2_EXPORT bool morph(UnitType type);
    BWAPI2_EXPORT bool burrow();
    BWAPI2_EXPORT bool unburrow();
    BWAPI2_EXPORT bool siege();
    BWAPI2_EXPORT bool unsiege();
    BWAPI2_EXPORT bool cloak();
    BWAPI2_EXPORT bool decloak();
    BWAPI2_EXPORT bool lift();
    BWAPI2_EXPORT bool land(TilePosition position);
    BWAPI2_EXPORT bool load(Unit* target);
    BWAPI2_EXPORT bool unload(Unit* target);
    BWAPI2_EXPORT bool unloadAll();
    BWAPI2_EXPORT bool unloadAll(Position position);
    BWAPI2_EXPORT bool cancelConstruction();
    BWAPI2_EXPORT bool haltConstruction();
    BWAPI2_EXPORT bool cancelMorph();
    BWAPI2_EXPORT bool cancelTrain();
    BWAPI2_EXPORT bool cancelTrain(int slot);
    BWAPI2_EXPORT bool cancelAddon();
    BWAPI2_EXPORT bool cancelResearch();
    BWAPI2_EXPORT bool cancelUpgrade();
    BWAPI2_EXPORT bool useTech(TechType tech);
    BWAPI2_EXPORT bool useTech(TechType tech, Position position);
    BWAPI2_EXPORT bool useTech(TechType tech, Unit* target);
  };
}

#pragma once

#include "BWSLInterface.h"

IP_STRIP

#include "Order.h"
#include "TechType.h"
#include "UpgradeType.h"
#include "UnitType.h"
#include "Position.h"
#include "TilePosition.h"

#include <list>
#include <BWAPI\all.h>
namespace BWSL
{
  class Player;
  class Unit
  {
  private:
    const BWAPI::UnitState* self;
    int id;
    bool alive;
    UnitType initialType;
    int initialResources;
    int initialHitPoints;
    Position initialPosition;
  public:
    bool startingAttack;
    int lastGroundWeaponCooldown;
    int lastAirWeaponCooldown;
    std::set<Unit*> larva;
    void saveInitialState();


    Unit(int id);
    BWSL_EXPORT int getID() const;
    BWSL_EXPORT Player* getPlayer() const;
    BWSL_EXPORT UnitType getType() const;
    BWSL_EXPORT UnitType getInitialType() const;
    BWSL_EXPORT int getHitPoints() const;
    BWSL_EXPORT int getInitialHitPoints() const;
    BWSL_EXPORT int getShields() const;
    BWSL_EXPORT int getEnergy() const;
    BWSL_EXPORT int getResources() const;
    BWSL_EXPORT int getInitialResources() const;
    BWSL_EXPORT int getKillCount() const;
    BWSL_EXPORT int getGroundWeaponCooldown() const;
    BWSL_EXPORT int getAirWeaponCooldown() const;
    BWSL_EXPORT int getSpellCooldown() const;
    BWSL_EXPORT int getDefenseMatrixPoints() const;

    BWSL_EXPORT int getDefenseMatrixTimer() const;
    BWSL_EXPORT int getEnsnareTimer() const;
    BWSL_EXPORT int getIrradiateTimer() const;
    BWSL_EXPORT int getLockdownTimer() const;
    BWSL_EXPORT int getMaelstromTimer() const;
    BWSL_EXPORT int getPlagueTimer() const;
    BWSL_EXPORT int getRemoveTimer() const;
    BWSL_EXPORT int getStasisTimer() const;
    BWSL_EXPORT int getStimTimer() const;

    BWSL_EXPORT Position getPosition() const;
    BWSL_EXPORT Position getInitialPosition() const;
    BWSL_EXPORT TilePosition getTilePosition() const;
    BWSL_EXPORT TilePosition getInitialTilePosition() const;
    BWSL_EXPORT double getDistance(Unit* target) const;
    BWSL_EXPORT double getDistance(Position target) const;
    BWSL_EXPORT double getAngle() const;
    BWSL_EXPORT double getVelocityX() const;
    BWSL_EXPORT double getVelocityY() const;

    BWSL_EXPORT Unit* getTarget() const;
    BWSL_EXPORT Position getTargetPosition() const;
    BWSL_EXPORT Order getOrder() const;
    BWSL_EXPORT Unit* getOrderTarget() const;
    BWSL_EXPORT int getOrderTimer() const;
    BWSL_EXPORT Order getSecondaryOrder() const;
    BWSL_EXPORT Unit* getBuildUnit() const;
    BWSL_EXPORT UnitType getBuildType() const;
    BWSL_EXPORT int getRemainingBuildTime() const;
    BWSL_EXPORT int getRemainingTrainTime() const;
    BWSL_EXPORT Unit* getChild() const;
    BWSL_EXPORT std::list<UnitType > getTrainingQueue() const;
    BWSL_EXPORT Unit* getTransport() const;
    BWSL_EXPORT std::list<Unit*> getLoadedUnits() const;
    BWSL_EXPORT int getInterceptorCount() const;
    BWSL_EXPORT int getScarabCount() const;
    BWSL_EXPORT int getSpiderMineCount() const;
    BWSL_EXPORT TechType getTech() const;
    BWSL_EXPORT UpgradeType getUpgrade() const;
    BWSL_EXPORT int getRemainingResearchTime() const;
    BWSL_EXPORT int getRemainingUpgradeTime() const;
    BWSL_EXPORT Position getRallyPosition() const;
    BWSL_EXPORT Unit* getRallyUnit() const;
    BWSL_EXPORT Unit* getAddon() const;
    BWSL_EXPORT Unit* getHatchery() const;
    BWSL_EXPORT std::set<Unit*> getLarva() const;
    BWSL_EXPORT int getUpgradeLevel(UpgradeType upgrade) const;

    BWSL_EXPORT bool exists() const;
    BWSL_EXPORT bool isAccelerating() const;
    BWSL_EXPORT bool isBeingConstructed() const;
    BWSL_EXPORT bool isBeingGathered() const;
    BWSL_EXPORT bool isBeingHealed() const;
    BWSL_EXPORT bool isBlind() const;
    BWSL_EXPORT bool isBraking() const;
    BWSL_EXPORT bool isBurrowed() const;
    BWSL_EXPORT bool isCarryingGas() const;
    BWSL_EXPORT bool isCarryingMinerals() const;
    BWSL_EXPORT bool isCloaked() const;
    BWSL_EXPORT bool isCompleted() const;
    BWSL_EXPORT bool isConstructing() const;
    BWSL_EXPORT bool isDefenseMatrixed() const;
    BWSL_EXPORT bool isEnsnared() const;
    BWSL_EXPORT bool isFollowing() const;
    BWSL_EXPORT bool isGatheringGas() const;
    BWSL_EXPORT bool isGatheringMinerals() const;
    BWSL_EXPORT bool isHallucination() const;
    BWSL_EXPORT bool isIdle() const;
    BWSL_EXPORT bool isIrradiated() const;
    BWSL_EXPORT bool isLifted() const;
    BWSL_EXPORT bool isLoaded() const;
    BWSL_EXPORT bool isLockedDown() const;
    BWSL_EXPORT bool isMaelstrommed() const;
    BWSL_EXPORT bool isMorphing() const;
    BWSL_EXPORT bool isMoving() const;
    BWSL_EXPORT bool isParasited() const;
    BWSL_EXPORT bool isPatrolling() const;
    BWSL_EXPORT bool isPlagued() const;
    BWSL_EXPORT bool isRepairing() const;
    BWSL_EXPORT bool isResearching() const;
    BWSL_EXPORT bool isSelected() const;
    BWSL_EXPORT bool isSieged() const;
    BWSL_EXPORT bool isStartingAttack() const;
    BWSL_EXPORT bool isStasised() const;
    BWSL_EXPORT bool isStimmed() const;
    BWSL_EXPORT bool isTraining() const;
    BWSL_EXPORT bool isUnderStorm() const;
    BWSL_EXPORT bool isUnpowered() const;
    BWSL_EXPORT bool isUpgrading() const;
    BWSL_EXPORT bool isVisible() const;

    BWSL_EXPORT bool attackMove(Position position);
    BWSL_EXPORT bool attackUnit(Unit* target);
    BWSL_EXPORT bool rightClick(Position position);
    BWSL_EXPORT bool rightClick(Unit* target);
    BWSL_EXPORT bool train(UnitType type);
    BWSL_EXPORT bool build(TilePosition position, UnitType type);
    BWSL_EXPORT bool buildAddon(UnitType type);
    BWSL_EXPORT bool research(TechType tech);
    BWSL_EXPORT bool upgrade(UpgradeType upgrade);
    BWSL_EXPORT bool stop();
    BWSL_EXPORT bool holdPosition();
    BWSL_EXPORT bool patrol(Position position);
    BWSL_EXPORT bool follow(Unit* target);
    BWSL_EXPORT bool setRallyPosition(Position target);
    BWSL_EXPORT bool setRallyUnit(Unit* target);
    BWSL_EXPORT bool repair(Unit* target);
    BWSL_EXPORT bool returnCargo();
    BWSL_EXPORT bool morph(UnitType type);
    BWSL_EXPORT bool burrow();
    BWSL_EXPORT bool unburrow();
    BWSL_EXPORT bool siege();
    BWSL_EXPORT bool unsiege();
    BWSL_EXPORT bool cloak();
    BWSL_EXPORT bool decloak();
    BWSL_EXPORT bool lift();
    BWSL_EXPORT bool land(TilePosition position);
    BWSL_EXPORT bool load(Unit* target);
    BWSL_EXPORT bool unload(Unit* target);
    BWSL_EXPORT bool unloadAll();
    BWSL_EXPORT bool unloadAll(Position position);
    BWSL_EXPORT bool cancelConstruction();
    BWSL_EXPORT bool haltConstruction();
    BWSL_EXPORT bool cancelMorph();
    BWSL_EXPORT bool cancelTrain();
    BWSL_EXPORT bool cancelTrain(int slot);
    BWSL_EXPORT bool cancelAddon();
    BWSL_EXPORT bool cancelResearch();
    BWSL_EXPORT bool cancelUpgrade();
    BWSL_EXPORT bool useTech(TechType tech);
    BWSL_EXPORT bool useTech(TechType tech, Position position);
    BWSL_EXPORT bool useTech(TechType tech, Unit* target);
  };
}

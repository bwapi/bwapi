#pragma once
#include "BWAgentInterface.h"
#include <BWAPITypes\UnitStruct.h>

namespace BWAgent
{
  class Unit
  {
  public:
    Unit();
    AGENT_INTERFACE int getID() const;
    AGENT_INTERFACE int getPlayer() const;
    AGENT_INTERFACE int getType() const;
    AGENT_INTERFACE int getInitialType() const;
    AGENT_INTERFACE int getHitPoints() const;
    AGENT_INTERFACE int getInitialHitPoints() const;
    AGENT_INTERFACE int getShields() const;
    AGENT_INTERFACE int getEnergy() const;
    AGENT_INTERFACE int getResources() const;
    AGENT_INTERFACE int getInitialResources() const;
    AGENT_INTERFACE int getKillCount() const;
    AGENT_INTERFACE int getGroundWeaponCooldown() const;
    AGENT_INTERFACE int getAirWeaponCooldown() const;
    AGENT_INTERFACE int getSpellCooldown() const;
    AGENT_INTERFACE int getDefenseMatrixPoints() const;

    AGENT_INTERFACE int getDefenseMatrixTimer() const;
    AGENT_INTERFACE int getEnsnareTimer() const;
    AGENT_INTERFACE int getIrradiateTimer() const;
    AGENT_INTERFACE int getLockdownTimer() const;
    AGENT_INTERFACE int getMaelstromTimer() const;
    AGENT_INTERFACE int getPlagueTimer() const;
    AGENT_INTERFACE int getRemoveTimer() const;
    AGENT_INTERFACE int getStasisTimer() const;
    AGENT_INTERFACE int getStimTimer() const;

/*
    Position getPosition() const;
    Position getInitialPosition() const;
    TilePosition getTilePosition() const;
    TilePosition getInitialTilePosition() const;
    double getDistance(Unit* target) const;
    double getDistance(Position target) const;
    double getAngle() const;
    double getVelocityX() const;
    double getVelocityY() const;

    Unit* getTarget() const;
    Position getTargetPosition() const;
    Order getOrder() const;
    Unit* getOrderTarget() const;
    int getOrderTimer() const;
    Order getSecondaryOrder() const;
    Unit* getBuildUnit() const;
    int getRemainingBuildTime() const;
    int getRemainingTrainTime() const;
    Unit* getChild() const;
    std::list<UnitType > getTrainingQueue() const;
    Unit* getTransport() const;
    std::list<Unit*> getLoadedUnits() const;
    int getInterceptorCount() const;
    int getScarabCount() const;
    int getSpiderMineCount() const;
    TechType getTech() const;
    UpgradeType getUpgrade() const;
    int getRemainingResearchTime() const;
    int getRemainingUpgradeTime() const;
    Position getRallyPosition() const;
    Unit* getRallyUnit() const;
    Unit* getAddon() const;
    int getUpgradeLevel(UpgradeType upgrade) const;
*/
    AGENT_INTERFACE bool exists() const;
    AGENT_INTERFACE bool isAccelerating() const;
    AGENT_INTERFACE bool isBeingConstructed() const;
    AGENT_INTERFACE bool isBeingGathered() const;
    AGENT_INTERFACE bool isBeingHealed() const;
    AGENT_INTERFACE bool isBlind() const;
    AGENT_INTERFACE bool isBraking() const;
    AGENT_INTERFACE bool isBurrowed() const;
    AGENT_INTERFACE bool isCarryingGas() const;
    AGENT_INTERFACE bool isCarryingMinerals() const;
    AGENT_INTERFACE bool isCloaked() const;
    AGENT_INTERFACE bool isCompleted() const;
    AGENT_INTERFACE bool isConstructing() const;
    AGENT_INTERFACE bool isDefenseMatrixed() const;
    AGENT_INTERFACE bool isEnsnared() const;
    AGENT_INTERFACE bool isFollowing() const;
    AGENT_INTERFACE bool isGatheringGas() const;
    AGENT_INTERFACE bool isGatheringMinerals() const;
    AGENT_INTERFACE bool isHallucination() const;
    AGENT_INTERFACE bool isIdle() const;
    AGENT_INTERFACE bool isIrradiated() const;
    AGENT_INTERFACE bool isLifted() const;
    AGENT_INTERFACE bool isLoaded() const;
    AGENT_INTERFACE bool isLockedDown() const;
    AGENT_INTERFACE bool isMaelstrommed() const;
    AGENT_INTERFACE bool isMorphing() const;
    AGENT_INTERFACE bool isMoving() const;
    AGENT_INTERFACE bool isParasited() const;
    AGENT_INTERFACE bool isPatrolling() const;
    AGENT_INTERFACE bool isPlagued() const;
    AGENT_INTERFACE bool isRepairing() const;
    AGENT_INTERFACE bool isResearching() const;
    AGENT_INTERFACE bool isSelected() const;
    AGENT_INTERFACE bool isSieged() const;
    AGENT_INTERFACE bool isStartingAttack() const;
    AGENT_INTERFACE bool isStasised() const;
    AGENT_INTERFACE bool isStimmed() const;
    AGENT_INTERFACE bool isTraining() const;
    AGENT_INTERFACE bool isUnderStorm() const;
    AGENT_INTERFACE bool isUnpowered() const;
    AGENT_INTERFACE bool isUpgrading() const;
    AGENT_INTERFACE bool isVisible() const;

//    bool attackMove(Position position);
    AGENT_INTERFACE bool attackUnit(Unit* target);
//    bool rightClick(Position position);
    AGENT_INTERFACE bool rightClick(Unit* target);
//    bool train(UnitType type);
//    bool build(TilePosition position, UnitType type);
//    bool buildAddon(UnitType type);
//    bool research(TechType tech);
//    bool upgrade(UpgradeType upgrade);
    AGENT_INTERFACE bool stop();
    AGENT_INTERFACE bool holdPosition();
//    bool patrol(Position position);
    AGENT_INTERFACE bool follow(Unit* target);
//    bool setRallyPosition(Position target);
    AGENT_INTERFACE bool setRallyUnit(Unit* target);
    AGENT_INTERFACE bool repair(Unit* target);
//    bool morph(UnitType type);
    AGENT_INTERFACE bool burrow();
    AGENT_INTERFACE bool unburrow();
    AGENT_INTERFACE bool siege();
    AGENT_INTERFACE bool unsiege();
    AGENT_INTERFACE bool cloak();
    AGENT_INTERFACE bool decloak();
    AGENT_INTERFACE bool lift();
//    bool land(TilePosition position);
    AGENT_INTERFACE bool load(Unit* target);
    AGENT_INTERFACE bool unload(Unit* target);
    AGENT_INTERFACE bool unloadAll();
//    bool unloadAll(Position position);
    AGENT_INTERFACE bool cancelConstruction();
    AGENT_INTERFACE bool haltConstruction();
    AGENT_INTERFACE bool cancelMorph();
    AGENT_INTERFACE bool cancelTrain();
    AGENT_INTERFACE bool cancelTrain(int slot);
    AGENT_INTERFACE bool cancelAddon();
    AGENT_INTERFACE bool cancelResearch();
    AGENT_INTERFACE bool cancelUpgrade();
//    bool useTech(TechType tech);
//    bool useTech(TechType tech, Position position);
//    bool useTech(TechType tech, Unit* target);
    void _update(BWAPI::ClearanceLevels level,BWAPI::State* data);
  private:
    BWAPI::ClearanceLevels level;//type of struct that data points to
    BWAPI::State* data;
  };
}

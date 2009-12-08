#pragma once
#include "BWAPI2Interface.h"


IP_IGNORE;   // tell IP to ignore lines untill AGENT_STRIP or AGENT_COPY

// this include files will be ignored in export, because it is only
// needed for private functions, which will not be exported
#include <Bridge\KnownUnitEntry.h>

IP_STRIP;	// tell IP to strip all that's not BWAPI2_METHOD declared

namespace BWAgent
{
  class Unit
  {
  public:
    Unit();
    BWAPI2_METHOD int getID() const;
    BWAPI2_METHOD int getPlayer() const;
    BWAPI2_METHOD int getType() const;
    BWAPI2_METHOD int getHitPoints() const;
    BWAPI2_METHOD int getShields() const;
    BWAPI2_METHOD int getEnergy() const;
    BWAPI2_METHOD int getResources() const;
    BWAPI2_METHOD int getKillCount() const;
    BWAPI2_METHOD int getGroundWeaponCooldown() const;
    BWAPI2_METHOD int getAirWeaponCooldown() const;
    BWAPI2_METHOD int getSpellCooldown() const;
    BWAPI2_METHOD int getDefenseMatrixPoints() const;

    BWAPI2_METHOD int getDefenseMatrixTimer() const;
    BWAPI2_METHOD int getEnsnareTimer() const;
    BWAPI2_METHOD int getIrradiateTimer() const;
    BWAPI2_METHOD int getLockdownTimer() const;
    BWAPI2_METHOD int getMaelstromTimer() const;
    BWAPI2_METHOD int getPlagueTimer() const;
    BWAPI2_METHOD int getRemoveTimer() const;
    BWAPI2_METHOD int getStasisTimer() const;
    BWAPI2_METHOD int getStimTimer() const;

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
    BWAPI2_METHOD bool isAccelerating() const;
    BWAPI2_METHOD bool isBeingConstructed() const;
    BWAPI2_METHOD bool isBeingGathered() const;
    BWAPI2_METHOD bool isBeingHealed() const;
    BWAPI2_METHOD bool isBlind() const;
    BWAPI2_METHOD bool isBraking() const;
    BWAPI2_METHOD bool isBurrowed() const;
    BWAPI2_METHOD bool isCarryingGas() const;
    BWAPI2_METHOD bool isCarryingMinerals() const;
    BWAPI2_METHOD bool isCloaked() const;
    BWAPI2_METHOD bool isCompleted() const;
    BWAPI2_METHOD bool isConstructing() const;
    BWAPI2_METHOD bool isDefenseMatrixed() const;
    BWAPI2_METHOD bool isEnsnared() const;
    BWAPI2_METHOD bool isFollowing() const;
    BWAPI2_METHOD bool isGatheringGas() const;
    BWAPI2_METHOD bool isGatheringMinerals() const;
    BWAPI2_METHOD bool isHallucination() const;
    BWAPI2_METHOD bool isIdle() const;
    BWAPI2_METHOD bool isIrradiated() const;
    BWAPI2_METHOD bool isLifted() const;
    BWAPI2_METHOD bool isLoaded() const;
    BWAPI2_METHOD bool isLockedDown() const;
    BWAPI2_METHOD bool isMaelstrommed() const;
    BWAPI2_METHOD bool isMorphing() const;
    BWAPI2_METHOD bool isMoving() const;
    BWAPI2_METHOD bool isParasited() const;
    BWAPI2_METHOD bool isPatrolling() const;
    BWAPI2_METHOD bool isPlagued() const;
    BWAPI2_METHOD bool isRepairing() const;
    BWAPI2_METHOD bool isResearching() const;
    BWAPI2_METHOD bool isSelected() const;
    BWAPI2_METHOD bool isSieged() const;
    BWAPI2_METHOD bool isStartingAttack() const;
    BWAPI2_METHOD bool isStasised() const;
    BWAPI2_METHOD bool isStimmed() const;
    BWAPI2_METHOD bool isTraining() const;
    BWAPI2_METHOD bool isUnderStorm() const;
    BWAPI2_METHOD bool isUnpowered() const;
    BWAPI2_METHOD bool isUpgrading() const;
    BWAPI2_METHOD bool isVisible() const;

//    bool attackMove(Position position);
    BWAPI2_METHOD bool attackUnit(Unit* target);
//    bool rightClick(Position position);
    BWAPI2_METHOD bool rightClick(Unit* target);
//    bool train(UnitType type);
//    bool build(TilePosition position, UnitType type);
//    bool buildAddon(UnitType type);
//    bool research(TechType tech);
//    bool upgrade(UpgradeType upgrade);
    BWAPI2_METHOD bool stop();
    BWAPI2_METHOD bool holdPosition();
//    bool patrol(Position position);
    BWAPI2_METHOD bool follow(Unit* target);
//    bool setRallyPosition(Position target);
    BWAPI2_METHOD bool setRallyUnit(Unit* target);
    BWAPI2_METHOD bool repair(Unit* target);
//    bool morph(UnitType type);
    BWAPI2_METHOD bool burrow();
    BWAPI2_METHOD bool unburrow();
    BWAPI2_METHOD bool siege();
    BWAPI2_METHOD bool unsiege();
    BWAPI2_METHOD bool cloak();
    BWAPI2_METHOD bool decloak();
    BWAPI2_METHOD bool lift();
//    bool land(TilePosition position);
    BWAPI2_METHOD bool load(Unit* target);
    BWAPI2_METHOD bool unload(Unit* target);
    BWAPI2_METHOD bool unloadAll();
//    bool unloadAll(Position position);
    BWAPI2_METHOD bool cancelConstruction();
    BWAPI2_METHOD bool haltConstruction();
    BWAPI2_METHOD bool cancelMorph();
    BWAPI2_METHOD bool cancelTrain();
    BWAPI2_METHOD bool cancelTrain(int slot);
    BWAPI2_METHOD bool cancelAddon();
    BWAPI2_METHOD bool cancelResearch();
    BWAPI2_METHOD bool cancelUpgrade();
//    bool useTech(TechType tech);
//    bool useTech(TechType tech, Position position);
//    bool useTech(TechType tech, Unit* target);
  private:
    Bridge::KnownUnitEntry *knownUnit;  // if it's NULL, the unit does not exist anymore

    IP_IGNORE;   // ignore templates, stripping templates does not work yet in IP
    // helpers
    template<typename T>
      const T &getRespectClearance(T BWAPI::UnitState::*var) const
      {
        static T null = 0;
        if(!this->knownUnit)
          return null; // TODO: set last error
        // TODO: check if clearance level clears
        return this->knownUnit->state.*var;
      }
    IP_STRIP;
  };
}

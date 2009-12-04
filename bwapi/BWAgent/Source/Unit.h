#pragma once
#include "BWAgentInterface.h"
#include <BWAPITypes\UnitStruct.h>

namespace BWAgent
{
  class AGENT_API Unit
  {
  public:
    Unit();
    int getID();
    int getType();
    int getHitPoints();

    bool exists() const;
    bool isAccelerating() const;
    bool isBeingConstructed() const;
    bool isBeingGathered() const;
    bool isBeingHealed() const;
    bool isBlind() const;
    bool isBraking() const;
    bool isBurrowed() const;
    bool isCarryingGas() const;
    bool isCarryingMinerals() const;
    bool isCloaked() const;
    bool isCompleted() const;
    bool isConstructing() const;
    bool isDefenseMatrixed() const;
    bool isEnsnared() const;
    bool isFollowing() const;
    bool isGatheringGas() const;
    bool isGatheringMinerals() const;
    bool isHallucination() const;
    bool isIdle() const;
    bool isIrradiated() const;
    bool isLifted() const;
    bool isLoaded() const;
    bool isLockedDown() const;
    bool isMaelstrommed() const;
    bool isMorphing() const;
    bool isMoving() const;
    bool isParasited() const;
    bool isPatrolling() const;
    bool isPlagued() const;
    bool isRepairing() const;
    bool isResearching() const;
    bool isSelected() const;
    bool isSieged() const;
    bool isStartingAttack() const;
    bool isStasised() const;
    bool isStimmed() const;
    bool isTraining() const;
    bool isUnderStorm() const;
    bool isUnpowered() const;
    bool isUpgrading() const;
    bool isVisible() const;

    bool attackUnit(Unit* target);
    bool stop();
    bool holdPosition();
    bool burrow();
    bool unburrow();
    bool siege();
    bool unsiege();
    bool cloak();
    bool decloak();
    bool lift();
    bool cancelConstruction();
    bool haltConstruction();
    bool cancelMorph();
    bool cancelTrain();
    bool cancelTrain(int slot);
    bool cancelAddon();
    bool cancelResearch();
    bool cancelUpgrade();
    void _update(BWAPI::ClearanceLevels level,BWAPI::State* data);
  private:
    BWAPI::ClearanceLevels level;//type of struct that data points to
    BWAPI::State* data;
  };
}

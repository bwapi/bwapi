#pragma once
#include <BWAPI.h>
#include "UnitData.h"
#include <set>
#include <string>

namespace BWAPI
{
  class Player;
  class UnitImpl : public Unit
  {
    private:
      const BWAPIC::UnitData* self;
      int id;
      UnitType initialType;
      int initialResources;
      int initialHitPoints;
      Position initialPosition;
    public:
      std::set<Unit*> larva;
      void clear();
      void saveInitialState();

      UnitImpl(int id);
      int getID() const;
      std::string getName() const;
      Player* getPlayer() const;
      UnitType getType() const;
      UnitType getInitialType() const;
      int getHitPoints() const;
      int maxHitPoints() const;
      int getInitialHitPoints() const;
      int getShields() const;
      int maxShields() const;
      int getEnergy() const;
      int maxEnergy() const;
      int getResources() const;
      int getInitialResources() const;
      int getKillCount() const;
      int getGroundWeaponCooldown() const;
      int getAirWeaponCooldown() const;
      int getSpellCooldown() const;
      int getDefenseMatrixPoints() const;

      int getDefenseMatrixTimer() const;
      int getEnsnareTimer() const;
      int getIrradiateTimer() const;
      int getLockdownTimer() const;
      int getMaelstromTimer() const;
      int getPlagueTimer() const;
      int getRemoveTimer() const;
      int getStasisTimer() const;
      int getStimTimer() const;

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
      UnitType getBuildType() const;
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
      Unit* getHatchery() const;
      std::set<Unit*> getLarva() const;
      int getUpgradeLevel(UpgradeType upgrade) const;

      bool exists() const;
      bool isAccelerating() const;
      bool isAttacking() const;
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
      bool isVisible(Player* player) const;

      bool issueCommand(UnitCommand command);
      bool attackMove(Position target);
      bool attackUnit(Unit* target);
      bool rightClick(Position target);
      bool rightClick(Unit* target);
      bool train(UnitType type);
      bool build(TilePosition target, UnitType type);
      bool buildAddon(UnitType type);
      bool research(TechType tech);
      bool upgrade(UpgradeType upgrade);
      bool stop();
      bool holdPosition();
      bool patrol(Position target);
      bool follow(Unit* target);
      bool setRallyPosition(Position target);
      bool setRallyUnit(Unit* target);
      bool repair(Unit* target);
      bool returnCargo();
      bool morph(UnitType type);
      bool burrow();
      bool unburrow();
      bool siege();
      bool unsiege();
      bool cloak();
      bool decloak();
      bool lift();
      bool land(TilePosition target);
      bool load(Unit* target);
      bool unload(Unit* target);
      bool unloadAll();
      bool unloadAll(Position target);
      bool cancelConstruction();
      bool haltConstruction();
      bool cancelMorph();
      bool cancelTrain();
      bool cancelTrain(int slot);
      bool cancelAddon();
      bool cancelResearch();
      bool cancelUpgrade();
      bool useTech(TechType tech);
      bool useTech(TechType tech, Position target);
      bool useTech(TechType tech, Unit* target);
  };
}

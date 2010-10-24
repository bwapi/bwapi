#pragma once

#include "BWAPI/Unit.h"

#include <list>

#include <Util/Types.h>
#include <BWAPI/Order.h>
#include <BW/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UnitCommand.h>
#include <BW/TilePosition.h>
#include <BW/OrderID.h>
#include <BWAPI/Client/UnitData.h>

#define checkAccessBool() if (!attemptAccess()) return false
#define checkOwnership()\
{\
  if (_getPlayer != Broodwar->self())\
    return BroodwarImpl.setLastError(Errors::Unit_Not_Owned);\
}

namespace Util  { class Logger; }
namespace BW    { class Position; };
namespace BW    { struct Unit; };
namespace BWAPI { class UnitPrototype;  };
namespace BWAPI { class AbilityPrototype;  };
namespace BWAPI { class Player;  };

namespace BWAPI
{
  /**
   * Interface for broodwar unit, can be used to obtain any information and
   * issue commands.
   */
  class UnitImpl : public Unit
  {
    public:

      virtual int          getID() const;
      virtual Player*      getPlayer() const;
      virtual UnitType     getType() const;
      virtual Position     getPosition() const;
      virtual TilePosition getTilePosition() const;
      virtual double       getAngle() const;
      virtual double       getVelocityX() const;
      virtual double       getVelocityY() const;
      virtual int          getHitPoints() const;
      virtual int          getShields() const;
      virtual int          getEnergy() const;
      virtual int          getResources() const;
      virtual int          getResourceGroup() const;

      virtual double getDistance(Unit* target) const;
      virtual double getDistance(Position target) const;
      virtual bool   hasPath(Unit* target) const;
      virtual bool   hasPath(Position target) const;
      virtual int    getLastOrderFrame() const;
      virtual int    getUpgradeLevel(UpgradeType upgrade) const;

      virtual UnitType     getInitialType() const;
      virtual Position     getInitialPosition() const;
      virtual TilePosition getInitialTilePosition() const;
      virtual int          getInitialHitPoints() const;
      virtual int          getInitialResources() const;

      virtual int getKillCount() const;
      virtual int getInterceptorCount() const;
      virtual int getScarabCount() const;
      virtual int getSpiderMineCount() const;
      virtual int getGroundWeaponCooldown() const;
      virtual int getAirWeaponCooldown() const;
      virtual int getSpellCooldown() const;
      virtual int getDefenseMatrixPoints() const;

      virtual int getDefenseMatrixTimer() const;
      virtual int getEnsnareTimer() const;
      virtual int getIrradiateTimer() const;
      virtual int getLockdownTimer() const;
      virtual int getMaelstromTimer() const;
      virtual int getOrderTimer() const;
      virtual int getPlagueTimer() const;
      virtual int getRemoveTimer() const;
      virtual int getStasisTimer() const;
      virtual int getStimTimer() const;

      virtual UnitType            getBuildType() const;
      virtual std::list<UnitType> getTrainingQueue() const;
      virtual TechType            getTech() const;
      virtual UpgradeType         getUpgrade() const;
      virtual int                 getRemainingBuildTime() const;
      virtual int                 getRemainingTrainTime() const;
      virtual int                 getRemainingResearchTime() const;
      virtual int                 getRemainingUpgradeTime() const;
      virtual Unit*               getBuildUnit() const;

      virtual Unit*    getTarget() const;
      virtual Position getTargetPosition() const;
      virtual Order    getOrder() const;
      virtual Unit*    getOrderTarget() const;
      virtual Order    getSecondaryOrder() const;
      virtual Position getRallyPosition() const;
      virtual Unit*    getRallyUnit() const;
      virtual Unit*    getAddon() const;
      virtual Unit*    getNydusExit() const;
      virtual Unit*    getPowerUp() const;

      virtual Unit*           getTransport() const;
      virtual std::set<Unit*> getLoadedUnits() const;
      virtual Unit*           getCarrier() const;
      virtual std::set<Unit*> getInterceptors() const;
      virtual Unit*           getHatchery() const;
      virtual std::set<Unit*> getLarva() const;

      virtual bool exists() const;
      virtual bool hasNuke() const;
      virtual bool isAccelerating() const;
      virtual bool isAttacking() const;
      virtual bool isBeingConstructed() const;
      virtual bool isBeingGathered() const;
      virtual bool isBeingHealed() const;
      virtual bool isBlind() const;
      virtual bool isBraking() const;
      virtual bool isBurrowed() const;
      virtual bool isCarryingGas() const;
      virtual bool isCarryingMinerals() const;
      virtual bool isCloaked() const;
      virtual bool isCompleted() const;
      virtual bool isConstructing() const;
      virtual bool isDefenseMatrixed() const;
      virtual bool isDetected() const;
      virtual bool isEnsnared() const;
      virtual bool isFollowing() const;
      virtual bool isGatheringGas() const;
      virtual bool isGatheringMinerals() const;
      virtual bool isHallucination() const;
      virtual bool isHoldingPosition() const;
      virtual bool isIdle() const;
      virtual bool isInterruptible() const;
      virtual bool isIrradiated() const;
      virtual bool isLifted() const;
      virtual bool isLoaded() const;
      virtual bool isLockedDown() const;
      virtual bool isMaelstrommed() const;
      virtual bool isMorphing() const;
      virtual bool isMoving() const;
      virtual bool isParasited() const;
      virtual bool isPatrolling() const;
      virtual bool isPlagued() const;
      virtual bool isRepairing() const;
      virtual bool isResearching() const;
      virtual bool isSelected() const;
      virtual bool isSieged() const;
      virtual bool isStartingAttack() const;
      virtual bool isStartingAttackSequence() const;
      virtual bool isStasised() const;
      virtual bool isStimmed() const;
      virtual bool isTraining() const;
      virtual bool isUnderStorm() const;
      virtual bool isUnpowered() const;
      virtual bool isUpgrading() const;
      virtual bool isVisible() const;
      virtual bool isVisible(Player* player) const;

      virtual bool issueCommand(UnitCommand command);

      virtual bool attackMove(Position target);
      virtual bool attackUnit(Unit* target);
      virtual bool build(TilePosition target, UnitType type);
      virtual bool buildAddon(UnitType type);
      virtual bool train(UnitType type);
      virtual bool morph(UnitType type);
      virtual bool research(TechType tech);
      virtual bool upgrade(UpgradeType upgrade);
      virtual bool setRallyPosition(Position target);
      virtual bool setRallyUnit(Unit* target);
      virtual bool move(Position target);
      virtual bool patrol(Position target);
      virtual bool holdPosition();
      virtual bool stop();
      virtual bool follow(Unit* target);
      virtual bool gather(Unit* target);
      virtual bool returnCargo();
      virtual bool repair(Unit* target);
      virtual bool burrow();
      virtual bool unburrow();
      virtual bool cloak();
      virtual bool decloak();
      virtual bool siege();
      virtual bool unsiege();
      virtual bool lift();
      virtual bool land(TilePosition target);
      virtual bool load(Unit* target);
      virtual bool unload(Unit* target);
      virtual bool unloadAll();
      virtual bool unloadAll(Position target);
      virtual bool rightClick(Position target);
      virtual bool rightClick(Unit* target);
      virtual bool haltConstruction();
      virtual bool cancelConstruction();
      virtual bool cancelAddon();
      virtual bool cancelTrain();
      virtual bool cancelTrain(int slot);
      virtual bool cancelMorph();
      virtual bool cancelResearch();
      virtual bool cancelUpgrade();
      virtual bool useTech(TechType tech);
      virtual bool useTech(TechType tech, Position target);
      virtual bool useTech(TechType tech, Unit* target);

      virtual void setClientInfo(void* clientinfo);
      virtual void *getClientInfo() const;

      //Internal BWAPI commands:
      UnitImpl(BW::Unit* originalUnit,
               u16 index);
      ~UnitImpl();
      static UnitImpl* BWUnitToBWAPIUnit(BW::Unit* unit);
      void die();
      void setID(int newID);
      bool canAccess() const;
      bool canAccessDetected() const;
      bool canAccessInside() const;
      bool attemptAccess() const;
      Player* _getPlayer;
      UnitType _getType;
      Position _getPosition;
      int _getResources;
      int _getHitPoints;
      bool _isCompleted;
      Unit* _getTransport;

      /**
       * Gets index of the unit in the unit array. Note that the index is same
       * for the original unit array, BWAPI::Unit array and BWAI::Unit array, so
       * it is good way to compare units of different types and for conversion.
       * @return 0-based index of the unit in the unit array.
       */
      u16 getIndex() const;
      void setSelected(bool selectedState);
      void setLoaded(bool loadedState);
      UnitImpl* getNext() const;
      /** Gets #bwOriginalUnit */
      BW::Unit* getOriginalRawData;
      /** Gets #bwUnitLocal */
      u8 getBuildQueueSlot;
      /** Gets #bwUnit->BW#Unit#buildQueue */
      BW::UnitType* getBuildQueue;
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue;

      void saveInitialInformation();
      void updateInternalData();
      void updateData();
      UnitData data;
      UnitData *self;

      bool userSelected;
      bool staticInformation;
      bool nukeDetected;
      int lastGroundWeaponCooldown;
      int lastAirWeaponCooldown;
      bool startingAttack;
      UnitType lastType;
      Player* lastPlayer;
      int id;
      bool isAlive;
      bool wasAlive;
      bool wasAccessible;
      bool wasVisible;

      std::set<Unit*> connectedUnits;
      std::set<Unit*> loadedUnits;
    private:
      bool canIssueCommand(UnitCommand c);
      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();

      u16 index; /**< Index of the unit in the array (can be computed, it's just optimisation) */
      UnitType staticType;
      BWAPI::Position staticPosition;
      int staticResources;
      int staticHitPoints;
      int lastOrderFrame;

      void* clientInfo;
  };
};


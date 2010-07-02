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

#define checkAccessBool() if (!this->attemptAccess()) return false
#define checkAccessInt() if (!this->attemptAccess()) return 0
#define checkAccessPosition() if (!this->attemptAccess()) return BWAPI::Positions::Unknown
#define checkAccessTilePosition() if (!this->attemptAccess()) return BWAPI::TilePositions::Unknown
#define checkAccessDouble() if (!this->attemptAccess()) return std::numeric_limits<double>::infinity()
#define checkAccessPointer() if (!this->attemptAccess()) return NULL

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

      virtual double getDistance(Unit* target) const;
      virtual double getDistance(Position target) const;
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
      virtual int getPlagueTimer() const;
      virtual int getRemoveTimer() const;
      virtual int getStasisTimer() const;
      virtual int getStimTimer() const;
      virtual int getOrderTimer() const;

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
      virtual Unit*    getChild() const;
      virtual Position getRallyPosition() const;
      virtual Unit*    getRallyUnit() const;
      virtual Unit*    getAddon() const;
      virtual Unit*    getNydusExit() const;

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
      virtual bool isBeingHealed() const;
      virtual bool isBeingGathered() const;
      virtual bool isBlind() const;
      virtual bool isBraking() const;
      virtual bool isBurrowed() const;
      virtual bool isCarryingGas() const;
      virtual bool isCarryingMinerals() const;
      virtual bool isCloaked() const;
      virtual bool isCompleted() const;
      virtual bool isConstructing() const;
      virtual bool isDefenseMatrixed() const;
      virtual bool isEnsnared() const;
      virtual bool isFollowing() const;
      virtual bool isGatheringGas() const;
      virtual bool isGatheringMinerals() const;
      virtual bool isHallucination() const;
      virtual bool isIdle() const;
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
      virtual bool attackMove(Position position);
      virtual bool attackUnit(Unit* target);
      virtual bool rightClick(Position position);
      virtual bool rightClick(Unit* target);
      virtual bool train(UnitType type);
      virtual bool build(TilePosition position, UnitType type);
      virtual bool buildAddon(UnitType type);
      virtual bool research(TechType tech);
      virtual bool upgrade(UpgradeType upgrade);
      virtual bool stop();
      virtual bool holdPosition();
      virtual bool patrol(Position position);
      virtual bool follow(Unit* target);
      virtual bool setRallyPosition(Position target);
      virtual bool setRallyUnit(Unit* target);
      virtual bool repair(Unit* target);
      virtual bool returnCargo();
      virtual bool morph(UnitType type);
      virtual bool burrow();
      virtual bool unburrow();
      virtual bool siege();
      virtual bool unsiege();
      virtual bool cloak();
      virtual bool decloak();
      virtual bool lift();
      virtual bool land(TilePosition position);
      virtual bool load(Unit* target);
      virtual bool unload(Unit* target);
      virtual bool unloadAll();
      virtual bool unloadAll(Position position);
      virtual bool cancelConstruction();
      virtual bool haltConstruction();
      virtual bool cancelMorph();
      virtual bool cancelTrain();
      virtual bool cancelTrain(int slot);
      virtual bool cancelAddon();
      virtual bool cancelResearch();
      virtual bool cancelUpgrade();
      virtual bool useTech(TechType tech);
      virtual bool useTech(TechType tech, Position position);
      virtual bool useTech(TechType tech, Unit* target);

      //Internal BWAPI commands:
      UnitImpl(BW::Unit* originalUnit,
               u16 index);
      ~UnitImpl();
      static UnitImpl* BWUnitToBWAPIUnit(BW::Unit* unit);
      void die();
      bool canAccess() const;
      bool canAccessSpecial() const;
      bool canAccessInside() const;
      bool attemptAccess() const;
      bool _exists;
      Player* _getPlayer;
      UnitType _getType;
      std::string getName() const;
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
      void updateUnitData();
      BWAPIC::UnitData data;
      BWAPIC::UnitData* self;

      bool dead;
      bool userSelected;
      bool staticInformation;
      BWAPI::Player* savedPlayer;
      BWAPI::UnitType savedUnitType;
      bool lastVisible;
      bool makeVisible;
      UnitType lastType;
      Player* lastPlayer;
      bool nukeDetected;
      int animState;
      int lastGroundWeaponCooldown;
      int lastAirWeaponCooldown;
      bool startingAttack;
      int id;

      std::set<Unit*> connectedUnits;
      std::set<Unit*> loadedUnits;
    private:

      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();

      u16 index; /**< Index of the unit in the array (can be computed, it's just optimisation) */
      UnitType staticType;
      BWAPI::Position staticPosition;
      int staticResources;
      int staticHitPoints;
  };
};


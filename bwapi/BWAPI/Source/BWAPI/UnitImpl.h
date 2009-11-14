#pragma once

#include "BWAPI/Unit.h"

#include <list>

#include <Util/Types.h>
#include <BWAPI/Order.h>
#include <BW/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include <BW/TilePosition.h>
#include <BW/OrderID.h>

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

      virtual Player* __cdecl getPlayer() const;
      virtual UnitType __cdecl getType() const;
      virtual int __cdecl getHitPoints() const;
      virtual int __cdecl getShields() const;
      virtual int __cdecl getEnergy() const;
      virtual int __cdecl getResources() const;
      virtual int __cdecl getKillCount() const;
      virtual int __cdecl getGroundWeaponCooldown() const;
      virtual int __cdecl getAirWeaponCooldown() const;
      virtual int __cdecl getSpellCooldown() const;
      virtual int __cdecl getDefenseMatrixPoints() const;

      virtual int __cdecl getDefenseMatrixTimer() const;
      virtual int __cdecl getEnsnareTimer() const;
      virtual int __cdecl getIrradiateTimer() const;
      virtual int __cdecl getLockdownTimer() const;
      virtual int __cdecl getMaelstromTimer() const;
      virtual int __cdecl getPlagueTimer() const;
      virtual int __cdecl getRemoveTimer() const;
      virtual int __cdecl getStasisTimer() const;
      virtual int __cdecl getStimTimer() const;

      virtual Position __cdecl getPosition() const;
      virtual TilePosition __cdecl getTilePosition() const;
      virtual double __cdecl getDistance(Unit* target) const;
      virtual double __cdecl getDistance(Position target) const;
      virtual int __cdecl getCurrentDirection() const;

      virtual Unit* __cdecl getTarget() const;
      virtual Position __cdecl getTargetPosition() const;
      virtual Order __cdecl getOrder() const;
      virtual Unit* __cdecl getOrderTarget() const;
      /* Timer specifiing how long it will take to finish the current order
       * (verified for mining).
       */
      virtual int __cdecl getOrderTimer() const;
      virtual Order __cdecl getSecondaryOrder() const;
      virtual Unit* __cdecl getBuildUnit() const;
      virtual int __cdecl getRemainingBuildTime() const;
      virtual int __cdecl getRemainingTrainTime() const;
      virtual Unit* __cdecl getChild() const;
      virtual std::list<UnitType > __cdecl getTrainingQueue() const;
      virtual Unit* __cdecl getTransport() const;
      virtual std::list<Unit*> __cdecl getLoadedUnits() const;
      virtual int __cdecl getInterceptorCount() const;
      virtual int __cdecl getScarabCount() const;
      virtual int __cdecl getSpiderMineCount() const;
      virtual TechType __cdecl getTech() const;
      virtual UpgradeType __cdecl getUpgrade() const;
      virtual int __cdecl getRemainingResearchTime() const;
      virtual int __cdecl getRemainingUpgradeTime() const;
      virtual Position __cdecl getRallyPosition() const;
      virtual Unit* __cdecl getRallyUnit() const;
      virtual Unit* __cdecl getAddon() const;


      virtual bool __cdecl exists() const;
      virtual bool __cdecl isAccelerating() const;
      virtual bool __cdecl isBeingConstructed() const;
      virtual bool __cdecl isBeingHealed() const;
      virtual bool __cdecl isBlind() const;
      virtual bool __cdecl isBraking() const;
      virtual bool __cdecl isBurrowed() const;
      virtual bool __cdecl isCarryingGas() const;
      virtual bool __cdecl isCarryingMinerals() const;
      virtual bool __cdecl isCloaked() const;
      virtual bool __cdecl isCompleted() const;
      virtual bool __cdecl isConstructing() const;
      virtual bool __cdecl isDefenseMatrixed() const;
      virtual bool __cdecl isEnsnared() const;
      virtual bool __cdecl isFollowing() const;
      virtual bool __cdecl isGatheringGas() const;
      virtual bool __cdecl isGatheringMinerals() const;
      virtual bool __cdecl isHallucination() const;
      virtual bool __cdecl isIdle() const;
      virtual bool __cdecl isIrradiated() const;
      virtual bool __cdecl isLifted() const;
      virtual bool __cdecl isLoaded() const;
      virtual bool __cdecl isLockedDown() const;
      virtual bool __cdecl isMaelstrommed() const;
      virtual bool __cdecl isMorphing() const;
      virtual bool __cdecl isMoving() const;
      virtual bool __cdecl isParasited() const;
      virtual bool __cdecl isPatrolling() const;
      virtual bool __cdecl isPlagued() const;
      virtual bool __cdecl isRepairing() const;
      virtual bool __cdecl isResearching() const;
      virtual bool __cdecl isSelected() const;
      virtual bool __cdecl isSieged() const;
      virtual bool __cdecl isStartingAttack() const;
      virtual bool __cdecl isStasised() const;
      virtual bool __cdecl isStimmed() const;
      virtual bool __cdecl isTraining() const;
      virtual bool __cdecl isUnderStorm() const;
      virtual bool __cdecl isUnpowered() const;
      virtual bool __cdecl isUpgrading() const;
      virtual bool __cdecl isVisible() const;

      virtual bool __cdecl attackMove(Position position);
      virtual bool __cdecl attackUnit(Unit* target);
      /**
       * Order this unit to right click on the specified location. Note that
       * right click on location will always result in move.
       */
      virtual bool __cdecl rightClick(Position position);
      /**
       * Orders this unit to right click on the specified unit. Note that right
       * click on unit can result in lot of commands (attack, gather, follow,
       * set rally point)
       */
      virtual bool __cdecl rightClick(Unit* target);
      /** Orders this unit to train (construct) the specified unit. */
      virtual bool __cdecl train(UnitType type);
      /** Orders to build the specified building. */
      virtual bool __cdecl build(TilePosition position, UnitType type);
      virtual bool __cdecl buildAddon(UnitType type);
      /** Orders to build the research the specified tech. */
      virtual bool __cdecl research(TechType tech);
      /** Orders to build the upgrade the specified upgrade. */
      virtual bool __cdecl upgrade(UpgradeType upgrade);
      virtual bool __cdecl stop();
      virtual bool __cdecl holdPosition();
      virtual bool __cdecl patrol(Position position);
      virtual bool __cdecl follow(Unit* target);
      virtual bool __cdecl setRallyPosition(Position target);
      virtual bool __cdecl setRallyUnit(Unit* target);
      virtual bool __cdecl repair(Unit* target);
      virtual bool __cdecl morph(UnitType type);
      virtual bool __cdecl burrow();
      virtual bool __cdecl unburrow();
      virtual bool __cdecl siege();
      virtual bool __cdecl unsiege();
      virtual bool __cdecl cloak();
      virtual bool __cdecl decloak();
      virtual bool __cdecl lift();
      virtual bool __cdecl land(TilePosition position);
      virtual bool __cdecl load(Unit* target);
      virtual bool __cdecl unload(Unit* target);
      virtual bool __cdecl unloadAll();
      virtual bool __cdecl unloadAll(Position position);
      virtual bool __cdecl cancelConstruction();
      virtual bool __cdecl haltConstruction();
      virtual bool __cdecl cancelMorph();
      virtual bool __cdecl cancelTrain();
      virtual bool __cdecl cancelTrain(int slot);
      virtual bool __cdecl cancelAddon();
      virtual bool __cdecl cancelResearch();
      virtual bool __cdecl cancelUpgrade();
      virtual bool __cdecl useTech(TechType tech);
      virtual bool __cdecl useTech(TechType tech, Position position);
      virtual bool __cdecl useTech(TechType tech, Unit* target);


      //Internal BWAPI commands:
      UnitImpl(BW::Unit* originalUnit,
               BW::Unit* unitLocal,
               u16 index);
      ~UnitImpl();
      static UnitImpl* BWUnitToBWAPIUnit(BW::Unit* unit);
      void die();

      inline bool canAccess() const;
      inline bool canAccessSpecial() const;
      inline bool attemptAccess() const;
      inline bool attemptAccessSpecial() const;
      inline bool _exists() const;
      bool died() const;
      Player* _getPlayer() const;
      UnitType _getType() const;
      Position _getPosition() const;
      TilePosition _getTilePosition() const;
      Unit* _getOrderTarget() const;
      bool _isCompleted() const;

      BW::UnitType UnitImpl::getBWType() const;

      /**
       * Gets index of the unit in the unit array. Note that the index is same
       * for the original unit array, BWAPI::Unit array and BWAI::Unit array, so
       * it is good way to compare units of different types and for conversion.
       * @return 0-based index of the unit in the unit array.
       */
      u16 getIndex() const;
      BW::OrderID::Enum getBWOrder() const;
      void setSelected(bool selectedState);
      void setLoaded(bool loadedState);
      std::string getName() const;
      UnitImpl* getNext() const;
      /** Gets #bwOriginalUnit */
      BW::Unit* getOriginalRawData() const;
      /** Gets #bwUnitLocal */
      BW::Unit* getRawDataLocal() const;
      u8 getBuildQueueSlot() const;
      /** Gets #bwUnit->BW#Unit#buildQueue */
      BW::UnitType* getBuildQueue() const;
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue() const;
      /** Returns if the unit has full building queue */
      bool hasFullBuildQueue() const;

      UnitImpl* buildUnit;
      bool alive;
    private:

      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();

      BW::Unit* bwUnitLocal; /**< Pointer to our local (precomputed) version of unit data table  @ref localData. */
      BW::Unit* bwOriginalUnit; /**< Pointer to broodwar unit data table. */
      u16 index; /**< Index of the unit in the array (can be computed, it's just optimisation) */
      bool userSelected;
      BWAPI::Player* savedPlayer;
      BWAPI::UnitType savedUnitType;
      bool dead;
  };
};


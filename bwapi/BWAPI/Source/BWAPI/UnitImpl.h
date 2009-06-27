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

      virtual Player* getOwner() const;
      virtual UnitType getType() const;
      virtual int health() const;
      virtual int shield() const;
      virtual int energy() const;
      virtual int getKillCount() const;
      virtual int groundWeaponCooldown() const;
      virtual int airWeaponCooldown() const;
      virtual int spellCooldown() const;
      virtual int defenseMatrixPoints() const;
      virtual int defenseMatrixTimer() const;
      virtual int stimTimer() const;
      virtual int ensnareTimer() const;
      virtual int lockdownTimer() const;
      virtual int irradiateTimer() const;
      virtual int stasisTimer() const;
      virtual int plagueTimer() const;
      virtual int maelstromTimer() const;
      virtual Position getPosition() const;
      virtual TilePosition getTilePosition() const;

      virtual Unit* getTarget() const;
      virtual Position getTargetPosition() const;
      virtual Order getOrder() const;
      virtual Unit* getOrderTarget() const;
      /* Timer specifiing how long it will take to finish the current order
       * (verified for mining).
       */
      virtual int getOrderTimer() const;
      virtual Order getSecondaryOrder() const;
      virtual Unit* getBuildUnit() const;
      virtual int getRemainingBuildTime() const;
      virtual Unit* getChild() const;
      virtual std::list<UnitType > getTrainingQueue() const;
      virtual std::list<Unit*> getLoadedUnits() const;

      virtual bool isAccelerating() const;
      virtual bool isBeingHealed() const;
      virtual bool isBlind() const;
      virtual bool isBraking() const;
      virtual bool isBurrowed() const;
      virtual bool isCloaked() const;
      virtual bool isCompleted() const;
      virtual bool isDisabled() const;
      virtual bool isIdle() const;
      virtual bool isLifted() const;
      virtual bool isLoaded() const;
      virtual bool isLockedDown() const;
      virtual bool isMoving() const;
      virtual bool isResearching() const;
      virtual bool isSelected() const;
      virtual bool isSieged() const;
      virtual bool isStartingAttack() const;
      virtual bool isStasised() const;
      virtual bool isTraining() const;
      virtual bool isUnderStorm() const;
      virtual bool isUpgrading() const;
      virtual bool isVisible() const;

      virtual bool attackMove(Position position);
      virtual bool attackUnit(Unit* target);
      /**
       * Order this unit to right click on the specified location. Note that
       * right click on location will always result in move.
       */
      virtual bool rightClick(Position position);
      /**
       * Orders this unit to right click on the specified unit. Note that right
       * click on unit can result in lot of commands (attack, gather, follow,
       * set rally point)
       */
      virtual bool rightClick(Unit *target);
      /** Orders this unit to train (construct) the specified unit. */
      virtual bool train(UnitType type);
      /** Orders to build the specified building. */
      virtual bool build(TilePosition position, UnitType type);
      /** Orders to build the research the specified tech. */
      virtual bool research(TechType tech);
      /** Orders to build the upgrade the specified upgrade. */
      virtual bool upgrade(UpgradeType upgrade);
      virtual bool stop();
      virtual bool holdPosition();
      virtual bool patrol(Position position);
      virtual bool repair(Unit* target);
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
      virtual bool cancelTrain();
      virtual bool cancelTrain(int slot);
      virtual bool cancelAddon();
      virtual bool cancelResearch();
      virtual bool cancelUpgrade();
      virtual bool useTech(TechType tech);
      virtual bool useTech(TechType tech, Position position);
      virtual bool useTech(TechType tech, Unit* target);


    //Internal BWAPI commands:
      /** 
       * @param bwUnit         #bwUnit                          
       * @param bwOriginalUnit #bwOriginalUnit
       * @param bwUnitLocal    #bwUnitLocal
       * @param index          #index
       */
      UnitImpl(BW::Unit* bwUnit,
           BW::Unit* bwOriginalUnit,
           BW::Unit* bwUnitLocal,
           u16 index);
      ~UnitImpl();
      static UnitImpl* BWUnitToBWAPIUnit(BW::Unit* unit);
      /**
       * Gets if the unit is alive (it exists), it uses hp > 0 heuristic for
       * now.
       */
      bool isValid() const;
      BW::UnitType UnitImpl::getBWType() const;
      /** Gets if the unit is alive - exists and it's construction is done. */
      bool isReady() const;

      /** Gets distance of unit edges. */
      u16 getDistance(Unit *unit) const; 
      /** Gets distance of unit center and position. */
      u16 getDistance(BW::Position position) const; 
      /** Gets distance of unit centers. */
      u16 getCenterDistance(Unit *unit) const; 
      /**
       * Gets index of the unit in the unit array. Note that the index is same
       * for the original unit array, BWAPI::Unit array and BWAI::Unit array, so
       * it is good way to compare units of different types and for conversion.
       * @return 0-based index of the unit in the unit array.
       */
      u16 getIndex() const;
      void setSelected(bool selectedState);
      void setLoaded(bool loadedState);
      std::string getName() const;
      UnitImpl* getNext() const;
      bool isMineral() const;
      /** Gets #bwOriginalUnit */
      BW::Unit *getOriginalRawData() const;
      /** Gets #bwUnitLocal */
      BW::Unit *getRawDataLocal() const;
      u8 getBuildQueueSlot() const;
      /** Gets #bwUnit->BW#Unit#buildQueue */
      BW::UnitType* getBuildQueue() const;
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue() const;
      /** Returns if the unit has full building queue */
      bool hasFullBuildQueue() const;

    private:
      /** Gets #bwUnit */
      BW::Unit *getRawData() const;

      bool hasEmptyBuildQueueSync() const;
      BW::UnitType* getBuildQueueSync() const;
      u8 getBuildQueueSlotSync() const;
      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();
      /** Gets distance between two positions. */
      u16 getDistance(int x1, int y1, int x2, int y2) const; 

      BW::Unit* bwUnit; /**< Pointer to our copy of of unit data table. */
      BW::Unit* bwUnitLocal; /**< Pointer to our local (precomputed) version of unit data table  @ref localData. */ 
      BW::Unit* bwOriginalUnit; /**< Pointer to broodwar unit data table. */
      u16 index; /**< Index of the unit in the array (can be computed, it's just optimisation) */
      bool userSelected;
      bool visible;
  };
};


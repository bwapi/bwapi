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
      virtual u16 health() const;
      virtual u32 shield() const;
      virtual u16 energy() const;
      virtual const BW::Position& getPosition() const;
      virtual BW::TilePosition getTilePosition() const;

      virtual Unit* getTarget() const;
      virtual BW::Position getTargetPosition() const;
      virtual Order getOrderID() const;
      virtual Unit* getOrderTarget() const;
      /* Timer specifiing how long it will take to finish the current order
       * (verified for mining).
       */
      virtual u8 getOrderTimer() const;
      virtual Order getSecondaryOrderID() const;
      virtual Unit* getBuildUnit() const;
      virtual u16 getRemainingBuildTime() const;
      virtual Unit* getChild() const;

      /** Returns true if this unit is currently in a dropship/bunker/refinery/shuttle/overlord */
      virtual bool isLoaded() const;
      /** For now, visibility is derived from heuristics (i.e. fog of war map, cloaking, burrowing, etc) */
      virtual bool isVisible() const;
      /** Only active when UserInput flag is enabled */
      virtual bool isSelected() const;
      /** Gets if the unit construction is done */
      virtual bool isCompleted() const;
      virtual bool isLifted() const;
      virtual bool isBurrowed() const;
      virtual bool isIdle() const;
      virtual bool isCloaked() const;
      virtual bool isDisabled() const;
      virtual bool isTraining() const;
      /** Gets if the current unit mineral (there are 3 Types of minerals) */
      virtual bool isMineral() const; /* TODO: Remove */
      virtual std::list<UnitType > getTrainingQueue() const;

      /**
       * Order this unit to right click on the specified location. Note that
       * right click on location will always result in move.
       */
      virtual void attackLocation(BW::Position position, Order order);
      /**
       * Order this unit to right click on the specified location. Note that
       * right click on location will always result in move.
       */
      virtual void rightClick(BW::Position position);
      /**
       * Orders this unit to right click on the specified unit. Note that right
       * click on unit can result in lot of commands (attack, gather, follow,
       * set rally point)
       */
      virtual void rightClick(Unit *target);
      /** Orders this unit to train (construct) the specified unit. */
      virtual void train(UnitType type);
      /** Orders to build the specified building. */
      virtual void build(BW::TilePosition position, UnitType type);
      /** Orders to build the invent the specified tech. */
      virtual void invent(TechType tech);
      /** Orders to build the invent the specified upgrade. */
      virtual void upgrade(UpgradeType upgrade);
      virtual void stop();
      virtual void holdPosition();
      virtual void patrol(BW::Position position);
      virtual void useTech(TechType tech);
      virtual void useTech(TechType tech, BW::Position position);
      virtual void useTech(TechType tech, Unit* target);


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
      bool loaded;
  };
};


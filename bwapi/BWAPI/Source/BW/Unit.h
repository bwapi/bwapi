#pragma once

#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"

#include <BW/OrderID.h>
#include <BW/UnitType.h>
#include <BW/MovementFlags.h>
#include <BW/Position.h>
#include <BW/UnitStatusFlags.h>
#include <BW/ParasiteFlags.h>
#include <BW/OrderFlags.h>
#include <BW/Offsets.h>

namespace BW { struct CSprite; };
namespace BW { struct Order; };

namespace BW
{
  const u32 UNIT_SIZE_IN_BYTES = 336;


  /**
   * Direct data mapping of the broodwar unit structure. Most of the information here is derived
   * from http://farty1billion.dyndns.org/EUDDB/?pg=ref&a=unitnode
   * Values marked @todo Unknown are labeled as the EUDDB has them labeled, but the true meaning
   * is not understood. Values marked @todo Verify have known meanings, but are not confirmed.
   */
#pragma pack(1)
  struct Unit
  {
    /*0x000*/ BW::Unit*                           previousUnit;
    /*0x004*/ BW::Unit*                           nextUnit;              /**< Pointer to next unit in the unit linked list, we use
                                                                          *   it to iterate units.
                                                                          *   @see BW#BWXFN_UnitNodeTable_FirstElement
                                                                          **/
    /*0x008*/ s32                                 hitPoints;             /**< Hit points of unit, note that the displayed value
                                                                          *   in broodwar is ceil(healthPoints/256)
                                                                          */
    /*0x00C*/ BW::CSprite*                        sprite;
    /*0x010*/ BW::Position                        moveToPos;
    /*0x014*/ BW::Unit*                           targetUnit;
    /*0x018*/ BW::Position                        nextWaypoint;  /**< The next way point in the path the unit is following to get to its destination.
                                                                    * Equal to moveToPos for air units since they don't need to navigate around buildings
                                                                    * or other units.
                                                                    */
    /*0x01C*/ BW::Position                        nextWaypoint2;  /**< Appears to always be equal to nextWaypoint */
    /*0x020*/ Util::BitMask<u8>                   movementFlags;  /**< Flags specifying movement type - defined in BW#MovementFlags. */
    /*0x021*/ u8                                  currentDirection;     /**< The current direction the unit is facing */
    /*0x022*/ u8                                  flingyTurnRadius;
    /*0x023*/ u8                                  velocityDirection;    /**< This usually only differs from the currentDirection field for units that can accelerate
                                                                   *   and travel in a different direction than they are facing. For example Mutalisks can change
                                                                   *   the direction they are facing faster than then can change the direction they are moving.
                                                                   */
    /*0x024*/ u16                                 flingyID;
    /*0x026*/ _UNKNOWN                            unknown_0x026;
    /*0x027*/ u8                                  flingyMovementType;
    /*0x028*/ BW::Position                        position;             /**< Current position of the unit */
    /*0x02C*/ u32                                 xHalt;                /**< @todo Unknown */
    /*0x030*/ u32                                 yHalt;                /**< @todo Unknown */
    /*0x034*/ u32                                 flingySpeed;
    /*0x038*/ u32                                 unknownSpeed_0x038;   /**< @todo Unknown */
    /*0x03C*/ u32                                 unknownSpeed_0x03C;   /**< @todo Unknown */
    /*0x040*/ s32                                 current_speedX;
    /*0x044*/ s32                                 current_speedY;

    /*0x048*/ u16                                 flingyAcceleration;
    /*0x04A*/ u16                                 unknown_0x04A;
    /*0x04C*/ u8                                  playerID;             /**< Specification of owner of this unit. */
    /*0x04D*/ u8                                  orderID;              /**< Specification of type of order currently given. */
    /*0x04E*/ Util::BitMask<u8>                   orderFlags;     /**< Additional order info (mostly unknown, wander property investigated so far) */
    /*0x04F*/ u8                                  orderSignal;          /**< @todo Unknown */
    /*0x050*/ BW::UnitType                        orderUnitID;
    /*0x052*/ u16                                 unused_0x052;
    /*0x054*/ u8                                  mainOrderTimer;       /**< @todo Unknown */
    /*0x055*/ u8                                  groundWeaponCooldown;
    /*0x056*/ u8                                  airWeaponCooldown;
    /*0x057*/ u8                                  spellCooldown;
    /*0x058*/ BW::Position                        orderTargetPos;
    /*0x05C*/ BW::Unit*                           orderTargetUnit;
    /*0x060*/ u32                                 shieldPoints;         /**< Bw shows this value/256 */
    /*0x064*/ BW::UnitType                        unitID;               /**< Specifies the type of unit. */
    /*0x066*/ u16                                 unknown_0x066;
    /*0x068*/ BW::Unit*                           previousPlayerUnit;
    /*0x06C*/ BW::Unit*                           nextPlayerUnit;
    /*0x070*/ BW::Unit*                           subUnit;
    /*0x074*/ BW::Order*                          orderQueueHead;
    /*0x078*/ BW::Order*                          orderQueueTail;
    /*0x07C*/ u32                                 unknown_0x07C;
    /*0x080*/ BW::Unit*                           connectedUnit;        /**< Addon is connected to building (addon has conntected building, but not in other direction */
    /*0x084*/ u8                                  orderQueueCount;         /**< @todo Verify */
    /*0x085*/ u8                                  unknownOrderTimer_0x085; /**< @todo Unknown */
    /*0x086*/ u16                                 unknown_0x086;
    /*0x088*/ u16                                 displayedUnitID;
    /*0x08A*/ u32                                 unknown_0x08A;
    /*0x08E*/ u8                                  rankIncrease;       /**< Adds this value to the unit's base rank */
    /*0x08F*/ u8                                  killCount;          /**< Killcount */
    /*0x090*/ u32                                 stateFlags;         /**< @todo Unknown */
    /*0x094*/ u16                                 currentButtonSet; // The u16 is a guess, used to be u8
    /*0x095*/ u16                                 unknown_0x095;
    /*0x098*/ BW::UnitType                        buildQueue[5];      /**< Queue of units to build. Note that it doesn't begin with index 0, but with #buildQueueSlot index. */
    /*0x0A2*/ u16                                 energy;             /**< Energy Points */
    /*0x0A4*/ u8                                  buildQueueSlot;     /**< Index of active unit in #buildQueue. */
    /*0x0A5*/ u8                                  targetOrderSpecial; /**< A byte used to determine the target ID for the unit */
    /*0x0A6*/ u8                                  secondaryOrderID;   /**< (Build addon verified) @todo verify (Cloak, Build, ExpandCreep suggested by EUDDB) */
    /*0x0A7*/ u8                                  unknown_0x0A7;
    /*0x0A8*/ u32                                 hpGainDuringRepair; /**< @todo Verify */
    /*0x0AC*/ u16                                 remainingBuildTime; /**< Remaining bulding time */
    /*0x0AE*/ u16                                 unknown_0x0AE;
    /*0x0B0*/ u16                                 loadedUnitIndex[8];
    /** Child unit information (structure depends on unitID */
    /*0x0C0*/ union ChildInfoUnion_type
    {
      /** Child info structure if the unit is vulture. */
      struct UnitIsVultureBike_type
      {
        u8 spiderMineCount;
        _UNKNOWN _1[3];
      } vultureBikeMines;
      /**
       * Unit type - meaning
       * -# Carrier/Reaver - First Hangar Unit
       * -# Scarab/Interceptor - Parent
       * -# Building - addon
       * -# Worker - Powerup carried
       */
      BW::Unit* childUnit1;
      BW::Unit* parentUnit;
    } childInfoUnion;            /**< @todo Verify */

    /** Additional unit info depending on unit type */
    /*0x0C4*/ union ChildUnitUnion1_type
    {
      /** If the unit is building, this structure applies. */
      struct UnitIsBuilding_type
      {
        u16 addonBuildID;
        u16 upgradeResearchTime;
      } unitIsBuilding;

      /** If the unit is Scarab/Interceptor - Next Unit in Parent Hangar */
      BW::Unit* nextUnitInParentHangar;
    } childUnitUnion1;                /**< @todo Verify */

    /** Additional unit info depending on unit type */
    /*0x0C8*/ union ChildUnitUnion2_type
    {
      /** if the unit is not scarab/interceptor this structure maps the data. */
      struct UnitIsNotScarabInterceptor_type
      {
        /** If the unit is Reaver */
        union ScarabInfo                // 0x0C8
        {
          u8 scarabCount;
          u8 interceptorCountInHangar;
          u8 techID;
        } subChildUnitUnion1;
        /** If the unit is carrier */
        union InterceptorInfo           // 0x0C9
        {
          u8 interceptorCountOutOfHangar;
          u8 upgradeID;
        } subChildUnitUnion2;
        /** If the unit is hatchery, this contains timer of the larva spawning */
        u8 larvaSpawnTimer;
        /** If the unit is terran building, it contains timer of building landing */
        u8 isLanding;


      } unitIsNotScarabInterceptor;

      /* If this unit is Scarab/Interceptor, it is pointer to Previous in Parent's Hangar */
      BW::Unit* prevUnitInParentHangar;
    } childUnitUnion2;                  /**< @todo Verify */

    /** Additional unit info it's value meaning is different for interceptor/scarab - creep unit or repairing scv @todo verify*/
    /*0x0CC*/ union ChildUnitUnion3_type
    {
      u8 inHanger; // If interceptor/scarab
      u8 creepExpansionTimer; // If creep unit, timer between creep expansions
      u8 decreaseMineralTimer; // If repairing
    } childUnitUnion3;              /**< @todo Verify */
    /*0x0CD*/ u8                      upgradeLevel;       /**< @todo Unknown */
    /*0x0CE*/ u8                      isCarryingSomething; /**< @todo Verify (may be set if going to carry something or targetting resources.. If 'isgathering' ? */
    /*0x0CF*/ u8                      resourceCarying;     /**< The amount of resources it is carrying */
    /** Some internal unit data that are not investigated good enough*/
    /*0x0D0*/ union UnitUnion1_type
    {
      /** When the unit is worker? @todo investigate */
      union UnitUnion1Sub_type
      {
        u16 x, y;
        /** When the unit is mineral? @todo investigate */
        struct ResourceUnitUnionSub_type
        {
          u16 resourceContained;
          u8  resourceIscript;
          u8  isBeingGathered;
        } resourceUnitUnionSub;
      } unitUnion1Sub;
      /** Probably the connected nydius canal @todo investigate*/
      BW::Unit* resourceTarget_connectedNydus;
      /** Nuke dot, but of what, the ghost? @todo investigate*/
      BW::CSprite* nukeDot;
    } unitUnion1;                     /**< @todo Verify */

    /*0x0D4*/ u32                               hasNuke;
    /*0x0D8*/ BW::Unit                          *unknown_0x0D8;
    /*0x0DC*/ Util::BitMask<u32>                status;
    /*0x0E0*/ u8                                resourceType;       /**< Resource being held by worker: 1 = gas, 2 = ore */
    /*0x0E1*/ u8                                wireframeRandomizer;/**< @todo Unknown */
    /*0x0E2*/ u8                                secondaryOrderState;/**< @todo Unknown */
    /*0x0E3*/ u8                                unknownCounterDown_0x0E3; /**< @todo Unknown */
    /*0x0E4*/ u32                               visibilityStatus;
    /*0x0E8*/ u32                               unknown_0x0E8;
    /*0x0EC*/ BW::Unit*                         currentBuildUnit;   /**< @todo Unknown */
    /*0x0F0*/ _UNKNOWN                          unknown_0x0F0[8];
    /**
     * Additional unit data; its meaning depends if it is a Unit that can have rally point or is psi provider
     * @todo investigate
     */
    /*0x0F8*/ union RallyPsiProviderUnion_type
    {
      /** If the unit is rally type @todo investigate*/
      struct Rally_type
      {
        /* BW::Position rallyPos; */
        u16 rallyX, rallyY;
        BW::Unit* rallyUnit;
      } rally;
      /** If the unit is psi provider @todo investigate */
      struct PsiProvider_type
      {
        BW::Unit* prevPsiProvider;
        BW::Unit* nextPsiProvider;
      };
    } rallyPsiProviderUnion;                        /**< @todo Verify */
    /*0x100*/ u32                                 pathUnknown_0x100;  /**< @todo Unknown */
    /*0x104*/ u16                                 unknown_0x104;
    /*0x106*/ u8                                  unused_0x106;
    /*0x107*/ u8                                  isBeingHealed;      /**< @todo Verify - seems like it isn't working */
    /*0x108*/ BW::Position                        contours1Unknown;   /**< @todo Unknown */
    /*0x10C*/ BW::Position                        contours2Unknown;   /**< @todo Unknown */
    /*0x110*/ u16                                 removeTimer;        /**< @todo Verified for Hallucination, unverified for DSwarm, DWeb, Broodling */
    /*0x112*/ u16                                 defenseMatrixDamage;
    /*0x114*/ u8                                  defenseMatrixTimer;
    /*0x115*/ u8                                  stimTimer;
    /*0x116*/ u8                                  ensnareTimer;
    /*0x117*/ u8                                  lockdownTimer;
    /*0x118*/ u8                                  irradiateTimer;
    /*0x119*/ u8                                  stasisTimer;
    /*0x11A*/ u8                                  plagueTimer;
    /*0x11B*/ u8                                  isUnderStorm;
    /*0x11C*/ BW::Unit*                           irradiatedBy;       /**< @todo Verify */
    /*0x120*/ u8                                  irradiatePlayerID;  /**< @todo Verify */
    /*0x121*/ Util::BitMask<u8>                   parasiteFlags;
    /*0x122*/ u8                                  cycleCounter;       /**< @todo Verify (runs updates approx 2 times per sec) */
    /*0x123*/ u8                                  isBlind;
    /*0x124*/ u8                                  maelstromTimer;
    /*0x125*/ u8                                  unused_0x125;
    /*0x126*/ u8                                  acidSporeCount;   /**< @todo Verify */
    /*0x127*/ u8                                  acidSporeTime[9];
    /*0x130*/ u16                                 offsetIndex3by3;    /**< @todo Unknown */
    /*0x132*/ u16                                 unused_0x132;
    /*0x134*/ u32                                 isAIControlled;
    /*0x138*/ u16                                 airStrength;        /**< @todo Verify (1 if has no air weapon but has a ground) */
    /*0x13A*/ u16                                 groundStrength;     /**< @todo Verify (1 if has no ground weapon but has an air) */
    /*0x13C*/ u32                                 unknown_0x13C;
    /*0x13C*/ u32                                 unknown_0x140;
    /*0x13C*/ u32                                 unknown_0x144;
    /*0x13C*/ u32                                 unknown_0x148;
    /*0x14C*/ u8                                  repulseUnknown1;    /**< @todo Unknown */
    /*0x14D*/ u8                                  repulseUnknown2;    /**< @todo Unknown */
    /*0x14E*/ u8                                  driftPosX;          /**< @todo Unknown (mapsizex/1.5 max) */
    /*0x14F*/ u8                                  driftPosY;          /**< @todo Unknown (mapsizex/1.5 max) */
  };
#pragma pack()
  /** Direct maping of the UnitNodeTable in bw memory. */
  struct UnitArray
  {
    Unit unit[UNIT_ARRAY_MAX_LENGTH];
  };

  //BOOST_STATIC_ASSERT(sizeof(Unit) == UNIT_SIZE_IN_BYTES);
  //BOOST_STATIC_ASSERT(sizeof(UnitArray) == UNIT_SIZE_IN_BYTES*  UNIT_ARRAY_MAX_LENGTH);
};


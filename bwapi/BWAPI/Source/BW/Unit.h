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

namespace BW { struct Sprite; };
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
    /*0x000*/ BW::Unit*                    previousUnit;
    /*0x004*/ BW::Unit*                    nextUnit;              /**< Pointer to next unit in the unit linked list, we use 
                                                                   *   it to iterate units.
                                                                   *   @see BW#BWXFN_UnitNodeTable_FirstElement
                                                                   **/
    /*0x008*/ u32                          healthPoints;          /**< Health points of unit, note that the displayed value
                                                                   *   in broodwar is ceil(healthPoints/256)
                                                                   */
    /*0x00C*/ BW::Sprite*                   sprite;
    /*0x010*/ BW::Position                  moveToPos;
    /*0x014*/ BW::Unit*                     targetUnit;
    /*0x018*/ _UNKNOWN _2[8];
    /*0x020*/ Util::BitMask<MovementFlags::Enum>  movementFlags;  /**< Flags specifying movement type - defined in BW#MovementFlags. */
    /*0x021*/ _UNKNOWN _3[1];
    /*0x022*/ u8                            flingyTurnRadius;     /**< @todo Unknown */
    /*0x023*/ u8                            currentDirection;     /**< @todo Unknown */
    /*0x024*/ u16                           flingyID;             /**< @todo Unknown */
    /*0x026*/ _UNKNOWN _4[1];
    /*0x027*/ u8                            flingyMovementType;   /**< @todo Unknown */
    /*0x028*/ BW::Position                  position;             /**< Current position of the unit */
    /*0x02C*/ u32                           xHalt;                /**< @todo Unknown */
    /*0x030*/ u32                           yHalt;                /**< @todo Unknown */
    /*0x034*/ u32                           flingySpeed;          /**< @todo Unknown */
    /*0x038*/ u32                           unknownSpeed_0x038;   /**< @todo Unknown */
    /*0x03C*/ u32                           unknownSpeed_0x03C;   /**< @todo Unknown */
    /*0x040*/ _UNKNOWN _5[8];
    /*0x048*/ u16                           flingyAcceleration;   /**< @todo Unknown */
    /*0x04A*/ _UNKNOWN _6[2];
    /*0x04C*/ u8                            playerID;             /**< Specification of owner of this unit. (Note that player 11 is neutral). */
    /*0x04D*/ BW::OrderID::Enum             orderID;              /**< Specification of type of order currently given. */
    /*0x04E*/ Util::BitMask<BW::OrderFlags::Enum> orderFlags;     /**< Additional order info (mostly unknown, wander property investigated so far) */
    /*0x04F*/ u8                            orderSignal;          /**< @todo Unknown */
    /*0x050*/ _UNKNOWN _7[4];
    /*0x054*/ u8                            mainOrderTimer;       /**< @todo Unknown */
    /*0x055*/ u8                            groundWeaponCooldown;
    /*0x056*/ u8                            airWeaponCooldown;    /**< @todo Verify */
    /*0x057*/ u8                            spellCooldown;        /**< @todo Verify */
    /*0x058*/ BW::Position                  orderTargetPos;       /**< @todo Verify */
    /*0x05C*/ BW::Unit*                     orderTargetUnit;      /**< @todo Verify */
    /*0x060*/ u32                           shieldPoints;         /**< Bw shows this value/256 */
    /*0x064*/ BW::UnitType                  unitID;               /**< Specifies the type of unit. */
    /*0x066*/ _UNKNOWN _9[2];
    /*0x068*/ BW::Unit*                     previousPlayerUnit;   /**< @todo Verify */
    /*0x06C*/ BW::Unit*                     nextPlayerUnit;       /**< @todo Verify */
    /*0x070*/ BW::Unit*                     subUnit;              /**< @todo Verify */
    /*0x074*/ BW::Order*                    orderQueueHead;       /**< @todo Verify */
    /*0x078*/ BW::Order*                    orderQueueTail;       /**< @todo Verify */
    /*0x07C*/ _UNKNOWN _10[4];
    /*0x080*/ BW::Unit*                     connectedUnit;        /**< @todo Verify */
    /*0x084*/ u8                            orderQueueCount;         /**< @todo Verify */
    /*0x085*/ u8                            unknownOrderTimer_0x085; /**< @todo Unknown */
    /*0x086*/ _UNKNOWN _11[2];
    /*0x088*/ u16                          displayedUnitID;    /**< @todo Verify */
    /*0x08A*/ _UNKNOWN _12[4];
    /*0x08E*/ u8                           rankIncrease;       /**< @todo Unknown */
    /*0x08F*/ u8                           killCount;          /**< @todo Verify */
    /*0x090*/ _UNKNOWN _13[3];
    /*0x093*/ u8                           unknownState_0x93;  /**< @todo Unknown */
    /*0x094*/ u8                           currentButtonSet;   /**< @todo Verify */
    /*0x095*/ _UNKNOWN _14[3];
    /*0x098*/ BW::UnitType                 buildQueue[5];      /**< Queue of units to build. Note that it doesn't begin with index 0, but with #buildQueueSlot index. */
    /*0x0A2*/ u16                          energy;             /**< @todo Verify */
    /*0x0A4*/ u8                           buildQueueSlot;     /**< Index of active unit in #buildQueue. */
    /*0x0A5*/ _UNKNOWN _15[1];
    /*0x0A6*/ u8                           secondaryOrderID;   /**< @todo Unknown  (Cloak, Build, ExpandCreep suggested by EUDDB) */
    /*0x0A7*/ _UNKNOWN _16[1];
    /*0x0A8*/ u16                          hpGainDuringRepair; /**< @todo Verify */
    /*0x0AA*/ _UNKNOWN _17[2];
    /*0x0AC*/ u16                          remainingBuildTime; /**< Remaining bulding time */
    /*0x0AE*/ _UNKNOWN _18[18];
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
              }                       childInfoUnion;     /**< @todo Verify */
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
              }                       childUnitUnion1;    /**< @todo Verify */
              /** Additional unit info depending on unit type */
    /*0x0C8*/ union ChildUnitUnion2_type
              {
                /** if the unit is not scarab/interceptor this structure maps the data. */
                struct UnitIsNotScarabInterceptor_type
                {
                  /** If the unit is Reaver */
                  union ScarabInfo
                  {
                    u8 scarabCount;
                    u8 techID;
                  } subChildUnitUnion1;
                  /** If the unit is carrier */
                  union InterceptorInfo
                  {
                    u8 interceptorCount;
                    u8 upgradeID;
                  } subChildUnitUnion2;
                  /** If the unit is hatchery, this contains timer of the larva spawning */
                  u8 larvaSpawnTimer;
                  /** If the unit is terran building, it contains timer of building landing */
                  u8 isLanding;
                } unitIsNotScarabInterceptor;

                /* If this unit is Scarab/Interceptor, it is pointer to Previous in Parent's Hangar */
                BW::Unit* prevUnitInParentHangar;                
              }                       childUnitUnion2;    /**< @todo Verify */
              /** Additional unit info it's value meaning is different for interceptor/scarab - creep unit or repairing scv @todo verify*/
    /*0x0CC*/ union ChildUnitUnion3_type
              {                
                u8 inHanger; // If interceptor/scarab
                u8 creepExpansionTimer; // If creep unit, timer between creep expansions
                u8 decreaseMineralTimer; // If repairing
              }                       childUnitUnion3;    /**< @todo Verify */
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
                    u8 resourceIsScript;
                    u8 resourceCount;
                  } resourceUnitUnionSub;
                } unitUnion1Sub;
                /** Probably the connected nydius canal @todo investigate*/
                BW::Unit* resourceTarget_connectedNydus;
                /** Nuke dot, but of what, the ghost? @todo investigate*/
                BW::Sprite* nukeDot;
              }                       unitUnion1;         /**< @todo Verify */
    /*0x0D4*/ _UNKNOWN _19[8];
    /*0x0DC*/ Util::BitMask<StatusFlags::Enum>   status;       /**< @todo Verify */
    /*0x0E0*/ u8                           resourceType;       /**< @todo Unknown (type of resource this unit is?) */
    /*0x0E1*/ u8                           wireframeRandomizer;/**< @todo Unknown */
    /*0x0E2*/ u8                           secondaryOrderState;/**< @todo Unknown */
    /*0x0E3*/ u8                           unknownCounterDown_0x0E3; /**< @todo Unknown */
    /*0x0E4*/ _UNKNOWN _20[8];
    /*0x0EC*/ BW::Unit*                    currentBuildUnit;   /**< @todo Unknown */
    /*0x0F0*/ _UNKNOWN _21[8];
              /** 
               * Additional unit data it's meaning depends if it is Unit that can have rally point or is psi provider (what about main?)
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
              }                            rallyPsiProviderUnion; /**< @todo Verify */
    /*0x100*/ u32                          pathUnknown_0x100;  /**< @todo Unknown */
    /*0x104*/ _UNKNOWN _22[3];
    /*0x107*/ u8                           isBeingHealed;      /**< @todo Verify */
    /*0x108*/ BW::Position                 contours1Unknown;   /**< @todo Unknown */
    /*0x10A*/ BW::Position                 contours2Unknown;   /**< @todo Unknown */
    /*0x110*/ u16                          removeTimer;        /**< @todo Verify (Hallucination, DSwarm, DWeb, Broodling) */
    /*0x112*/ u16                          defenseMatrixDamage;/**< @todo Verify */
    /*0x114*/ u8                           defenseMatrixTimer; /**< @todo Verify */
    /*0x115*/ u8                           stimTimer;          /**< @todo Verify */
    /*0x116*/ u8                           ensareTimer;        /**< @todo Verify */
    /*0x117*/ u8                           lockdownTimer;      /**< @todo Verify */
    /*0x118*/ u8                           irradiateTimer;     /**< @todo Verify */
    /*0x119*/ u8                           stasisTimer;        /**< @todo Verify */
    /*0x11A*/ u8                           plagueTimer;        /**< @todo Verify */
    /*0x11B*/ u8                           isUnderStorm;       /**< @todo Verify */
    /*0x11C*/ BW::Unit*                    irradiatedBy;       /**< @todo Verify */
    /*0x120*/ u8                           irradiatePlayerID;  /**< @todo Verify */
    /*0x121*/ Util::BitMask<ParasiteFlags::Enum> parasiteFlags;/**< @todo Verify BW#ParasiteFlags */
    /*0x122*/ u8                           cycleCounter;       /**< @todo Verify (runs updates approx 2 times per sec) */
    /*0x123*/ u8                           isBlind;            /**< @todo Verify */
    /*0x124*/ u8                           maelstromTimer;     /**< @todo Verify */
    /*0x125*/ _UNKNOWN _23[1];
    /*0x126*/ u8                           acidSporeTime[9];   /**< @todo Verify */
    /*0x130*/ u16                          offsetIndex3by3;    /**< @todo Unknown */
    /*0x132*/ _UNKNOWN _24[6];
    /*0x138*/ u16                          airStrength;        /**< @todo Verify (1 if has no air weapon but has a ground) */
    /*0x13A*/ u16                          groundStrength;     /**< @todo Verify (1 if has no ground weapon but has an air) */
    /*0x13C*/ _UNKNOWN _25[16];
    /*0x14C*/ u8                           repulseUnknown1;    /**< @todo Unknown */
    /*0x14D*/ u8                           repulseUnknown2;    /**< @todo Unknown */
    /*0x14E*/ u8                           driftPosX;          /**< @todo Unknown (mapsizex/1.5 max) */
    /*0x14F*/ u8                           driftPosY;          /**< @todo Unknown (mapsizex/1.5 max) */
    /*0x150*/ _UNKNOWN _26[1];
  };
  #pragma pack()
  /** Direct maping of the UnitNodeTable in bw memory. */
  struct UnitArray
  {
    Unit unit[UNIT_ARRAY_MAX_LENGTH];
  };

  BOOST_STATIC_ASSERT(sizeof(Unit) == UNIT_SIZE_IN_BYTES);
  BOOST_STATIC_ASSERT(sizeof(UnitArray) == UNIT_SIZE_IN_BYTES * UNIT_ARRAY_MAX_LENGTH);
};


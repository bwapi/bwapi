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
namespace BW { struct Path; };

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
    /*0x000*/ BW::Unit                            *prev;
    /*0x004*/ BW::Unit                            *next;                 /**< Pointer to next unit in the unit linked list, we use
                                                                          *   it to iterate units.
                                                                          *   @see BW#BWXFN_UnitNodeTable_FirstElement
                                                                          **/
    /*0x008*/ s32                                 hitPoints;             /**< Hit points of unit, note that the displayed value
                                                                          *   in broodwar is ceil(healthPoints/256)
                                                                          */
    /*0x00C*/ BW::CSprite                         *sprite;
    /*0x010*/ BW::Position                        moveToPos;
    /*0x014*/ BW::Unit                            *targetUnit;
    /*0x018*/ BW::Position                        nextWaypoint;  /**< The next way point in the path the unit is following to get to its destination.
                                                                    * Equal to moveToPos for air units since they don't need to navigate around buildings
                                                                    * or other units.
                                                                    */
    /*0x01C*/ BW::Position                        nextWaypoint2;  /**< Appears to always be equal to nextWaypoint */
    /*0x020*/ Util::BitMask<u8>                   movementFlags;  /**< Flags specifying movement type - defined in BW#MovementFlags. */
    /*0x021*/ u8                                  currentDirection1;     /**< The current direction the unit is facing */
    /*0x022*/ u8                                  flingyTurnRadius;
    /*0x023*/ u8                                  velocityDirection1;    /**< This usually only differs from the currentDirection field for units that can accelerate
                                                                   *   and travel in a different direction than they are facing. For example Mutalisks can change
                                                                   *   the direction they are facing faster than then can change the direction they are moving.
                                                                   */
    /*0x024*/ u16                                 flingyID;
    /*0x026*/ u8                                  unknown_0x026;
    /*0x027*/ u8                                  flingyMovementType;
    /*0x028*/ BW::Position                        position;             /**< Current position of the unit */
    /*0x02C*/ u32                                 xHalt;                /**< @todo Unknown */
    /*0x030*/ u32                                 yHalt;                /**< @todo Unknown */
    /*0x034*/ u32                                 flingySpeed;
    /*0x038*/ s32                                 current_speed1;
    /*0x03C*/ s32                                 current_speed2;
    /*0x040*/ s32                                 current_speedX;
    /*0x044*/ s32                                 current_speedY;

    /*0x048*/ u16                                 flingyAcceleration;
    /*0x04A*/ u8                                  currentDirection2;
    /*0x04B*/ u8                                  velocityDirection2;    // pathing related, gets this value from Path::unk_1A?
    /*0x04C*/ u8                                  playerID;             /**< Specification of owner of this unit. */
    /*0x04D*/ u8                                  orderID;              /**< Specification of type of order currently given. */
    /*0x04E*/ Util::BitMask<u8>                   orderFlags;     /**< Additional order info (mostly unknown, wander property investigated so far) */
                                                  /*  0x01  Moving/Following Order
                                                      0x02  No collide (Larva)?
                                                      0x04  Harvesting? Working?
                                                      0x08  Constructing Stationary
                                                      Note: I don't actually think these are flags
                                                  */
    /*0x04F*/ u8                                  orderSignal;          /**< @todo Unknown */
                                                  /*
                                                      0x01  Update building graphic/state
                                                      0x02  Casting spell
                                                      0x04  Reset collision? Always enabled for hallucination...
                                                      0x10  Lift/Land state
                                                  */
    /*0x050*/ BW::UnitType                        orderUnitID;
    /*0x052*/ u16                                 unused_0x052;
    /*0x054*/ u8                                  mainOrderTimer;         // A timer for orders, example: time left before minerals are harvested
    /*0x055*/ u8                                  groundWeaponCooldown;
    /*0x056*/ u8                                  airWeaponCooldown;
    /*0x057*/ u8                                  spellCooldown;
    /*0x058*/ BW::Position                        orderTargetPos;
    /*0x05C*/ BW::Unit                            *orderTargetUnit;
    /*0x060*/ u32                                 shieldPoints;         /**< Bw shows this value/256 */
    /*0x064*/ BW::UnitType                        unitType;             /**< Specifies the type of unit. */
    /*0x066*/ u16                                 unknown_0x066;
    /*0x068*/ BW::Unit                            *previousPlayerUnit;
    /*0x06C*/ BW::Unit                            *nextPlayerUnit;
    /*0x070*/ BW::Unit                            *subUnit;
    /*0x074*/ BW::Order                           *orderQueueHead;
    /*0x078*/ BW::Order                           *orderQueueTail;
    /*0x07C*/ BW::Unit                            *AIOrderTargetUnit;   // Unknown, something with auto targetting?
    /*0x080*/ BW::Unit                            *connectedUnit;        /**< Addon is connected to building (addon has conntected building, but not in other direction */
    /*0x084*/ u8                                  orderQueueCount;         /**< @todo Verify */
    /*0x085*/ u8                                  orderQueueTimer; /* counts/cycles down from from 8 to 0 (inclusive). See also 0x122. */
    /*0x086*/ u16                                 unknown_0x086;  // pathing related
    /*0x088*/ u16                                 displayedUnitID;
    /*0x08A*/ u16                                 unknown_0x08A;
    /*0x08C*/ u8                                  unknown_0x08C;
    /*0x08D*/ u8                                  unknown_timer_0x08D;
    /*0x08E*/ u8                                  rankIncrease;       /**< Adds this value to the unit's base rank */
    /*0x08F*/ u8                                  killCount;          /**< Killcount */
    /*0x090*/ u32                                 stateFlags;         /**< @todo Unknown */
    /*0x094*/ u16                                 currentButtonSet; // The u16 is a guess, used to be u8
    /*0x096*/ u8                                  cloaked;          // 1 for cloaked
    /*0x097*/ u8                                  movementState;    // A value based on conditions related to pathing
    /*0x098*/ BW::UnitType                        buildQueue[5];      /**< Queue of units to build. Note that it doesn't begin with index 0, but with #buildQueueSlot index. */
    /*0x0A2*/ u16                                 energy;             /**< Energy Points */
    /*0x0A4*/ u8                                  buildQueueSlot;     /**< Index of active unit in #buildQueue. */
    /*0x0A5*/ u8                                  targetOrderSpecial; /**< A byte used to determine the target ID for the unit */
    /*0x0A6*/ u8                                  secondaryOrderID;   /**< (Build addon verified) @todo verify (Cloak, Build, ExpandCreep suggested by EUDDB) */
    /*0x0A7*/ u8                                  buildingFlags; 
                                                  /*  0x02    Unknown, applies to assimilators/refineries/extractors and buildings that can train units
                                                      0x04    Building
                                                  */
    /*0x0A8*/ u32                                 hpGainDuringRepair; /**< @todo Verify */
    /*0x0AC*/ u16                                 remainingBuildTime; /**< Remaining bulding time */
    /*0x0AE*/ u16                                 previousHP;   // The HP of the unit before it changed (example Drone->Hatchery, the Drone's HP will be stored here)
    /*0x0B0*/ u16                                 loadedUnitIndex[8];
    /*0x0C0*/ union
              {
                struct
                {
                  u8 spiderMineCount; // 0
                } vulture;
                
                struct
                {
                  BW::Unit *child;          // 0
                  u32      _unknown_04;     // 4
                  u8       inHangerCount;   // 8
                  u8       outHangerCount;
                } carrier;
                
                struct
                {
                  BW::Unit *parent;   // 0
                  BW::Unit *next;     // 4
                  BW::Unit *prev;     // 8
                  u8       inHanger;  // C
                } interceptor;
                
                struct
                {
                  BW::Unit *addon;          // 0
                  u16 addonBuildType;       // 4
                  u16 upgradeResearchTime;  // 6
                  u8  techType;             // 8
                  u8  upgradeType;          // 9
                  u8  larvaTimer;           // A
                  u8  landingTimer;         // B
                  u8  creepTimer;           // C
                } building;
                
                struct 
                {
                  BW::Unit *powerup;                // 0
                  u32      _unknown_04;             // 4
                  u32      _unknown_08;             // 8
                  u8       repairResourceLossTimer; // C
                } worker;
              };
    /*0x0CD*/ u8 upgradeLevel;        /**< @todo Unknown */
    /*0x0CE*/ u8 isCarryingSomething; /**< @todo Verify (may be set if going to carry something or targetting resources.. If 'isgathering' ? */
    /*0x0CF*/ u8 resourceCarying;     /**< The amount of resources it is carrying */
    /*0x0D0*/ union
              { union
                { struct { u16 x, y; } worker2; /** When the unit is worker? @todo investigate; This is a guess */
                  struct
                  { u16 resourceContained;
                    u8  resourceIscript;
                    u8  isBeingGathered;
                  } resource;  /** When the unit is mineral? @todo investigate */
                };
                struct { BW::Unit* exit;       } nydus; /** Probably the connected nydius canal @todo investigate*/
                struct { BW::CSprite* nukeDot; } ghost; /** Nuke dot, but of what, the ghost? @todo investigate*/
              };
    /*0x0D4*/ u32                               hasNuke;
    /*0x0D8*/ BW::Unit                          *unknown_0x0D8;
    /*0x0DC*/ Util::BitMask<u32>                status;
    /*0x0E0*/ u8                                resourceType;       /**< Resource being held by worker: 1 = gas, 2 = ore */
    /*0x0E1*/ u8                                wireframeRandomizer;/**< @todo Unknown */
    /*0x0E2*/ u8                                secondaryOrderState;/**< @todo Unknown */
    /*0x0E3*/ u8                                unknownCounterDown_0x0E3; /**< @todo Unknown */
    /*0x0E4*/ s32                               visibilityStatus;
    /*0x0E8*/ u32                               unknown_0x0E8;
    /*0x0EC*/ BW::Unit                          *currentBuildUnit;   /**< @todo Unknown */
    /*0x0F0*/ u8                                unknown_0x0F0[8];
    /*0x0F8*/ union
              { struct
                { u16      x, y;
                  BW::Unit *unit;
                } rally;                /** If the unit is rally type @todo investigate*/

                struct
                { BW::Unit* prevPsiProvider;
                  BW::Unit* nextPsiProvider;
                } pylon;                /** If the unit is psi provider @todo investigate */
              };
    /*0x100*/ BW::Path                            *path;                  /**< @todo Unknown */
    /*0x104*/ u8                                  pathingCollisionInterval;  // unknown
    /*0x105*/ u8                                  pathingEnabled;         // 1 for ground units
    /*0x106*/ u16                                 unknown_0x106;
    /*0x108*/ BW::rect                            contourBounds;          /**< @todo Unknown */  // top left point
    /*0x110*/ u16                                 removeTimer;            /**< Verified for Hallucination, DWeb, Scarab, DSwarm, and Broodling; does not apply to scanner sweep */
    /*0x112*/ u16                                 defenseMatrixDamage;
    /*0x114*/ u8                                  defenseMatrixTimer;
    /*0x115*/ u8                                  stimTimer;
    /*0x116*/ u8                                  ensnareTimer;
    /*0x117*/ u8                                  lockdownTimer;
    /*0x118*/ u8                                  irradiateTimer;
    /*0x119*/ u8                                  stasisTimer;
    /*0x11A*/ u8                                  plagueTimer;
    /*0x11B*/ u8                                  isUnderStorm;
    /*0x11C*/ BW::Unit                            *irradiatedBy;      /**< @todo Verify */
    /*0x120*/ u8                                  irradiatePlayerID;  /**< @todo Verify */
    /*0x121*/ Util::BitMask<u8>                   parasiteFlags;
    /*0x122*/ u8                                  cycleCounter;       /* counts/cycles up from 0 to 7 (inclusive). See also 0x85. */
    /*0x123*/ u8                                  isBlind;
    /*0x124*/ u8                                  maelstromTimer;
    /*0x125*/ u8                                  unused_0x125;
    /*0x126*/ u8                                  acidSporeCount;     /**< @todo Verify */
    /*0x127*/ u8                                  acidSporeTime[9];
    /*0x130*/ u16                                 offsetIndex3by3;    /**< @todo Unknown */
    /*0x132*/ u16                                 unused_0x132;
    /*0x134*/ void                                *CAIControl;        // pointer to AI class, we're not using this though
    /*0x138*/ u16                                 airStrength;        /**< verified */
    /*0x13A*/ u16                                 groundStrength;     /**< verified */
    /*0x13C*/ u32                                 leftMostUnitOrder;  // Left-to-right order (triggers for example)
    /*0x140*/ u32                                 leftMostUnitOrder2; // Left-to-right variant
    /*0x144*/ u32                                 topMostUnitOrder;   // Top-to-bottom order
    /*0x148*/ u32                                 topMostUnitOrder2;  // Top-to-bottom variant
    /*0x14C*/ u8                                  repulseUnknown;     /**< @todo Unknown */
    /*0x14D*/ u8                                  repulseAngle;       // updated only when air unit is being pushed
    /*0x14E*/ u8                                  driftPosX;          /**< (mapsizex/1.5 max) */
    /*0x14F*/ u8                                  driftPosY;          /**< (mapsizex/1.5 max) */
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


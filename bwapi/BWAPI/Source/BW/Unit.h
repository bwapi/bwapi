#pragma once

#include <Util/Types.h>
#include <BW/Sprite.h>
#include <BW/Order.h>
#include <BW/Path.h>
#include <BW/Position.h>

#include <BWAPI/UnitType.h>
#include <BW/Offsets.h>

namespace BW { class Sprite; };
namespace BW { struct Order; };
namespace BW { struct Path; };

namespace BW
{
  /**
   * Direct data mapping of the broodwar unit structure. Most of the information here is derived
   * from http://farty1billion.dyndns.org/EUDDB/?pg=ref&a=unitnode
   * Values marked @todo Unknown are labeled as the EUDDB has them labeled, but the true meaning
   * is not understood. Values marked @todo Verify have known meanings, but are not confirmed.
   */
#pragma pack(1) // Note: alignment is probably default 4, not 1
  class Unit
  {
  public:
    BWAPI::UnitType type();
    bool movementFlag(u8 flags);
    bool statusFlag(u32 flags);

    /*0x000*/ BW::Unit                            *prev;
    /*0x004*/ BW::Unit                            *next;                 /**< Pointer to next unit in the unit linked list, we use
                                                                          *   it to iterate units.
                                                                          *   @see BW#BWXFN_UnitNodeTable_FirstElement
                                                                          **/
    /*0x008*/ s32                                 hitPoints;             /**< Hit points of unit, note that the displayed value
                                                                          *   in broodwar is ceil(healthPoints/256)
                                                                          */
    /*0x00C*/ BW::Sprite                          *sprite;
    /*0x010*/ BW::Position                        moveToPos;
    /*0x014*/ BW::Unit                            *moveToUnit;          // The original description "target unit" was incorrect
    /*0x018*/ BW::Position                        nextMovementWaypoint;  /**< The next way point in the path the unit is following to get to its destination.
                                                                        * Equal to moveToPos for air units since they don't need to navigate around buildings
                                                                        * or other units.
                                                                        */
    /*0x01C*/ BW::Position                        nextTargetWaypoint;  /**< The desired position */
    /*0x020*/ u8                                  movementFlags;       /**< Flags specifying movement type - defined in BW#MovementFlags. */
    /*0x021*/ u8                                  currentDirection1;   /**< The current direction the unit is facing */
    /*0x022*/ u8                                  flingyTurnRadius;
    /*0x023*/ u8                                  velocityDirection1;  /**< This usually only differs from the currentDirection field for units that can accelerate
                                                                       *   and travel in a different direction than they are facing. For example Mutalisks can change
                                                                       *   the direction they are facing faster than then can change the direction they are moving.
                                                                       */
    /*0x024*/ u16                                 flingyID;
    /*0x026*/ u8                                  _unknown_0x026;
    /*0x027*/ u8                                  flingyMovementType;
    /*0x028*/ BW::Position                        position;             /**< Current position of the unit */
    /*0x02C*/ u32                                 xHalt;                /**< @todo Unknown */
    /*0x030*/ u32                                 yHalt;                /**< @todo Unknown */
    /*0x034*/ u32                                 flingyTopSpeed;
    /*0x038*/ s32                                 current_speed1;
    /*0x03C*/ s32                                 current_speed2;
    /*0x040*/ s32                                 current_speedX;
    /*0x044*/ s32                                 current_speedY;

    /*0x048*/ u16                                 flingyAcceleration;
    /*0x04A*/ u8                                  currentDirection2;
    /*0x04B*/ u8                                  velocityDirection2;    // pathing related, gets this value from Path::unk_1A?
    /*0x04C*/ u8                                  playerID;             /**< Specification of owner of this unit. */
    /*0x04D*/ u8                                  orderID;              /**< Specification of type of order currently given. */
    /*0x04E*/ u8                                  orderState;     /**< Additional order info (mostly unknown, wander property investigated so far) */
                                                  /*  0x01  Moving/Following Order
                                                      0x02  No collide (Larva)?
                                                      0x04  Harvesting? Working?
                                                      0x08  Constructing Stationary
                                                      Note: I don't actually think these are flags
                                                  */
    /*0x04F*/ u8                                  orderSignal;
                                                  /*
                                                      0x01  Update building graphic/state
                                                      0x02  Casting spell
                                                      0x04  Reset collision? Always enabled for hallucination...
                                                      0x10  Lift/Land state
                                                  */
    /*0x050*/ u16                                 orderUnitType;
    /*0x052*/ u16                                 _unused_0x052; // possible alignment
    /*0x054*/ u8                                  mainOrderTimer;         // A timer for orders, example: time left before minerals are harvested
    /*0x055*/ u8                                  groundWeaponCooldown;
    /*0x056*/ u8                                  airWeaponCooldown;
    /*0x057*/ u8                                  spellCooldown;
    /*0x058*/ BW::Position                        orderTargetPos;
    /*0x05C*/ BW::Unit                            *orderTargetUnit;
    /*0x060*/ u32                                 shieldPoints;         /**< Bw shows this value/256 */
    /*0x064*/ u16                                 unitType;             /**< Specifies the type of unit. */
    /*0x066*/ u16                                 _unused_0x066; // possible alignment
    /*0x068*/ BW::Unit                            *previousPlayerUnit;
    /*0x06C*/ BW::Unit                            *nextPlayerUnit;
    /*0x070*/ BW::Unit                            *subUnit;
    /*0x074*/ BW::Order                           *orderQueueHead;
    /*0x078*/ BW::Order                           *orderQueueTail;
    /*0x07C*/ BW::Unit                            *AutoTargetUnit;   // The auto-acquired target
    /*0x080*/ BW::Unit                            *connectedUnit;        /**< Addon is connected to building (addon has conntected building, but not in other direction */
    /*0x084*/ u8                                  orderQueueCount;       /**< @todo Verify */
    /*0x085*/ u8                                  orderQueueTimer; /* counts/cycles down from from 8 to 0 (inclusive). See also 0x122. */
    /*0x086*/ u8                                  _unknown_0x086;  // pathing related?
    /*0x087*/ u8                                  _unknownTimer_0x087;  // pathing related?
    /*0x088*/ u16                                 displayedUnitID;
    /*0x08A*/ u8                                  lastEventTimer; // countdown that stops being recent when it hits 0 
    /*0x08B*/ u8                                  lastEventColor; // 17 = was completed (train, morph), 174 = was attacked
    /*0x08C*/ u16                                 _unused_0x08C;  // might have originally been RGB from lastEventColor
    /*0x08E*/ u8                                  rankIncrease;       /**< Adds this value to the unit's base rank */
    /*0x08F*/ u8                                  killCount;          /**< Killcount */
    /*0x090*/ u8                                  lastAttackingPlayer; // the player that last attacked this unit
    /*0x091*/ u16                                 secondaryOrderTimer; // A guess
    /*0x093*/ u8                                  userActionFlags;     // some flags that change when the user interacts with the unit
                                                                       // 2 = issued an order, 3 = interrupted an order, 4 = self destructing
    /*0x094*/ u16                                 currentButtonSet;    // The u16 is a guess, used to be u8
    /*0x096*/ u8                                  cloaked;             // 1 for cloaked
    /*0x097*/ u8                                  movementState;       // A value based on conditions related to pathing, see Path.h for info
    /*0x098*/ u16                                 buildQueue[5];      /**< Queue of units to build. Note that it doesn't begin with index 0, but with #buildQueueSlot index. */
    /*0x0A2*/ u16                                 energy;             /**< Energy Points */
    /*0x0A4*/ u8                                  buildQueueSlot;     /**< Index of active unit in #buildQueue. */
    /*0x0A5*/ u8                                  targetOrderSpecial; /**< A byte used to determine the target ID for the unit */
    /*0x0A6*/ u8                                  secondaryOrderID;   /**< (Build addon verified) @todo verify (Cloak, Build, ExpandCreep suggested by EUDDB) */
    /*0x0A7*/ u8                                  buildingOverlayState; // 0 means the building has the largest amount of fire/blood
    /*0x0A8*/ u32                                 hpGainDuringRepair; /**< @todo Verify */
    /*0x0AC*/ u16                                 remainingBuildTime; /**< Remaining bulding time; This is also the timer for powerups (flags) to return to their original location. */
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
                  BW::Unit *inHangerChild;  // 0   first child inside the hanger
                  BW::Unit *outHangerChild; // 4   first child outside the hanger
                  u8       inHangerCount;   // 8   number inside the hanger
                  u8       outHangerCount;  // 9   number outside the hanger
                } carrier; // also applies to reaver
                
                struct
                {
                  BW::Unit *parent;   // 0
                  BW::Unit *next;     // 4
                  BW::Unit *prev;     // 8
                  u8       inHanger;  // C
                } interceptor;  // also applies to scarab
                
                struct
                {
                  u32 _unknown_00;
                  u32 _unknown_04;
                  u32 flagSpawnFrame; // flag beacons, the frame that the flag will spawn
                } beacon;

                struct
                {
                  BW::Unit *addon;               // 0
                  u16      addonBuildType;       // 4
                  u16      upgradeResearchTime;  // 6
                  u8       techType;             // 8
                  u8       upgradeType;          // 9
                  u8       larvaTimer;           // A
                  u8       landingTimer;         // B
                  u8       creepTimer;           // C
                  u8       upgradeLevel;         // D
                  u16      _padding_0E;          // E    // possible alignment
                  union                          // 10
                  { 
                    struct
                    { u16       resourceCount;  // amount of resources
                      u8        resourceIscript;
                      u8        gatherQueueCount;
                      BW::Unit  *nextGatherer;    // 14  // pointer to the next workerunit waiting in line to gather
                      u8        resourceGroup;    // 18
                      u8        resourceBelongsToAI;
                    } resource;  /** When the unit is resource container */
                    struct { BW::Unit *exit; } nydus; /** connected nydius canal */
                    struct { BW::Sprite *nukeDot; } ghost;
                    struct { BW::Sprite *pylonAura; } pylon;
                    struct
                    { BW::Unit *nuke;  // attached nuke
                      u32 hasNuke;     // 14
                    } silo;
                    struct
                    { u16 harvestValue1;
                      u16 harvestValue2;
                      u16 _unknown_14;    // 14
                      u16 harvestValue3;
                    } hatchery; // wtf???
                    struct
                    {
                      s16 originX;
                      s16 originY;
                    } powerup;
                  };
                } building;
                
                struct 
                {
                  BW::Unit      *powerup;                // 0
                  u16           targetResourceX;         // 4
                  u16           targetResourceY;         // 6
                  BW::Unit      *targetResourceUnit;     // 8
                  u8            repairResourceLossTimer; // C
                  u8            _unknown_0D;             // D
                  u8            isCarryingSomething;     // E
                  u8            resourceCarryCount;      // F
                  BW::Unit      *harvestTarget;          // 10
                  BW::Unit      *nextHarvestUnit;        // 14   // When there is a gather conflict
                  BW::Unit      *previousHarvestUnit;    // 18
                } worker;
              };
    /*0x0DC*/ u32                               statusFlags;
    /*0x0E0*/ u8                                resourceType;       /**< Resource being held by worker: 1 = gas, 2 = ore */
    /*0x0E1*/ u8                                wireframeRandomizer;
    /*0x0E2*/ u8                                secondaryOrderState;
    /*0x0E3*/ u8                                recentOrderTimer;   // Counts down from 15 to 0 when most orders are given,
                                                                    // or when the unit moves after reaching a patrol location
    /*0x0E4*/ s32                               visibilityStatus;   // Flags specifying which players can detect this unit (cloaked/burrowed)
    /*0x0E8*/ u16                               _unknown_0x0E8;    // ??
    /*0x0EA*/ u16                               _unknown_0x0EA;    // possible alignment
    /*0x0EC*/ BW::Unit                          *currentBuildUnit;
    /*0x0F0*/ BW::Unit                          *previousBurrowedUnit;
    /*0x0F4*/ BW::Unit                          *nextBurrowedUnit;
    /*0x0F8*/ union
              { struct
                { u16      x, y;
                  BW::Unit *unit;
                } rally;                /** If the unit is rally type */

                struct
                { BW::Unit *prevPsiProvider;
                  BW::Unit *nextPsiProvider;
                } pylon;                /** If the unit is psi provider */
              };
    /*0x100*/ BW::Path                            *path;
    /*0x104*/ u8                                  pathingCollisionInterval;  // unknown
    /*0x105*/ u8                                  pathingFlags;           // 0x01 = uses pathing; 0x02 = ?; 0x04 = ?
    /*0x106*/ u8                                  _unused_0x106;
    /*0x107*/ u8                                  isBeingHealed;          // 1 if a medic is currently healing this unit
    /*0x108*/ rect                                contourBounds;          // a rect that specifies the closest contour (collision) points
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
    /*0x121*/ u8                                  parasiteFlags;
    /*0x122*/ u8                                  cycleCounter;       /* counts/cycles up from 0 to 7 (inclusive). See also 0x85. */
    /*0x123*/ u8                                  isBlind;
    /*0x124*/ u8                                  maelstromTimer;
    /*0x125*/ u8                                  _unused_0x125;  // ?? Might be afterburner timer or ultralisk roar timer
    /*0x126*/ u8                                  acidSporeCount;
    /*0x127*/ u8                                  acidSporeTime[9];
    /*0x130*/ u16                                 bulletBehaviour3by3AttackSequence; // Counts up for the number of bullets shot by a unit using
                                                                                     // this weapon behaviour and resets after it reaches 12
    /*0x132*/ u16                                 _unused_0x132;   // possible alignment
    /*0x134*/ void                                *CAIControl;        // pointer to AI class, we're not using this though
    /*0x138*/ u16                                 airStrength;        /**< verified */
    /*0x13A*/ u16                                 groundStrength;     /**< verified */
    /*0x13C*/ u32                                 unitFinderIndexLeft;    // Order for the unit's left boundry
    /*0x140*/ u32                                 unitFinderIndexRight;   // Order for the unit's right boundry
    /*0x144*/ u32                                 unitFinderIndexTop;     // Order for the unit's top boundry
    /*0x148*/ u32                                 unitFinderIndexBottom;  // Order for the unit's bottom boundry
    /*0x14C*/ u8                                  _repulseUnknown;     /**< @todo Unknown */
    /*0x14D*/ u8                                  repulseAngle;       // updated only when air unit is being pushed
    /*0x14E*/ u8                                  driftPosX;          /**< (mapsizex/1.5 max) */
    /*0x14F*/ u8                                  driftPosY;          /**< (mapsizex/1.5 max) */
  };
#pragma pack()
};


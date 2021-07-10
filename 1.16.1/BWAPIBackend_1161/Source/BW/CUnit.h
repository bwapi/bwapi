#pragma once

#include "../Types.h"
#include "Position.h"
#include "Target.h"
#include "Path.h"

#pragma pack(1)
namespace BWAPI
{
  class UnitType;
}
namespace BW
{
  // Forwards
  class CSprite;
  class COrder;
  struct Path;  

  /** Broodwar CUnit class.
   * Started from http://farty1billion.dyndns.org/EUDDB/?pg=ref&a=unitnode
   * Heavily altered and corrected.
   * Most members are known, including some padding/alignment bytes that have been identified.
   * Members that are marked as "unknown", "todo", "maybe", etc. must still be researched.
   *
   * Additional notes: possibly missing a "Cloaked" link, has a static member "nSorted"
   */
  class CUnit
  {
  public:
    BWAPI::UnitType type() const;
    bool movementFlag(u8 flags) const;
    bool statusFlag(u32 flags) const;

    const CUnit* getDamageDealer() const;

    bool isAttacking() const;
    int getGroundWeaponCooldown() const;
    int getAirWeaponCooldown() const;

// v-- POSSIBLE SHARED BULLET/UNIT STRUCTURE BEGIN

  // v--- CLink
    // CLink<CUnit> link;
    /*0x000*/ BW::CUnit*    prev;
    /*0x004*/ BW::CUnit*    next;      /**< Pointer to next unit in the unit linked list, we use
                                          *   it to iterate units.
                                          *   @see BW#BWXFN_UnitNodeTable_FirstElement
                                          **/
  // ^--- CLink
  // v--- CThingy
    /*0x008*/ s32           hitPoints;  // Hit points of unit, note that the displayed value in broodwar is ceil(healthPoints/256)
                                        // Officially called "xLife"
    /*0x00C*/ BW::CSprite*  sprite;
  // ^--- CThingy
  // v--- CFlingy
                                                    // One of these Position structs are just called "swLocX" and "swLocY"
    /*0x010*/ BW::Target    moveTarget;             // The position or unit to move to. It is NOT an order target.
    /*0x018*/ BW::Position  nextMovementWaypoint;   /**< The next way point in the path the unit is following to get to its destination.
                                                      * Equal to moveToPos for air units since they don't need to navigate around buildings
                                                      * or other units.
                                                      */
    /*0x01C*/ BW::Position  nextTargetWaypoint;   // The desired position
    /*0x020*/ u8            movementFlags;        // Flags specifying movement type - defined in BW#MovementFlags.
    /*0x021*/ u8            currentDirection1;    // The current direction the unit is facing
    /*0x022*/ u8            flingyTurnRadius;     // TODO: "TurnSpeed" in GPTP
    /*0x023*/ u8            velocityDirection1;  /**< This usually only differs from the currentDirection field for units that can accelerate
                                                 *   and travel in a different direction than they are facing. For example Mutalisks can change
                                                 *   the direction they are facing faster than then can change the direction they are moving.
                                                 */
    /*0x024*/ u16           flingyID;
    /*0x026*/ u8            _unknown_0x026;
    /*0x027*/ u8            flingyMovementType;
    /*0x028*/ BW::Position  position;         // Current position of the unit
    /*0x02C*/ point         halt;             // @todo Unknown    // Either this or current_speed is officially called "xDX, xDY" (no POINT struct)
    /*0x034*/ u32           flingyTopSpeed;
    /*0x038*/ s32           current_speed1;
    /*0x03C*/ s32           current_speed2;
    /*0x040*/ point         current_speed;    // TODO: velocity

    /*0x048*/ u16           flingyAcceleration;
    /*0x04A*/ u8            currentDirection2;
    /*0x04B*/ u8            velocityDirection2;   // pathing related, gets this value from Path::unk_1A?
    /*0x04C*/ u8            playerID;             // Specification of owner of this unit.
    /*0x04D*/ u8            orderID;              // Specification of type of order currently given. (TODO: Rename to mainOrderId?)
    /*0x04E*/ u8            orderState;  //< Additional order info (mostly unknown, wander property investigated so far)  // officially "ubActionState"
                                         /*  0x01  Moving/Following Order
                                             0x02  No collide (Larva)?
                                             0x04  Harvesting? Working?
                                             0x08  Constructing Stationary
                                             Note: I don't actually think these are flags
                                           */
    /*0x04F*/ u8            orderSignal;  /*  0x01  Update building graphic/state
                                              0x02  Casting spell
                                              0x04  Reset collision? Always enabled for hallucination...
                                              0x10  Lift/Land state
                                          */

    /*0x050*/ u16           orderUnitType;      // officially "uwFoggedTarget"
    
    u16 __0x52;  // 2-byte padding

    /*0x054*/ u8            mainOrderTimer;       // A timer for orders, example: time left before minerals are harvested
    /*0x055*/ u8            groundWeaponCooldown;
    /*0x056*/ u8            airWeaponCooldown;
    /*0x057*/ u8            spellCooldown;
    /*0x058*/ BW::Target    orderTarget;        // officially called ActionFocus
// ^-- POSSIBLE SHARED BULLET/UNIT STRUCTURE END

    /*0x060*/ u32         shieldPoints;      // BW shows this value/256, TODO possibly not u32?
    /*0x064*/ u16         unitType;        // Specifies the type of unit.
    
    u16 __0x66;  // 2-byte padding

    // CLink<CUnit> player_link;
    /*0x068*/ BW::CUnit*  previousPlayerUnit;
    /*0x06C*/ BW::CUnit*  nextPlayerUnit;

    /*0x070*/ BW::CUnit*  subUnit;

    // CLink<COrder> orderQueue_link;
    /*0x074*/ BW::COrder*   orderQueueHead;
    /*0x078*/ BW::COrder*   orderQueueTail;

    /*0x07C*/ BW::CUnit*  autoTargetUnit;      // The auto-acquired target (Note: This field is actually used for different targets internally, especially by the AI)
    /*0x080*/ BW::CUnit*  connectedUnit;       // Addon is connected to building (addon has conntected building, but not in other direction  (officially "pAttached")
    /*0x084*/ u8          orderQueueCount;      // @todo Verify   // officially "ubQueuedOrders"
    /*0x085*/ u8          orderQueueTimer;      // counts/cycles down from from 8 to 0 (inclusive). See also 0x122.
    /*0x086*/ u8          _unknown_0x086;       // pathing related?
    /*0x087*/ u8          attackNotifyTimer;    // Prevent "Your forces are under attack." on every attack
    /*0x088*/ u16         previousUnitType;     // Stores the type of the unit prior to being morphed/constructed
    /*0x08A*/ u8          lastEventTimer;       // countdown that stops being recent when it hits 0 
    /*0x08B*/ u8          lastEventColor;       // 17 = was completed (train, morph), 174 = was attacked
    /*0x08C*/ u16         _unused_0x08C;        // might have originally been RGB from lastEventColor
    /*0x08E*/ u8          rankIncrease;         // Adds this value to the unit's base rank
    /*0x08F*/ u8          killCount;            // Killcount
    /*0x090*/ u8          lastAttackingPlayer;  // the player that last attacked this unit
    /*0x091*/ u8          secondaryOrderTimer;
    /*0x092*/ u8          AIActionFlag;           // Internal use by AI only
    /*0x093*/ u8          userActionFlags;        // some flags that change when the user interacts with the unit
                                                  // 2 = issued an order, 3 = interrupted an order, 4 = self destructing

    /*0x094*/ u16         currentButtonSet;       // The u16 is a guess, used to be u8
    /*0x096*/ bool        isCloaked;
    /*0x097*/ UnitMovementState movementState;          // A value based on conditions related to pathing, see Path.h for info
    /*0x098*/ u16         buildQueue[5];          //< Queue of units to build. Note that it doesn't begin with index 0, but with #buildQueueSlot index. 
    /*0x0A2*/ u16         energy;                 //< Energy Points   // officially "xwMagic"
    /*0x0A4*/ u8          buildQueueSlot;         //< Index of active unit in #buildQueue. 
    /*0x0A5*/ u8          uniquenessIdentifier;   //< A byte used to determine uniqueness of the unit
    /*0x0A6*/ u8          secondaryOrderID;       //< (Build addon verified) @todo verify (Cloak, Build, ExpandCreep suggested by EUDDB) 
    /*0x0A7*/ u8          buildingOverlayState;   // 0 means the building has the largest amount of fire/blood
    /*0x0A8*/ u16         hpGain;                 //< hp gained on construction or repair
    /*0x0AA*/ u16         shieldGain;             //< Shield gain on construction
    /*0x0AC*/ u16         remainingBuildTime;     //< Remaining bulding time; This is also the timer for powerups (flags) to return to their original location.
    /*0x0AE*/ u16         previousHP;             // The HP of the unit before it changed (example Drone->Hatchery, the Drone's HP will be stored here)
    /*0x0B0*/ u16         loadedUnitIndex[8];     // officially called "uwTransport[8]"
    ///////////////
    /*0x0C0*/ union
              {
                struct
                {
                  u8 spiderMineCount; // 0
                } vulture;
                
                struct
                {
                  BW::CUnit*  pInHanger;     // 0   first child inside the hanger    // official
                  BW::CUnit*  pOutHanger;    // 4   first child outside the hanger
                  u8          inHangerCount;      // 8   number inside the hanger (used for scarab count)
                  u8          outHangerCount;     // 9   number outside the hanger
                } carrier; // also applies to reaver (Carrier is official)
                
                struct
                {
                  BW::CUnit*  parent;    // 0
                  // CLink<CUnit> hanger_link;
                  BW::CUnit*  prev;      // 4
                  BW::CUnit*  next;      // 8
                  bool        inHanger;   // C    // TODO: isOutsideHangar
                } fighter;  // also applies to scarab
                
                struct
                {
                  u32 _unknown_00;
                  u32 _unknown_04;
                  u32 flagSpawnFrame; // flag beacons, the frame that the flag will spawn
                } beacon;

                struct
                {
                  BW::CUnit*  addon;               // 0
                  u16         addonBuildType;       // 4
                  u16         upgradeResearchTime;  // 6
                  u8          techType;             // 8
                  u8          upgradeType;          // 9
                  u8          larvaTimer;           // A
                  u8          landingTimer;         // B
                  u8          creepTimer;           // C
                  u8          upgradeLevel;         // D
                  u16 __E;  // 2-byte padding
                } building;
                
                struct 
                {
                  BW::CUnit*  pPowerup;                // 0    // official
                  points      targetResource;           // 4    // TODO: Use points+unit target
                  BW::CUnit*  targetResourceUnit;      // 8
                  u16         repairResourceLossTimer;  // C
                  bool        isCarryingSomething;      // E    // There is a "ubIsHarvesting" somewhere
                  u8          resourceCarryCount;       // F
                } worker;   // Official name, but there is also a "CUnit::WorkerList::pHarvestBldg" somewhere
              };
    ///////////////
    /*0x0D0*/ union
              {
                struct
                {
                  u16         resourceCount;  // amount of resources
                  u8          resourceIscript;
                  u8          gatherQueueCount;
                  BW::CUnit*  nextGatherer;    // 4  // pointer to the next workerunit waiting in line to gather
                  u8          resourceGroup;    // 8
                  u8          resourceBelongsToAI;
                } resource;  // When the unit is resource container
                struct { BW::CUnit*   exit; } nydus; // connected nydius canal
                struct { BW::CSprite* nukeDot; } ghost;
                struct { BW::CSprite* pPowerTemplate; } Pylon;
                struct
                {
                  BW::CUnit* pNuke;   // attached nuke    // official name
                  bool bReady;      // 4   // official name
                } silo;   // Should be "CUnit::Silo::"
                struct
                {
                  ::rect harvestValue;
                } hatchery; // wtf???
                struct { points origin; } powerup;
                struct
                {
                  BW::CUnit* harvestTarget;           // 0
                  // CLINK<CUnit> harvest_link;
                  BW::CUnit* prevHarvestUnit;         // 4   // When there is a gather conflict
                  BW::CUnit* nextHarvestUnit;         // 8
                } gatherer; //there is also a "CUnit::WorkerList::pHarvestBldg" somewhere
              };
    /*0x0DC*/ u32       statusFlags;
    /*0x0E0*/ u8        resourceType;         // Resource being held by worker: 1 = gas, 2 = ore
    /*0x0E1*/ u8        wireframeRandomizer;
    /*0x0E2*/ u8        secondaryOrderState;
    /*0x0E3*/ u8        recentOrderTimer;     // Counts down from 15 to 0 when most orders are given,
                                              // or when the unit moves after reaching a patrol location
    /*0x0E4*/ s32       visibilityStatus;     // Flags specifying which players can detect this unit (cloaked/burrowed)
    /*0x0E8*/ BW::Position secondaryOrderPosition;       // unused
    /*0x0EC*/ BW::CUnit* currentBuildUnit;    // tied to secondary order
    // CLink<CUnit> burrow_link;
    /*0x0F0*/ BW::CUnit* previousBurrowedUnit;
    /*0x0F4*/ BW::CUnit* nextBurrowedUnit;
    /*0x0F8*/ union
              {
                struct
                { 
                  points      position;   // Target
                  BW::CUnit*  unit;
                } rally;  // If the unit is rally type

                struct
                { // CLink<CUnit> power_link;
                  BW::CUnit*  prevPsiProvider;
                  BW::CUnit*  nextPsiProvider;
                } PsiProvider;  // If the unit is psi provider
              };
    /*0x100*/ BW::Path* path;    // officially "unitPath"
    /*0x104*/ u8        pathingCollisionInterval; // unknown
    /*0x105*/ u8        pathingFlags;             // 0x01 = uses pathing; 0x02 = ?; 0x04 = ?
    /*0x106*/ u8        _unused_0x106;
    /*0x107*/ bool      isBeingHealed;    // 1 if a medic is currently healing this unit
    /*0x108*/ rect      contourBounds;    // a rect that specifies the closest contour (collision) points
    struct
    {
      /*0x110*/ u16         removeTimer;      //  does not apply to scanner sweep
      /*0x112*/ u16         defenseMatrixDamage;    // TODO: Rename to HP
      /*0x114*/ u8          defenseMatrixTimer;
      /*0x115*/ u8          stimTimer;
      /*0x116*/ u8          ensnareTimer;
      /*0x117*/ u8          lockdownTimer;
      /*0x118*/ u8          irradiateTimer;
      /*0x119*/ u8          stasisTimer;
      /*0x11A*/ u8          plagueTimer;
      /*0x11B*/ u8          stormTimer;
      /*0x11C*/ BW::CUnit*  irradiatedBy;
      /*0x120*/ u8          irradiatePlayerID;
      /*0x121*/ u8          parasiteFlags;    // bitmask identifying which players have parasited this unit
      /*0x122*/ u8          cycleCounter;     // counts/cycles up from 0 to 7 (inclusive). See also 0x85.
      /*0x123*/ bool        isBlind;
      /*0x124*/ u8          maelstromTimer;
      /*0x125*/ u8          _unused_0x125;    // ?? Might be afterburner timer or ultralisk roar timer
      /*0x126*/ u8          acidSporeCount;
      /*0x127*/ u8          acidSporeTime[9];
    } status;
    /*0x130*/ u16   bulletBehaviour3by3AttackSequence;  // Counts up for the number of bullets shot by a unit using
                                                        // this weapon behaviour and resets after it reaches 12

    u16   _padding_0x132;   // 2-byte padding

    /*0x134*/ void* pAI;            // pointer to AI class, we're not using this though  // official name
    /*0x138*/ u16   airStrength;
    /*0x13A*/ u16   groundStrength;
    struct
    {             // Official names are "posSortXL, posSortXR, posSortYT, posSortYB"
      u32 left, right, top, bottom; // Ordering for unit boundries in unit finder for binary search
    } finder;
    /*0x14C*/ u8    _repulseUnknown;        // @todo Unknown
    /*0x14D*/ u8    repulseAngle;           // updated only when air unit is being pushed
    /*0x14E*/ u8    bRepMtxX;              //  (mapsizex/1.5 max)   // repulse matrix X/Y
    /*0x14F*/ u8    bRepMtxY;              //  (mapsizex/1.5 max)

    ////////////////////////////////////////////////////////////////////
    // Official Broodwar methods (from beta), ignore these
    /*
    bool  IsAddOn() const;
    bool  IsATransport() const;
    bool  IsBuilding() const;     // Checks unit status flags
    bool  IsBuildingEx() const;   // Checks unit type (special ability flags)
    bool  IsBurrower() const;
    bool  IsCarrier() const;
    bool  IsCarrierType() const;
    bool  IsCloakable() const;
    bool  IsCloaker() const;
    bool  IsCompleted() const;
    bool  IsConstructing() const;
    bool  IsDead() const;
    bool  IsDoodad() const;
    bool  IsDoorType() const;
    bool  IsFighterType() const;
    bool  IsGhostType() const;
    bool  IsGoliathHead() const;
    bool  IsHallucination() const;
    bool  IsHidden() const;
    bool  IsJuiceMaker() const;     // same as IsPylon
    bool  IsMedic() const;
    bool  IsMobileBuilding() const;
    bool  IsMoving() const;
    bool  IsNydusCanal() const;
    bool  IsPowerup() const;
    bool  IsPylonType() const;
    bool  IsQueenType() const;
    bool  IsRallyer() const;
    bool  IsReaver() const;
    bool  IsRevealer() const;
    bool  IsResourceDest() const;
    bool  IsResourceSrc() const;
    bool  IsScanner() const;
    bool  IsSelected() const;
    bool  IsShieldBattery() const;
    bool  IsSiloType() const;
    bool  IsTemplar() const;
    u16   IsTrainingUnit(int id) const;
    bool  IsTurret() const;
    bool  IsVulture() const;
    bool  IsWorker() const;
    bool  IsZergBldgUpgrading() const;
    bool  IsZergTownhall() const;
    
    bool  HasAI() const;
    CAIControl  *GetAI() const;
    
    CUnit *GetAddOn() const;
    u16   GetAddOn2BldType() const;
    CUnit *GetAttached() const;
    u8    GetBGOrder() const;
    int   GetFixedMaxHP() const;
    int   GetHangarCapacity() const;
    int   GetMaxHP() const;
    int   GetNumInterceptors() const;
    u8    GetOrder() const;
    u8    GetOwner() const;
    u16   GetType() const;
    u16   GetUnitID() const;
    u8    GetWeaponGndEx() const;

    bool  HasGas() const;
    bool  HasMinerals() const;
    bool  HasNuke() const;
    bool  HasPath() const;

    bool  CompletionCounted() const;
    bool  ExistenceCounted() const;
    COrder  *PeekOrder() const;
    CUnit *Target() const;
    bool  CantBeHit() const;
    bool  AtMoveTarget() const;
    bool  CanEnterTransport(CUnit *trans) const;
    bool  InTransport() const;
    int   UnitPlaceboxWidth() const;  // Also has a global version taking unit type as param
    int   UnitPlaceboxHeight() const; // Also has a global version taking unit type as param
    int   ProvidedFood() const;
    int   TownSpot() const;   // resource base ID (0-250)
    int   X() const;
    int   Y() const;
    void  FindFree_FromUnit(Position *pResult, CUnit *) const;
    bool  CanTarget(CUnit *pTarg, bool) const;

    void  DispatchEnterTransport();
    void  DispatchDecloak();
    void  RunBGOrders();
    void  RunTransportOrders();
    void  RunBGTransportOrders();
    SetOrderEx
    AppendOrderEx
    void FixupAIPointer(bool);
    void UnfixupAIPointer(bool);
    void RemoveFromStatusBar();
    void AddToHarvestWaitingList(CUnit *pWorker);
    void PlaceUnplaceBldg(short x, short y, bool);

    bool  StdTargetTracking();
    void  PathCalcMove();
    bool  CheckResolveCollision();
    bool  SetMoveTarget_CUnit(CUnit *pUnit);
    bool  SetMoveTarget_xy(int x, int y);
    bool  ApproachMoveTarget_CUnit(CUnit *pUnit);
    bool  ApproachMoveTarget_xy(int x, int y);
    void  SetSubTarget(int x, int y);

    CUnit *NextNode() const;
    CUnit *PrevNode() const;
    bool  OnUsedList() const;   // scrapped (used in debug)
    bool  OnTransList() const;  // scrapped
    bool  OnFreeList() const;   // scrapped
    bool  OnDeadList() const;   // scrapped

    */
  };

  static_assert( sizeof(CUnit) == 336, "BW::CUnit is incorrect." );
};
#pragma pack()

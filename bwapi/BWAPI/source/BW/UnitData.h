#pragma once

#include "../Types.h"
#include "../StaticAssert.h"

#include "../BW/OrderID.h"
#include "../BW/UnitTypes.h"
#include "../BW/MovementFlags.h"
#include "../BW/Position.h"
#include "../BW/UnitStatusFlags.h"
#include "../BW/ParasiteFlags.h"

namespace BW { struct Sprite; };
namespace BW { struct Order; };

namespace BW
{
  const u32 UNIT_SIZE_IN_BYTES = 336;
  const u32 UNIT_ARRAY_MAX_LENGTH = 1700;

  /** 
  * Direct data mapping of the broodwar unit structure. Most of the information here is derived
  * from http://farty1billion.dyndns.org/EUDDB/?pg=ref&a=unitnode
  * Values marked TODO: Unknown are labeled as the EUDDB has them labeled, but the true meaning 
  * is not understood. Values marked TODO: Verify have known meanings, but are not confirmed.
  */
  #pragma pack(1)
  struct UnitData
  {
    /*0x000*/ BW::UnitData*           previousUnit;
    /*0x004*/ BW::UnitData*           nextUnit;
    /*0x008*/ u8                      healthPointsFraction;  
    /*0x009*/ u16                     healthPoints;
    /*0x00B*/ _UNKNOWN _1[1];
    /*0x00C*/ BW::Sprite*             sprite;
    /*0x010*/ BW::Position            moveToPos;
    /*0x014*/ BW::UnitData*           targetUnit;           // TODO: Verify
    /*0x018*/ _UNKNOWN _2[8];
    /*0x020*/ BW::MovementFlags::Enum movementFlags;        // TODO: Verify
    /*0x021*/ _UNKNOWN _3[1];
    /*0x022*/ u8                      flingyTurnRadius;     // TODO: Unknown
    /*0x023*/ u8                      currentDirection;     // TODO: Unknown
    /*0x024*/ u16                     flingyID;             // TODO: Unknown
    /*0x026*/ _UNKNOWN _4[1];
    /*0x027*/ u8                      flingyMovementType;   // TODO: Unknown
    /*0x028*/ BW::Position            currentPos;
    /*0x02C*/ u32                     xHalt;                // TODO: Unknown
    /*0x030*/ u32                     yHalt;                // TODO: Unknown
    /*0x034*/ u32                     flingySpeed;          // TODO: Unknown
    /*0x038*/ u32                     unknownSpeed_0x038;   // TODO: Unknown
    /*0x03C*/ u32                     unknownSpeed_0x03C;   // TODO: Unknown
    /*0x040*/ _UNKNOWN _5[8];
    /*0x048*/ u16                     flingyAcceleration;   // TODO: Unknown
    /*0x04A*/ _UNKNOWN _6[2];
    /*0x04C*/ u8                      playerID;
    /*0x04D*/ BW::OrderID::Enum       orderID;
    /*0x04E*/ u8                      mainOrderState;       // TODO: Unknown
    /*0x04F*/ u8                      orderSignal;          // TODO: Unknown
    /*0x050*/ _UNKNOWN _7[4];
    /*0x054*/ u8                      mainOrderTimer;       // TODO: Unknown
    /*0x055*/ u8                      groundWeaponCooldown;
    /*0x056*/ u8                      airWeaponCooldown;    // TODO: Verify
    /*0x057*/ u8                      spellCooldown;        // TODO: Verify
    /*0x058*/ BW::Position            orderTargetPos;       // TODO: Verify
    /*0x05C*/ BW::UnitData*           orderTargetUnit;      // TODO: Verify
    /*0x060*/ u8                      shieldPointsFraction;
    /*0x061*/ u16                     shieldPoints;
    /*0x063*/ _UNKNOWN _8[1];
    /*0x064*/ BW::UnitType::Enum      unitID;
    /*0x066*/ _UNKNOWN _9[2];
    /*0x068*/ BW::UnitData*           previousPlayerUnit; // TODO: Verify
    /*0x06C*/ BW::UnitData*           nextPlayerUnit;     // TODO: Verify
    /*0x070*/ BW::UnitData*           subUnit;            // TODO: Verify
    /*0x074*/ BW::Order*              orderQueueHead;     // TODO: Verify
    /*0x078*/ BW::Order*              orderQueueTail;     // TODO: Verify
    /*0x07C*/ _UNKNOWN _10[4];
    /*0x080*/ BW::UnitData*           connectedUnit;      // TODO: Verify
    /*0x084*/ u8                      orderQueueCount;    // TODO: Verify
    /*0x085*/ u8                      unknownOrderTimer_0x085; // TODO: Unknown
    /*0x086*/ _UNKNOWN _11[2];
    /*0x088*/ u16                     displayedUnitID;    // TODO: Verify
    /*0x08A*/ _UNKNOWN _12[4];
    /*0x08E*/ u8                      rankIncrease;       // TODO: Unknown
    /*0x08F*/ u8                      killCount;          // TODO: Verify
    /*0x090*/ _UNKNOWN _13[3];
    /*0x093*/ u8                      unknownState_0x93;  // TODO: Unknown
    /*0x094*/ u8                      currentButtonSet;   // TODO: Verify
    /*0x095*/ _UNKNOWN _14[3];
    /*0x098*/ BW::UnitType::Enum      queue[5];
    /*0x0A2*/ u16                     energy;             // TODO: Verify
    /*0x0A4*/ u8                      queueSlot;          // TODO: Unknown
    /*0x0A5*/ _UNKNOWN _15[1];
    /*0x0A6*/ u8                      secondaryOrderID;   // TODO: Unknown (Cloak, Build, ExpandCreep suggested by EUDDB)
    /*0x0A7*/ _UNKNOWN _16[1];
    /*0x0A8*/ u16                     hpGainDuringRepair; // TODO: Verify
    /*0x0AA*/ _UNKNOWN _17[2];
    /*0x0AC*/ u16                     remainingBuildTime; // TODO: Verify
    /*0x0AE*/ _UNKNOWN _18[18];
    /*0x0C0*/ union ChildInfoUnion_type
              {
                struct UnitIsVultureBike_type
                {
                  u8 spiderMineCount;
                  _UNKNOWN _1[3];
                } vultureBikeMines;

                // If Carrier/Reaver - First Hangar Unit
                // If Scarab/Interceptor - Parent
                // If Building - Addon
                // If Worker - Powerup Carried
                BW::UnitData* childUnit1;
              }                       childInfoUnion;     // TODO: Verify
    /*0x0C4*/ union ChildUnitUnion1_type
              {
                struct UnitIsBuilding_type
                {
                  u16 addonBuildID;
                  u16 upgradeResearchTime;
                } unitIsBuilding;

                // If Scarab/Interceptor - Next Unit in Parent Hangar
                BW::UnitData* nextUnitInParentHangar;
              }                       childUnitUnion1;    // TODO: Verify
    /*0x0C8*/ union ChildUnitUnion2_type
              {
                struct UnitIsNotScarabInterceptor_type
                {
                  union SubChildUnitUnion1_type
                  {
                    u8 scarabCount;
                    u8 techID;
                  } subChildUnitUnion1;
                  union SubChildUnitUnion2_type
                  {
                    u8 interceptorCount;
                    u8 upgradeID;
                  } subChildUnitUnion2;
                  u8 larvaSpawnTimer;
                  u8 isLanding;
                } unitIsNotScarabInterceptor;

                // IF Scarab/Interceptor - Previous in Parent's Hangar
                BW::UnitData* prevUnitInParentHangar;                
              }                       childUnitUnion2;    // TODO: Verify
    /*0x0CC*/ union ChildUnitUnion3_type
              {                
                u8 inHanger; // If interceptor/scarab
                u8 creepExpansionTimer; // If creep unit, timer between creep expansions
                u8 decreaseMineralTimer; // If repairing
              }                       childUnitUnion3;    // TODO: Verify
    /*0x0CD*/ u8                      upgradeLevel;       // TODO: Unknown
    /*0x0CE*/ u8                      isCarryingSomething; // TODO: Verify (may be set if going to carry something or targetting resources.. If 'isgathering' ?
    /*0x0CF*/ u8                      resourceAmount;     // TODO: Unknown (probably the amount of resources it is carrying?)
    /*0x0D0*/ union UnitUnion1_type
              {
                union UnitUnion1Sub_type
                {
                  BW::Position powerupDropTarget;
                  struct ResourceUnitUnionSub_type{
                    u16 resourceContained;
                    u8 resourceIsScript;
                    u8 resourceCount;
                  } resourceUnitUnionSub;
                } unitUnion1Sub;
                BW::UnitData* resourceTarget_connectedNydus;
                BW::Sprite* nukeDot;
              }                       unitUnion1;         // TODO: Verify
    /*0x0D4*/ _UNKNOWN _19[8];
    /*0x0DC*/ BW::StatusFlags::Enum   status;             // TODO: Verify
    /*0x0E0*/ u8                      resourceType;       // TODO: Unknown (type of resource this unit is?)
    /*0x0E1*/ u8                      wireframeRandomizer;// TODO: Unknown
    /*0x0E2*/ u8                      secondaryOrderState;// TODO: Unknown
    /*0x0E3*/ u8                      unknownCounterDown_0x0E3; // TODO: Unknown
    /*0x0E4*/ _UNKNOWN _20[8];
    /*0x0EC*/ BW::UnitData*           currentBuildUnit;   // TODO: Unknown
    /*0x0F0*/ _UNKNOWN _21[8];
    /*0x0F8*/ union RallyPsiProviderUnion_type
              {
                struct Rally_type
                {
                  BW::Position rallyPos;
                  BW::UnitData* rallyUnit;
                } rally;
                struct PsiProvider_type
                {
                  BW::UnitData* prevPsiProvider;
                  BW::UnitData* nextPsiProvider;
                };
              }                       rallyPsiProviderUnion; // TODO: Verify
    /*0x100*/ u32                     pathUnknown_0x100;  // TODO: Unknown
    /*0x104*/ _UNKNOWN _22[3];
    /*0x107*/ u8                      isBeingHealed;      // TODO: Verify
    /*0x108*/ BW::Position            contours1Unknown;   // TODO: Unknown
    /*0x10A*/ BW::Position            contours2Unknown;   // TODO: Unknown
    /*0x110*/ u16                     removeTimer;        // TODO: Verify (Hallucination, DSwarm, DWeb, Broodling)
    /*0x112*/ u16                     defenseMatrixDamage;// TODO: Verify
    /*0x114*/ u8                      defenseMatrixTimer; // TODO: Verify
    /*0x115*/ u8                      stimTimer;          // TODO: Verify
    /*0x116*/ u8                      ensareTimer;        // TODO: Verify
    /*0x117*/ u8                      lockdownTimer;      // TODO: Verify
    /*0x118*/ u8                      irradiateTimer;     // TODO: Verify
    /*0x119*/ u8                      stasisTimer;        // TODO: Verify
    /*0x11A*/ u8                      plaugeTimer;        // TODO: Verify
    /*0x11B*/ u8                      isUnderStorm;       // TODO: Verify
    /*0x11C*/ BW::UnitData*           irradiatedBy;       // TODO: Verify
    /*0x120*/ u8                      irradiatePlayerID;  // TODO: Verify
    /*0x121*/ BW::ParasiteFlags::Enum parasiteFlags;      // TODO: Verify
    /*0x122*/ u8                      cycleCounter;       // TODO: Verify (runs updates approx 2 times per sec)
    /*0x123*/ u8                      isBlind;            // TODO: Verify
    /*0x124*/ u8                      maelstromTimer;     // TODO: Verify
    /*0x125*/ _UNKNOWN _23[1];
    /*0x126*/ u8                      acidSporeTime[9];   // TODO: Verify
    /*0x130*/ u16                     offsetIndex3by3;    // TODO: Unknown
    /*0x132*/ _UNKNOWN _24[6];
    /*0x138*/ u16                     airStrength;        // TODO: Verify (1 if has no air weapon but has a ground)
    /*0x13A*/ u16                     groundStrength;     // TODO: Verify (1 if has no ground weapon but has an air)
    /*0x13C*/ _UNKNOWN _25[16];
    /*0x14C*/ u8                      repulseUnknown1;    // TODO: Unknown
    /*0x14D*/ u8                      repulseUnknown2;    // TODO: Unknown
    /*0x14E*/ u8                      driftPosX;          // TODO: Unknown (mapsizex/1.5 max)
    /*0x14F*/ u8                      driftPosY;          // TODO: Unknown (mapsizex/1.5 max)
    /*0x150*/ _UNKNOWN _26[1];
  };
  #pragma pack()

  struct UnitArray
  {
    UnitData unit[UNIT_ARRAY_MAX_LENGTH];
  };

  BOOST_STATIC_ASSERT(sizeof(UnitData) == UNIT_SIZE_IN_BYTES);
  BOOST_STATIC_ASSERT(sizeof(UnitArray) == UNIT_SIZE_IN_BYTES * UNIT_ARRAY_MAX_LENGTH);
};


#pragma once

#include "Types.h"
#include "BW_UnitTypes.h"
#include "StaticAssert.h"

struct BW_Sprite;
struct BW_Order;

struct BW_Position
{
   u16 X;
   u16 Y;
};

namespace BW_MovementFlags
{
   enum Enum : u8
   {
      MakingBuilding = 0x02,
      Moving         = 0x04
   };
};

#pragma pack(1)
struct BW_Unit
{
   /*0x000*/BW_Unit*                PrevUnit;
   /*0x004*/BW_Unit*                NextUnit;
   /*0x008*/_UNKNOWN _0[1];  
   /*0x009*/u16                     HealthPoints;
   /*0x00B*/_UNKNOWN _1[1];
   /*0x00C*/BW_Sprite*              Sprite;
   /*0x010*/BW_Position             MoveTo;
   /*0x014*/BW_Unit*                TargetUnit;
   /*0x018*/_UNKNOWN _2[8];
   /*0x020*/u8                      MovementFlags; // TODO - figure out why compilation error if i use the enum here
   /*0x021*/_UNKNOWN _3[7];
   /*0x028*/BW_Position             CurrentPos;
   /*0x02C*/_UNKNOWN _4[32];
   /*0x04C*/u8                      PlayerID;
   /*0x04D*/_UNKNOWN _5[23];
   /*0x064*/BW_UnitType::Enum       UnitID;
   /*0x066*/_UNKNOWN _6[234];

//_UNKNOWN _LazyFiller1[88];
   //    u8 GroundWeaponCooldown;
   //    u8 AirWeaponCooldown;
   //    u8 SpellCooldown;
   //    u16 OrderTargetX;
   //    u16 OrderTargetY;
   //    BW_Unit* Target_Order;
   //    u32 Shield;
   //    u16 UnitID;
   //    BW_Unit* PlayerPrev;
   //    BW_Unit* PlayerNext;
   //    BW_Unit* SubUnit;
   //    BW_Order* OrderQueueHead;
   //    BW_Order* OrderQueueTail;
   //    u8 _UNKNOWN6[4];
   //    BW_Unit* ConnectedUnit;
   //    u8 NumOfQueuedOrders;
   //    u8 Unkown_85_Order_Timer_Seprate;
   //    u8 _UNKNOWN7[2];
   //    u16 DisplayedUnitID;
   //    u8 _UNKNOWN8[4];
   //    u8 RankIncrease;
   //    u8 KillCount;
   //    u8 _UNKNOWN9[3];
   //    u8 UnknownState;
   //    u8 CurrentButtonSet;
   //    u16 Queue1;
   //    u16 Queue2;
   //    u16 Queue3;
   //    u16 Queue4;
   //    u16 Queue5;
   //    u16 Energy;
   //    u8 QueueSlot;
   //    u8 _UNKNOWN10[1];
   //    u8 SecondaryOrderID;
   //    u16 HPGainedDuringBuild;
   //    u16 RemainingBuildTime;
   //    union ChildUnion_type
   //    {
   //       struct UnitIsVultureBike_type
   //       {
   //          u8 SpiderMineCount;
   //          u8 Unused[3];
   //       } UnitIsVultureBike;
   //       BW_Unit* Unit_Unknown;
   //       // IF Carrier/Reaver - First Hangar Unit
   //       // IF Scarab/Interceptor - Parent
   //       // IF Building - Addon
   //       // IF Worker - Powerup Carried
   //    } ChildUnion;
   //    union ChildUnion2_type
   //    {
   //       struct UnitIsBuilding_type
   //       {
   //          u16 AddonBuildID;
   //          u16 UpgradeResearchTime;
   //       } UnitIsBuilding;
   //       BW_Unit* NextUnitInParentHangar;
   //       // IF Scarab/Interceptor - Next Unit in Parent Hangar
   //    } ChildUnion2;
   //    union ChildUnion3_type
   //    {
   //       struct UnitIsNotScarabInterceptor 
   //       {
   //          u8 ScarabCount_TechID;
   //          u8 InterceptorCount_UpgradeID;
   //          u8 LarvaSpawnTimer;
   //          u8 IsLanding;
   //       };
   //       BW_Unit* PrevUnitInParentHangar;
   //       // IF Scarab/Interceptor - Previous in Parent's Hangar
   //    } ChildUnion3;
   //    u8 MultiUse1;
   //    // -IF Interceptor/Scarab - InHanger?
   //    // -IF CreepUnit - timer between creep expansions
   //    // -IF Repairing - decrease mineral timer
   //    u8 UpgradeLevel;
   //    u8 IsCarryingSomething;
   //    u8 ResourceAmount;
   //    u8 DONOTCARE1[4];
   //    u32 Status;
   //    //       00000001(0x1) - Is Completed
   //    //       00000010(0x2) - Is on ground? or is it is unit...
   //    //       00000100(0x4) - Is in air
   //    //       00001000(0x8) - Checked for disabled, if it is 00001000, then the unit is disabled(/unpowered?)
   //    //       00010000(0x10) - Checked for burrowing purposes, if it is 00010000, then the unit is burrowed
   //    //       00100000(0x20) - Unit is entering a building
   //    //       01000000(0x40) - unit is entering a transport
   //    //       10000000(0x80) - 
   //    // 
   //    //       00000001(0x100) - Checked for invisible purposes, if it is 00000001, then the unit requires a detector?
   //    //       00000010(0x200) - checked for cloak?
   //    //       00000100(0x400) - deals with doodad states? if set, is disabled
   //    //       00001000(0x800) - Unit cloaking doesn't need energy decrease
   //    //       00010000(0x1000) - Unit is in unbreakable code section? Cannot receive orders
   //    //       00100000(0x2000) - Set by nobrkcodestart
   //    //       01000000(0x4000) - 
   //    //       10000000(0x8000) - cannot attack if set
   //    // 
   //    //       00000001(0x10000)
   //    //       00000010(0x20000) - Is a Building?
   //    // 
   //    //       00000100(0x4000000) - Invincible
   //    // 
   //    //       00010000(0x10000000) - Speed upgrade
   //    //       00100000(0x20000000) - cooldown upgrade
   //    u8 ResourceType;
   //    u8 WireframeRandomizer;
   //    u8 SecondaryOrderState;
   //    u8 Unknown_E3_Counter_Down;
   //    u8 _UNKNOWN11[8];
   //    BW_Unit* CurrentBuildUnit;
   //    u8 _UNKNOWN12[8];
   //    union RallyPsiProviderUnion_type
   //    {
   //       struct Rally_type
   //       {
   //          u16 RallyX;
   //          u16 RallyY;
   //          BW_Unit* RallyUnit;
   //       } Rally;
   //       struct PsiProvider_type
   //       {
   //          BW_Unit* PrevPsiProvider;
   //          BW_Unit* NextPsiProvider;
   //       };
   //    } RallyPsiProviderUnion;
   //    u32 Path_Unknown;
   //    u8 _UNKNOWN13[3];
   //    u8 IsBeingHealed;
   //    u16 X_Countours_1_Unknown;
   //    u16 Y_Countours_1_Unknown;
   //    u16 X_Countours_2_Unknown;
   //    u16 Y_Countours_2_Unknown;
   //    u16 RemoveTimer;
   //    // Hallucination, DSwarm, DWeb, Broodling
   //    u16 DefenseMatrixDamage;
   //    u8 DefenseMatrixTimer;
   //    u8 StimTimer;
   //    u8 EnsareTimer;
   //    u8 LockdownTimer;
   //    u8 IrradiateTimer;
   //    u8 StasisTimer;
   //    u8 PlaugeTimer;
   //    u8 IsUnderStrom;
   //    // Tell if a unit is under a psi storm
   //    BW_Unit* IrradiatedBy;
   //    u8 IrradiatePlayerID;
   //    u8 ParasiteFlags;
   //    // Each bit corresponds to a player who parasites this unit
   //    u8 CycleCounter;
   //    // Runs updates 2 times per sec (approx)
   //    u8 IsBlind;
   //    u8 MaelstromCounter;
   //    u8 _UNKNOWN14[1];
   //    u8 AcidSporeCount;
   //    u8 bAcidSpore1Time;
   //    u8 bAcidSpore2Time;
   //    u8 bAcidSpore3Time;
   //    u8 bAcidSpore4Time;
   //    u8 bAcidSpore5Time;
   //    u8 bAcidSpore6Time;
   //    u8 bAcidSpore7Time;
   //    u8 bAcidSpore8Time;
   //    u8 bAcidSpore9Time;
   //    u16 ThreeByThreeOffsetIndex;
   //    u8 _UNKNOWN15[6];
   //    u16 AirStrength; 
   //    // 1 if has no air weapon, but has a ground
   //    u16 GroundStrength;
   //    // 1 if has no ground weapon, but has an air
   //    u8 _UNKNOWN16[16];
   //    u8 Repulse_Unknown1;
   //    u8 Repulse_Unknown2;
   //    u8 DriftPosX; //mapsizex/1.5 max
   //    u8 DriftPosY; //mapsizex/1.5 max
};
#pragma pack()

struct BW_UnitArray
{
   BW_Unit Unit[1700];
};

BOOST_STATIC_ASSERT(sizeof(BW_Unit) == 336);
BOOST_STATIC_ASSERT(sizeof(BW_UnitArray) == 336*1700);




// void RecursePrevUnit(HANDLE& hProcess, BW_Unit& curUnit)
// {
//    if (curUnit.UnitID == BW_UnitType::SCV)
//    {
//       recuseDepth = recuseDepth;
//    }
// 
//    unitArray.Unit[recuseDepth] = curUnit;
// 
//    if (curUnit.PrevUnit == NULL)
//    {
//       recuseDepth = 0;
//       return;
//    }
// 
//    ++recuseDepth;
//    SIZE_T bytesRead;
//    BW_Unit prevUnit;
//    ReadProcessMemory(hProcess, (void*)curUnit.PrevUnit, (void*)&prevUnit, sizeof(BW_Unit), &bytesRead);
//    RecursePrevUnit(hProcess, prevUnit);
// }
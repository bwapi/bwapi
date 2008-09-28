#pragma once
#pragma pack(1)

#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"

#include "UnitPrototypeFlags.h"
#include "MiniTileFlags.h"
#include "GroupFlags.h"
#include "PlayerType.h"
#include "Race.h"

/**
 * Broodwar content access tools. The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */
namespace BW
{
  static const u8  PLAYER_COUNT          =  12;
  static const u8  PLAYABLE_PLAYER_COUNT =   8;
  static const u8  RACE_COUNT            =   3;
  static const u8  UNIT_TYPE_COUNT       = 228;
  static const u8  TECH_TYPE_COUNT       =  44;
  static const u8  UPGRADE_TYPE_COUNT    =  66;
  static const u16 SUPPLY_LIMIT          = 400; /**< We could read this value from memory as it is stored in 
                                                 * #Supplies#Max, but as it is always constant for every race
                                                 * for every player, I prefer using this constant. */
  static const int TILE_SIZE          =  32;
 
  struct Unit;
  struct UnitArray;
  class TileType;
  class DoodatType;
  //-------------------------------------------- PLAYER RESOURCES --------------------------------------------
  /** Direct mapping of players resource amount in the bw memory */
  struct PlayerResources
  {
    /** Direct mapping of resource value of some player (gas or minerals) */
    struct PlayerResource
    {
      s32 player[PLAYER_COUNT];
    };
    PlayerResource minerals;
    PlayerResource gas;
  };
  static PlayerResources* BWXFN_PlayerResources = (PlayerResources*) 0x0057F0D8;

  //----------------------------------------------- NAMES TYPE -----------------------------------------------
  /** Direct mapping of players info in bw memory */
  struct Players
  {
    /** Direct mapping of player info in bw memory */
    struct PlayerInfo
    {
      PlayerType::Enum type;
      Race::Enum       race;
      u8               force;
      char             name[23];
      u32              playerNumber; /**< @todo unknown */
      u32              unknown; /** @todo unkown */
      u16              unknown2; /** @todo unkown */
    };
    PlayerInfo player[PLAYER_COUNT - 1]; /**< Player 12 doesn't have the info */
  };

  struct Positions
  {
    /** Direct mapping of player info in bw memory */
    u16   x;
    u16   y;
  };

  static u32 BWXFN_PRIMARY = 0x00581DCC;      // Total Units Produced\Owned is first entry
  static Positions* startPositions = (Positions*) (BWXFN_PRIMARY + 0xB93C);

  static Players* BWXFN_Players = (Players*) 0x0057EEE8;

  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  static TileID**       BWXFN_MapTileArray = (TileID**) 0x005993AC;
  static TileType**     BWXFN_TileSet = (TileType**) 0x006D5EA8; /**< Index  0-1023 */
  static DoodatType**   BWXFN_DoodatSet = (DoodatType**) 0x006D5EA8; /**< Index 1024 + */
  static u16*           BWXFN_MapSizeX = (u16*) 0x0057F1BC;
  static u16*           BWXFN_MapSizeY = ((u16*) BWXFN_MapSizeX) + 1;
  static Unit**         BWXFN_UnitNodeTable_FirstElement = (Unit**)0x00628418;
  static UnitArray*     BWXFN_UnitNodeTable = (UnitArray*) 0x0059CB40;
  const  u32            UNIT_ARRAY_MAX_LENGTH = 1701;
                 
  static void (_stdcall* selectUnits)(int count, BW::Unit ** unitsToSelect) = (void (_stdcall*) (int, BW::Unit * *)) 0x004C0530; /** 1.15.3 */
  static void (_stdcall* selectUnitsHelperSTD)(int, BW::Unit **, bool, bool) = (void (_stdcall*) (int, BW::Unit * *, bool, bool)) 0x0049ACD0; /** 1.15.3 */
  static u32            BWFXN_CommandUnit = 0x4BFFD0; /** 1.15.3 */
  static u32            BWFXN_IssueCommand = 0x485920; /** 1.15.3 */
  static u32            BWFXN_HUD = 0x004202A0; 
  static u32*           BWFXN_InGame = (u32*) 0x6556C8;
  static u32            BWFXN_CountDownTimer = 0x0058D6DC; /**< @todo verify */
  static u32            BWXFN_PrintText = 0x0048CE90; /** 1.15.3 */
  //static u32            BWXFN_PrintPublicText = 0x4F2EC0; // Doesn't work now
  static u32            BWXFN_PrintPublicText = 0x004C2090; // Not working now
  static u32            BWXFN_SendPublicCall = 0x004F2F57; /** 1.15.3 */
  static u32            BWXFN_SendPublicCallBack = BWXFN_SendPublicCall + 5;
  static u32            BWXFN_SendPublicCallTarget = 0x004C20E0; /** 1.15.3 */
  //static int            BWXFN_Refresh = 0x41DE10; // Is some function, not tested yet (4 int arguments)
  static Unit*          BWXFN_CurrentPlayerSelectionGroup = (Unit*) 0x005971F0;
  static u32            BWXFN_GameStart = 0x004C96F1; /** 1.15.3 */
  static u32            BWXFN_GameStartBack = BW::BWXFN_GameStart + 5;
  static u32            BWXFN_GameStartTarget = 0x00417C40; /** 1.15.3 */
  static u32            BWXFN_GameEnd = 0x004EE623;/** 1.15.3 */
  static u32            BWXFN_GameEndBack = BWXFN_GameEnd + 5;
  static u32            BWXFN_GameEndTarget = 0x00416DB0; /** 1.15.3 */
  static u32            BWXFN_NextFrameHelperFunction = 0x004D954D; /** 1.15.3 */
  static u32            BWXFN_NextFrameHelperFunctionBack = BWXFN_NextFrameHelperFunction + 5;
  static u32            BWXFN_NextFrameHelperFunctionTarget = 0x004D1160; /** 1.15.3 */
  static u32            BWXFN_Refresh = 0x0041DE10; /**< or 0x0041E040 ? */
  static u32            BWXFN_CancelTrainByClickInTheQueue = 0x00457429; /** 1.15.3 */
  static u32            BWXFN_CancelTrainByClickInTheQueueBack = BWXFN_CancelTrainByClickInTheQueue + 5;
  static u32            BWXFN_CancelTrainByClickInTheQueueTarget = BWFXN_IssueCommand;
  static u32            BWXFN_CancelTrainByEscape = 0x004234C4; /** 1.15.3 */
  static u32            BWXFN_CancelTrainByEscapeBack = BWXFN_CancelTrainByEscape + 5;
  static u32            BWXFN_CancelTrainByEscapeTarget = BWFXN_IssueCommand;
  static u32            BWXFN_RemoveUnit = 0x004EC194; /** 1.15.3 */
  static u32            BWXFN_RemoveUnitBack = BWXFN_RemoveUnit + 5;
  static u32            BWXFN_RemoveUnitTarget = 0x004798D0; /** 1.15.3 */
  static u32            BWXFN_Redraw = 0x004D95C3; /** 1.15.3 */
  static u32            BWXFN_RedrawBack = BWXFN_Redraw + 5;
  static u32            BWXFN_RedrawTarget = 0x0041CA20; /** 1.15.3 */
  static u32            BWXFN_NextLogicFrame = 0x004D93DE; /** 1.15.3 */
  static u32            BWXFN_NextLogicFrameBack = BWXFN_NextLogicFrame + 5;
  static u32            BWXFN_NextLogicFrameTarget = 0x00488450; /** 1.15.3 */
  static int*           BWXFN_MouseX = (int*) 0x006CDDAC;
  static int*           BWXFN_MouseY = (int*) 0x006CDDB0;
  static int*           BWXFN_ScreenX = (int*) 0x00628430;
  static int*           BWXFN_ScreenY = (int*) 0x00628458;
  static char*          BWXFN_CurrentMapFileName = (char*) 0x0057FD24;
  static char**         BWXFN_StringTable = (char**) 0x006D1220;
  static u16*           BWXFN_TechLabelIndex = (u16*) 0x00656288;
  static u16*           BWXFN_UpgradeLabelIndex = (u16*) 0x00655A28;
  static u8*            BWXFN_UpgradeMax = (u8*) 0x006556E8;
  static u16*           BWXFN_UpgradeMineralCostBase = (u16*) 0x00655728;
  static u16*           BWXFN_UpgradeMineralCostFactor = (u16*) 0x006559A8;
  static u16*           BWXFN_UpgradeGasCostBase = (u16*) 0x00655828;
  static u16*           BWXFN_UpgradeGasCostFactor = (u16*) 0x006557A8;
  static u16*           BWXFN_UpgradeTimeCostBase = (u16*) 0x00655B68;
  static u16*           BWXFN_UpgradeTimeCostFactor = (u16*) 0x00655928;
  static Race::Enum*    BWXFN_UpgradeRace = (Race::Enum*) 0x00655BE4;
  static u16**          BWXFN_StringTableIndex = (u16**) 0x6D1220;
  static u32**          BWXFN_MapFogOfWar = (u32**) 0x006D1248;
  
  struct UpgradeProgress
  {
    Util::BitMask<u64> player[PLAYER_COUNT];
  };
  static UpgradeProgress* BWXFN_UpgradeProgress = (UpgradeProgress*) 0x0058F3C8;

  //------------------------------------------------ SUPPLIES ------------------------------------------------
  struct Supplies
  {
    /** Direct mapping of available/Used/Max supplies for one race and all players */
    struct SuppliesPerRace
    {
      /** Direct mapping of some supply vale for all players*/
      struct SupplyValues
      {
        s32 player[PLAYER_COUNT];
      };
      SupplyValues available;
      SupplyValues used;
      SupplyValues max;
    };
    SuppliesPerRace race[RACE_COUNT];
  };
  static Supplies* BWXFN_Supplies    = (Supplies*) 0x0058212C;
  
  //------------------------------------------- UNIT MINERAL PRICES ------------------------------------------
  /** Direct mapping of unit types mineral prices. */
  struct MineralPrices_type
  {
    u16 mineralPrice[UNIT_TYPE_COUNT];
  };
  static MineralPrices_type* BWXFN_MineralPrices = (MineralPrices_type*) 0x00663870;
  //------------------------------------------- UNIT MINERAL PRICES ------------------------------------------
  /** Direct mapping of unit types gas prices. */
  struct GasPrices_type
  {
    u16 gasPrice[UNIT_TYPE_COUNT];
  };
  static GasPrices_type* BWXFN_GasPrices = (GasPrices_type*) 0x0065FCE8;
  //------------------------------------------ UNIT SUPPLY DEMANDS -------------------------------------------
  /** Direct mapping of unit supply demands. */
  struct SupplyDemands_type
  {
    u8 supplyDemand[UNIT_TYPE_COUNT];
  };
  static SupplyDemands_type* BWXFN_SupplyDemands = (SupplyDemands_type*) 0x00663CD0;
  //---------------------------------------------- UNIT MAX HP -----------------------------------------------
  /** Direct mapping of unit unit type (Max Health Points)/(Not Attackable)/(Requirable) specification. */
  struct MaxHealthPoints_NotAttackable_Repairable_type
  {
    /** mapping of the Max Health Points)/(Not Attackable)/(Requirable) for single unit type. */
    struct MaxHealthPoints_NotAttackable_Repairable_Internal_type
    {
      u16 maxHealthPoints;
      u8 notAttackable;
      u8 repairable;
    };
    MaxHealthPoints_NotAttackable_Repairable_Internal_type raw[UNIT_TYPE_COUNT];
  };
  static MaxHealthPoints_NotAttackable_Repairable_type* BWXFN_MaxHealthPoints_NotAttackable_Repairable = (MaxHealthPoints_NotAttackable_Repairable_type*) 0x00662339;
  //-------------------------------------------- UNIT MAX SHIELDS --------------------------------------------
  /** Direct mapping of unit unit type Shield points */
  struct MaxShieldPoints_type
  {
    u16 maxShieldPoints[UNIT_TYPE_COUNT];
  };
  static MaxShieldPoints_type* BWXFN_MaxShieldPoints = (MaxShieldPoints_type*) 0x00660DE8;
   //------------------------------------------- UNIT MAX SHIELDS --------------------------------------------
  /** Direct mapping of unit type armor */
  struct Armor_type
  {
    u8 armor[UNIT_TYPE_COUNT];
  };
  static Armor_type* BWXFN_Armor = (Armor_type*) 0x0065FEB0;

    //----------------------------------------- UNIT MAX BUILD TIME ------------------------------------------
  /** Direct mapping of unit build time*/
  struct BuildTime_type
  {
   u16 buildTime[UNIT_TYPE_COUNT];
  };
  static BuildTime_type* BWXFN_BuildTime = (BuildTime_type*) 0x00660410;

  //-------------------------------------------- UNIT DIRECTIONS ---------------------------------------------
  /** Direct mapping of unit unit type armor */
  struct UnitsDimensions_type
  {
    /** Dimensions of unit, it's the distance from the 'center' of unit to each border */
    struct UnitDimensions
    {
      u16 left;
      u16 up;
      u16 right;
      u16 down;
    };
    UnitDimensions units[UNIT_TYPE_COUNT];
  };
  static UnitsDimensions_type* BWXFN_UnitDimensions = (UnitsDimensions_type*) 0x006617B0;
  const u8 NoWeapon = 130;
  //-------------------------------------------- GROUND WEAPONS  ---------------------------------------------
  /** Direct mapping of unit unit type armor */
  struct UnitsGroundWeapon_type
  {
    u8 unit[UNIT_TYPE_COUNT];
  };
  static UnitsGroundWeapon_type* BWXFN_UnitGroundWeapon = (UnitsGroundWeapon_type*) 0x006636A0;

  const int weaponTypeCount = 130;
  //------------------------------------------ WEAPON DAMAGE FACTOR ------------------------------------------
  /** Direct mapping of unit unit type armor */
  struct WeaponsDamageFactor_type
  {
    u8 weapon[weaponTypeCount];
  };
  static WeaponsDamageFactor_type* BWXFN_WeaponDamageFactor = (WeaponsDamageFactor_type*) 0x006564C8;         

  //------------- WEAPON DAMAGE TYPE ----------------------                                      -------------
  /** Direct mapping of unit unit type armor */
  struct WeaponsDamage_type
  {
    u16 weapon[weaponTypeCount];
  };
  static WeaponsDamage_type* BWXFN_WeaponDamage = (WeaponsDamage_type*) 0x00656E98;

  //------------------------------------------------- FLAGS --------------------------------------------------
  /** Direct mapping of unit flags data */
  struct PrototypeFlags_type
  {
    Util::BitMask<UnitPrototypeFlags::Enum> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeFlags_type* BWXFN_UnitPrototypeFlags = (PrototypeFlags_type*) 0x00664068;

  //---------------------------------------------- GROUP FLAGS -----------------------------------------------
  /** Direct mapping of unit flags data */
  struct PrototypeGroupFlags_type
  {
    Util::BitMask<GroupFlags::Enum> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeGroupFlags_type* BWXFN_PrototypeGroupFlags = (PrototypeGroupFlags_type*) 0x663788;
  //------------------------------------------- TECH MINERAL COST --------------------------------------------
  struct TechCost
  {
    u16 tech[TECH_TYPE_COUNT];
  };
  static TechCost* BWXFN_TechMineralCost = (TechCost*) 0x00656230;
  static TechCost* BWXFN_TechGasCost = (TechCost*) 0x006561D8;
  static TechCost* BWXFN_TechEnergyCost = (TechCost*) 0x00656368;
  //------------------------------------------------ MAPPING -------------------------------------------------
  const u16 tileTypeCount = 65535;
  /** Direct mapping of minitile flags array */
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      Util::BitMask<MiniTileFlags::Enum> miniTile[16];
    };
    MiniTileFlagArray tile[tileTypeCount];
  };

  static MiniTileMaps_type** BWXFN_MiniTileFlags = (MiniTileMaps_type**) 0x005993B8;
};
#pragma pack()

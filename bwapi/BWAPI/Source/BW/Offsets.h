#pragma once
#pragma pack(1)

#include "../Types.h"
#include <Bitmask.h>
#include "UnitPrototypeFlags.h"
#include "MiniTileFlags.h"
#include "GroupFlags.h"

/** 
 * Broodwar content access tools.
 * The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */
namespace BW
{
  struct Unit;
  struct UnitArray;
  class TileType;
  class DoodatType;
  //--------------------------------- MINERALS TYPE -----------------------------
  /** Direct mapping of players mineral amount in the bw memory */
  struct PlayerMinerals_type
  {
    s32 player[12];
  };
  static PlayerMinerals_type* BWXFN_PlayerMinerals = (PlayerMinerals_type*)0x0057F0D8;

  //------------------------------------ GAS TYPE -------------------------------
  /** Direct mapping of players gas amount in the bw memory */
  struct PlayerGas_type
  {
    s32 player[12];
  };
  static PlayerGas_type* BWXFN_PlayerGas = (PlayerGas_type*)0x0057F108;

  //----------------------------------- NAMES TYPE ------------------------------
  /** Direct mapping of players names in the bw memory */
  struct PlayerNames_type
  {
    /** Direct mapping of player name in bw memory. */
    struct PlayerName
    {
      char name[36];
    };
    PlayerName player[8];
  };
  static PlayerNames_type* BWXFN_PlayerNames = (PlayerNames_type*) 0x0057EEEB;

  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  static TileID**      BWXFN_MapTileArray = (TileID**) 0x005993AC;
  static TileType**    BWXFN_TileSet = (TileType**) 0x006D5EA8; /**< Index  0-1023 */
  static DoodatType**  BWXFN_DoodatSet = (DoodatType**) 0x006D5EA8; /**< Index 1024 + */
  static u16*          BWXFN_MapSizeX = (u16*) 0x0057F1BC;
  static u16*          BWXFN_MapSizeY = ((u16*) BWXFN_MapSizeX) + 1;
  static Unit**        BWXFN_UnitNodeTable_FirstElement = (Unit**)0x00628418;
  static UnitArray*    BWXFN_UnitNodeTable = (UnitArray*) 0x0059CB40;
  const  u32           UNIT_ARRAY_MAX_LENGTH = 1701;
  

  static u32           BWFXN_CommandUnit = 0x4BFF80;
  static u32           BWFXN_IssueCommand = 0x4858F0;
  static u32           BWFXN_HUD = 0x4202A0; 
  static u32*          BWFXN_InGame = (u32*) 0x6556C8;
  static u32           BWFXN_CountDownTimer = 0x0058D6DC; /**< @todo verify */
  static u32           BWXFN_PrintText = 0x48CE60;
  static u32           BWXFN_PrintPublicText = 0x4F2EC0; // Doesn't work now
  //static int           BWXFN_Refresh = 0x41DE10; // Is some function, not tested yet (4 int arguments)
  static Unit*         BWXFN_CurrentPlayerSelectionGroup = (Unit*) 0x005971F0;
  static u32           BWXFN_GameStart = 0x004C96A1;
  static u32           BWXFN_GameStartBack = BW::BWXFN_GameStart + 5;
  static u32           BWXFN_GameStartTarget = 0x00417C10;
  static u32           BWXFN_GameEnd = 0x004EE5C3;
  static u32           BWXFN_GameEndBack = BWXFN_GameEnd + 5;
  static u32           BWXFN_GameEndTarget = 0x00416D80;
  static u32           BWXFN_NextFrameHelperFunction = 0x004D94ED;
  static u32           BWXFN_NextFrameHelperFunctionBack = 0x004D94ED + 5;
  static u32           BWXFN_NextFrameHelperFunctionTarget = 0x4D1110;
  static u32           BWXFN_Refresh = 0x0041DE10; /**< or 0x0041E040 ? */
  static u32           BWXFN_CancelTrainByClickInTheQueue = 0x004573D9;
  static u32           BWXFN_CancelTrainByClickInTheQueueBack = 0x004573D9 + 5;
  static u32           BWXFN_CancelTrainByClickInTheQueueTarget = BWFXN_IssueCommand;
  static u32           BWXFN_CancelTrainByEscape = 0x00423494;
  static u32           BWXFN_CancelTrainByEscapeBack = 0x00423494 + 5;
  static u32           BWXFN_CancelTrainByEscapeTarget = BWFXN_IssueCommand;
  static u32           BWXFN_RemoveUnit = 0x004EC134;
  static u32           BWXFN_RemoveUnitBack = BWXFN_RemoveUnit + 5;
  static u32           BWXFN_RemoveUnitTarget = 0x00479880;
  static u32           BWXFN_Redraw = 0x004D9563;
  static u32           BWXFN_RedrawBack = 0x004D9563 + 5;
  static u32           BWXFN_RedrawTarget = 0x0041C9F0;
  static u32           BWXFN_NextLogicFrame = 0x004D937E;
  static u32           BWXFN_NextLogicFrameBack = BWXFN_NextLogicFrame + 5;
  static u32           BWXFN_NextLogicFrameTarget = 0x00488420;
  static int*          BWXFN_MouseX = (int*) 0x006CDDAC;
  static int*          BWXFN_MouseY = (int*) 0x006CDDB0;
  static int*          BWXFN_ScreenX = (int*) 0x00628430;
  static int*          BWXFN_ScreenY = (int*) 0x00628458;
  static char*         BWXFN_CurrentMapFileName = (char*) 0x0057FD24;
  static char**        BWXFN_Sting_table = (char**)0x6d1220;
                                                            


  //------------------------------------ SUPPLIES -----------------------------
  // -------- AVAILABLE PROTOSS
  /** Direct mapping of players Protoss available supplies in the bw memory. */
  struct PlayerSupliesAvailableProtoss_type
  {
    /** Direct mapping of player Protoss available supplies in the bw memory. */
    struct PlayerSuppliesAvaialableProtoss
    {
      s32 suppliesAvailableProtoss;
    };
    PlayerSuppliesAvaialableProtoss player[8];
  };
  static PlayerSupliesAvailableProtoss_type* BWXFN_SuppliesAvaialbeProtoss = (PlayerSupliesAvailableProtoss_type*)  	0x0058224C;
  // ----- USED PROTOSS
  /** Direct mapping of players Protoss used supplies in the bw memory. */
  struct PlayerSupliesUsedProtoss_type
  {
    /** Direct mapping of player Protoss used supplies in the bw memory. */
    struct PlayerSuppliesUsedProtoss
    {
      s32 suppliesUsedProtoss;
    };
    PlayerSuppliesUsedProtoss player[8];
  };
  static PlayerSupliesUsedProtoss_type* BWXFN_SuppliesUsedProtoss = (PlayerSupliesUsedProtoss_type*) 0x0058227C;
  // -------- AVAILABLE TERRAN
  /** Direct mapping of players Terran available supplies in the bw memory. */
  struct PlayerSupliesAvailableTerran_type
  {
   /** Direct mapping of player Terran available supplies in the bw memory. */
    struct PlayerSuppliesAvaialableTerran
    {
      s32 suppliesAvailableTerran;
    };
    PlayerSuppliesAvaialableTerran player[8];
  };
  static PlayerSupliesAvailableTerran_type* BWXFN_SuppliesAvaialbeTerran = (PlayerSupliesAvailableTerran_type*) 0x005821BC;
  // ----- USED TERRAN
  /** Direct mapping of players Terran used supplies in the bw memory. */
  struct PlayerSupliesUsedTerran_type
  {
   /** Direct mapping of player Terran used supplies in the bw memory. */
    struct PlayerSuppliesUsedTerran
    {
      s32 suppliesUsedTerran;
    };
    PlayerSuppliesUsedTerran player[8];
  };
  static PlayerSupliesUsedTerran_type* BWXFN_SuppliesUsedTerran = (PlayerSupliesUsedTerran_type*) 0x005821EC;
  // -------- AVAILABLE ZERG
  /** Direct mapping of players Zerg available supplies in the bw memory. */
  struct PlayerSupliesAvailableZerg_type
  {
    /** Direct mapping of players Zerg available supplies in the bw memory. */
    struct PlayerSuppliesAvaialableZerg
    {
      s32 suppliesAvailableZerg;
    };
    PlayerSuppliesAvaialableZerg player[8];
  };
  static PlayerSupliesAvailableZerg_type* BWXFN_SuppliesAvaialbeZerg = (PlayerSupliesAvailableZerg_type*) 0x0058212C;
  //----------------------------------- USED ZERG -----------------------------
  /** Direct mapping of players Zerg used supplies in the bw memory. */
  struct PlayerSupliesUsedZerg_type
  {
    /** Direct mapping of players Zerg used supplies in the bw memory. */
    struct PlayerSuppliesUsedZerg
    {
      s32 suppliesUsedZerg;
    };
    PlayerSuppliesUsedZerg player[8];
  };
  static PlayerSupliesUsedZerg_type* BWXFN_SuppliesUsedZerg = (PlayerSupliesUsedZerg_type*) 0x0058215C;
 
  const int unitTypeCount = 228;
  // -------------------------------- UNIT MINERAL PRICES ---------------------
  /** Direct mapping of unit types mineral prices. */
  struct MineralPrices_type
  {
    u16 mineralPrice[unitTypeCount];
  };
  static MineralPrices_type* BWXFN_MineralPrices = (MineralPrices_type*) 0x00663870;
  // -------------------------------- UNIT MINERAL PRICES ---------------------
  /** Direct mapping of unit types gas prices. */
  struct GasPrices_type
  {
    u16 gasPrice[unitTypeCount];
  };
  static GasPrices_type* BWXFN_GasPrices = (GasPrices_type*) 0x0065FCE8;
  //--------------------------------- UNIT SUPPLY DEMANDS ---------------------
  /** Direct mapping of unit supply demands. */
  struct SupplyDemands_type
  {
    u8 supplyDemand[unitTypeCount];
  };
  static SupplyDemands_type* BWXFN_SupplyDemands = (SupplyDemands_type*) 0x00663CD0;
  //---------------------------------- UNIT MAX HP ----------------------------
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
    MaxHealthPoints_NotAttackable_Repairable_Internal_type raw[unitTypeCount];
  };
  static MaxHealthPoints_NotAttackable_Repairable_type* BWXFN_MaxHealthPoints_NotAttackable_Repairable = (MaxHealthPoints_NotAttackable_Repairable_type*) 0x00662339;
  //--------------------------------- UNIT MAX SHIELDS ------------------------
  /** Direct mapping of unit unit type Shield points */
  struct MaxShieldPoints_type
  {
    u16 maxShieldPoints[unitTypeCount];
  };
  static MaxShieldPoints_type* BWXFN_MaxShieldPoints = (MaxShieldPoints_type*) 0x00660DE8;
   //--------------------------------- UNIT MAX SHIELDS ------------------------
  /** Direct mapping of unit type armor */
  struct Armor_type
  {
    u8 armor[unitTypeCount];
  };
  static Armor_type* BWXFN_Armor = (Armor_type*) 0x0065FEB0;

    //--------------------------------- UNIT MAX BUILD TIME ----------------------
  /** Direct mapping of unit build time*/
  struct BuildTime_type
  {
   u16 buildTime[unitTypeCount];
  };
  static BuildTime_type* BWXFN_BuildTime = (BuildTime_type*) 0x00660410;

  //--------------------------------- UNIT DIRECTIONS ------------------------
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
    UnitDimensions units[unitTypeCount];
  };
  static UnitsDimensions_type* BWXFN_UnitDimensions = (UnitsDimensions_type*) 0x006617B0;
  const u8 NoWeapon = 130;
  //--------------------------------- GROUND WEAPONS  ------------------------
  /** Direct mapping of unit unit type armor */
  struct UnitsGroundWeapon_type
  {
    u8 unit[unitTypeCount];
  };
  static UnitsGroundWeapon_type* BWXFN_UnitGroundWeapon = (UnitsGroundWeapon_type*) 0x006636A0;

  const int weaponTypeCount = 130;
  //-------------------------------- WEAPON DAMAGE FACTOR --------------------
  /** Direct mapping of unit unit type armor */
  struct WeaponsDamageFactor_type
  {
    u8 weapon[weaponTypeCount];
  };
  static WeaponsDamageFactor_type* BWXFN_WeaponDamageFactor = (WeaponsDamageFactor_type*) 0x006564C8;

  //-------------------------------- WEAPON DAMAGE TYPE ----------------------
  /** Direct mapping of unit unit type armor */
  struct WeaponsDamage_type
  {
    u16 weapon[weaponTypeCount];
  };
  static WeaponsDamage_type* BWXFN_WeaponDamage = (WeaponsDamage_type*) 0x00656E98;

  //--------------------------------PROTOTYPE FLAGS --------------------------
  /** Direct mapping of unit flags data */
  struct PrototypeFlags_type
  {
    Util::BitMask<UnitPrototypeFlags::Enum> unit[unitTypeCount];
  };
  static PrototypeFlags_type* BWXFN_UnitPrototypeFlags = (PrototypeFlags_type*) 0x00664068;

  //------------------------------PROTOTYPE GROUP FLAGS ----------------------
  /** Direct mapping of unit flags data */
  struct PrototypeGroupFlags_type
  {
    Util::BitMask<GroupFlags::Enum> unit[unitTypeCount];
  };

  static PrototypeGroupFlags_type* BWXFN_PrototypeGroupFlags = (PrototypeGroupFlags_type*) 0x663788;

  //---------------------------------MINITILE MAPPING ------------------------
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

  #define UNKNOWN_BIT_SIZE 16
  #define UNKNOWN_TYPE u16
  
  #define UNKNOWN_BIT_SIZE 16
  #define UNKNOWN_TYPE u16
  //----------------------------------- UNKNOWN TYPE  ------------------------
  /** Direct mapping of unit unit type armor */
  struct Unknown_type
  {
   UNKNOWN_TYPE unknown[unitTypeCount];
  };
  static Unknown_type* BWXFN_Unknown = (Unknown_type*) 0x065FF8D;
  //---------------------------------------------------------------------------
  static const int TileSize = 32;
};
#pragma pack()

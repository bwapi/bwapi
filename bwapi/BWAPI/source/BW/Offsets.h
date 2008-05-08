#pragma once
#include <windows.h>
#include "../Types.h"
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
  struct UnitData;

  //--------------------------------- MINERALS TYPE -----------------------------
  /** Direct mapping of players mineral amount in the bw memory */
  struct PlayerMinerals_type
  {
    s32 player[12];
  };
  static PlayerMinerals_type* PlayerMinerals = (PlayerMinerals_type*)0x0057F0D8;

  //------------------------------------ GAS TYPE -------------------------------
  /** Direct mapping of players gas amount in the bw memory */
  struct PlayerGas_type
  {
    s32 player[12];
  };
  static PlayerGas_type* PlayerGas = (PlayerGas_type*)0x0057F108;

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
  static PlayerNames_type* PlayerNames = (PlayerNames_type*)0x0057EEEB;


  static Unit*         UnitNodeTable_LastElement = (Unit*)0x00628280;
  static Unit*         UnitNodeTable_FirstElement = (Unit*)0x0059CB40;
  static UnitArray*    UnitNodeTable = (UnitArray*)UnitNodeTable_FirstElement;

  static u32           BWFXN_CommandUnit = 0x4BFF80;
  static u32           BWFXN_IssueCommand = 0x4858F0;
  static int           BWFXN_HUD = 0x4202A0; 
  static int           BWFXN_InGame = 0x654D64; // Wrong offset
  static int           BWXFN_PrintText = 0x48CE60;
  static int           BWXFN_PrintPublicText = 0x4F2EC0; // Doesn't work now
  static int           BWXFN_Refresh = 0x41DE10; // Is some function, not tested yet (4 int arguments)
  static UnitData*     BWXFN_CurrentPlayerSelectionGroup = (UnitData*) 0x005971F0;
  static int           BWXFN_GameStart = 0x004C96A1;
  static int           BWXFN_GameStartBack = BW::BWXFN_GameStart + 5;
  static int           BWXFN_GameStartTarget = 0x00417C10;
  static int           BWXFN_GameEnd = 0x004EE5C3;
  static int           BWXFN_GameEndBack = BWXFN_GameEnd + 5;
  static int           BWXFN_GameEndTarget = 0x00416D80;
  static int           BWXFN_NextFrameHelperFunction = 0x004D94ED;
  static int           BWXFN_NextFrameHelperFunctionBack = 0x004D94ED + 5;
  static int           BWXFN_NextFrameHelperFunctionTarget = 0x4D1110;
  //------------------------------------ SUPPLIES -----------------------------
  // -------- AVAILABLE PROTOSS
  /** Direct mapping of players Protoss avialable supplies in the bw memory. */
  struct PlayerSupliesAvailableProtoss_type
  {
    /** Direct mapping of player Protoss avialable supplies in the bw memory. */
    struct PlayerSuppliesAvaialableProtoss
    {
      s32 suppliesAvailableProtoss;
    };
    PlayerSuppliesAvaialableProtoss player[8];
  };
  static PlayerSupliesAvailableProtoss_type* SuppliesAvaialbeProtoss = (PlayerSupliesAvailableProtoss_type*)  	0x0058224C;
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
  static PlayerSupliesUsedProtoss_type* SuppliesUsedProtoss = (PlayerSupliesUsedProtoss_type*) 0x0058227C;
  // -------- AVAILABLE TERRAN
  /** Direct mapping of players Terran avialable supplies in the bw memory. */
  struct PlayerSupliesAvailableTerran_type
  {
   /** Direct mapping of player Terran avialable supplies in the bw memory. */
    struct PlayerSuppliesAvaialableTerran
    {
      s32 suppliesAvailableTerran;
    };
    PlayerSuppliesAvaialableTerran player[8];
  };
  static PlayerSupliesAvailableTerran_type* SuppliesAvaialbeTerran = (PlayerSupliesAvailableTerran_type*) 0x005821BC;
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
  static PlayerSupliesUsedTerran_type* SuppliesUsedTerran = (PlayerSupliesUsedTerran_type*) 0x005821EC;
  // -------- AVAILABLE ZERG
  /** Direct mapping of players Zerg avialable supplies in the bw memory. */
  struct PlayerSupliesAvailableZerg_type
  {
    /** Direct mapping of players Zerg avialable supplies in the bw memory. */
    struct PlayerSuppliesAvaialableZerg
    {
      s32 suppliesAvailableZerg;
    };
    PlayerSuppliesAvaialableZerg player[8];
  };
  static PlayerSupliesAvailableZerg_type* SuppliesAvaialbeZerg = (PlayerSupliesAvailableZerg_type*) 0x0058212C;
  // ----- USED ZERG
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
  static PlayerSupliesUsedZerg_type* SuppliesUsedZerg = (PlayerSupliesUsedZerg_type*) 0x0058215C;
 
  const int unitTypeCount = 228;
  // -------------------------------- UNIT MINERAL PRICES --------------------
  /** Direct mapping of unit types mineral prices. */
  struct MineralPrices_type
  {
    u16 mineralPrice[unitTypeCount];
  };
  static MineralPrices_type* MineralPrices = (MineralPrices_type*) 0x00663870;
  // -------------------------------- UNIT SUPPLY DEMANDS --------------------
  /** Direct mapping of unit supply demands. */
  struct SupplyDemands_type
  {
    u8 supplyDemand[unitTypeCount];
  };
  static SupplyDemands_type* SupplyDemands = (SupplyDemands_type*) 0x00663CD0;
  //---------------------------------------------------------------------------
};

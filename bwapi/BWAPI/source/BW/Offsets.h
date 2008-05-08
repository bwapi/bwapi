#pragma once
#include <windows.h>
#include "../Types.h"

namespace BW
{
  struct Unit;
  struct UnitArray;
  struct UnitData;

  //--------------------------------- MINERALS TYPE -----------------------------
  struct PlayerMinerals_type
  {
    s32 player[12];
  };
  static PlayerMinerals_type* PlayerMinerals = (PlayerMinerals_type*)0x0057F0D8;

  //------------------------------------ GAS TYPE -------------------------------
  struct PlayerGas_type
  {
    s32 player[12];
  };
  static PlayerGas_type* PlayerGas = (PlayerGas_type*)0x0057F108;

  //----------------------------------- NAMES TYPE ------------------------------
  struct PlayerNames_type
  {
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
  //------------------------------------ SUPPLIES -----------------------------
   // -------- AVAILABLE PROTOSS
  struct PlayerSupliesAvailableProtoss_type
  {
    struct PlayerSuppliesAvaialableProtoss
    {
      s32 suppliesAvailableProtoss;
    };
    PlayerSuppliesAvaialableProtoss player[8];
  };
  static PlayerSupliesAvailableProtoss_type* SuppliesAvaialbeProtoss = (PlayerSupliesAvailableProtoss_type*)  	0x0058224C;
  // ----- USED PROTOSS
  struct PlayerSupliesUsedProtoss_type
  {
    struct PlayerSuppliesUsedProtoss
    {
      s32 suppliesUsedProtoss;
    };
    PlayerSuppliesUsedProtoss player[8];
  };
  static PlayerSupliesUsedProtoss_type* SuppliesUsedProtoss = (PlayerSupliesUsedProtoss_type*) 0x0058227C;
  // -------- AVAILABLE TERRAN
  struct PlayerSupliesAvailableTerran_type
  {
    struct PlayerSuppliesAvaialableTerran
    {
      s32 suppliesAvailableTerran;
    };
    PlayerSuppliesAvaialableTerran player[8];
  };
  static PlayerSupliesAvailableTerran_type* SuppliesAvaialbeTerran = (PlayerSupliesAvailableTerran_type*) 0x005821BC;
  // ----- USED TERRAN
  struct PlayerSupliesUsedTerran_type
  {
    struct PlayerSuppliesUsedTerran
    {
      s32 suppliesUsedTerran;
    };
    PlayerSuppliesUsedTerran player[8];
  };
  static PlayerSupliesUsedTerran_type* SuppliesUsedTerran = (PlayerSupliesUsedTerran_type*) 0x005821EC;
  // -------- AVAILABLE ZERG
  struct PlayerSupliesAvailableZerg_type
  {
    struct PlayerSuppliesAvaialableZerg
    {
      s32 suppliesAvailableZerg;
    };
    PlayerSuppliesAvaialableZerg player[8];
  };
  static PlayerSupliesAvailableZerg_type* SuppliesAvaialbeZerg = (PlayerSupliesAvailableZerg_type*) 0x0058212C;
  // ----- USED ZERG
  struct PlayerSupliesUsedZerg_type
  {
    struct PlayerSuppliesUsedZerg
    {
      s32 suppliesUsedZerg;
    };
    PlayerSuppliesUsedZerg player[8];
  };
  static PlayerSupliesUsedZerg_type* SuppliesUsedZerg = (PlayerSupliesUsedZerg_type*) 0x0058215C;
  //---------------------------------------------------------------------------
};

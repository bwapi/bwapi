#pragma once

#include "../Types.h"

namespace BW
{
  struct Unit;
  struct UnitArray;

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


  static const Unit* UnitNodeTable_LastElement = (Unit*)0x00628280;
  static const Unit* UnitNodeTable_FirstElement = (Unit*)0x0059CB40;
  static const UnitArray* UnitNodeTable = (UnitArray*)UnitNodeTable_FirstElement;

  static const u32 BWFXN_CommandUnit = 0x4BFF80;
  static const u32 IssueCommand = 0x4858F0;
  static const int BWFXN_HUD = 0x4202A0; 
};

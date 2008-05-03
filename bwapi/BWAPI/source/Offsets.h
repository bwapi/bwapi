#pragma once

#include "Types.h"
#include "BW_Unit.h"

namespace BW
{

};

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
   BW_PlayerName player[8];
};
static PlayerNames_type* PlayerNames = (PlayerNames_type*)0x0057EEEB;


static Unit* UnitNodeTable_LastElement = (Unit*)0x00628280;
static Unit* UnitNodeTable_FirstElement = (Unit*)0x0059CB40;
static UnitArray* UnitNodeTable = (UnitArray*)UnitNodeTable_FirstElement;

const int BWFXN_CommandUnit = 0x4BFF80;
const int IssueCommand = 0x4858F0;


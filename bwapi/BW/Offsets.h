#pragma once

#include "Types.h"
#include "BW_Unit.h"

//--------------------------------- MINERALS TYPE -----------------------------
struct BW_PlayerMinerals_type
{
   s32 player[12];
};
static BW_PlayerMinerals_type* BW_PlayerMinerals = (BW_PlayerMinerals_type*)0x0057F0D8;

//------------------------------------ GAS TYPE -------------------------------
struct BW_PlayerGas_type
{
   s32 player[12];
};
static BW_PlayerGas_type* BW_PlayerGas = (BW_PlayerGas_type*)0x0057F108;

//----------------------------------- NAMES TYPE ------------------------------
struct BW_PlayerNames_type
{
   struct BW_PlayerName
   {
      char name[36];
   };
   BW_PlayerName player[8];
};
static BW_PlayerNames_type* BW_PlayerNames = (BW_PlayerNames_type*)0x0057EEEB;

static BW_UnitArray* UNIT_NODE_TABLE = (BW_UnitArray*)0x0059CB40;

const int BWFXN_CommandUnit = 0x4BFF80;
const int IssueCommand = 0x4858F0;


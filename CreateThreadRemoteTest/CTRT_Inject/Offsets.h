#pragma once

#include "Types.h"

//////////////////////////////////////////////////////////////////////////
struct BW_PlayerMinerals_type
{
   s32 Player[12];
};
BW_PlayerMinerals_type* BW_PlayerMinerals = (BW_PlayerMinerals_type*)0x0057F0D8;

//////////////////////////////////////////////////////////////////////////
struct BW_PlayerGas_type
{
   s32 Player[12];
};
BW_PlayerGas_type* BW_PlayerGas = (BW_PlayerGas_type*)0x0057F108;

//////////////////////////////////////////////////////////////////////////
struct BW_PlayerNames_type
{
   struct BW_PlayerName
   {
      char Name[36];
   };

   BW_PlayerName Player[8];
};
BW_PlayerNames_type* BW_PlayerNames = (BW_PlayerNames_type*)0x0057EEEB;

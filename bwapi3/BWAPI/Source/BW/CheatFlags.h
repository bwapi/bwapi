#pragma once
#include <Util/Types.h>

namespace BW
{
  // Enumeration of cheat codes
  namespace CheatFlags
  {
    enum Enum
    {
      BlackSheepWall         = 1 <<  0,
      OperationCwal          = 1 <<  1,
      PowerOverwelming       = 1 <<  2,
      SomethingForNothing    = 1 <<  3,
      ShowMeTheMoney         = 1 <<  4,
      GameOverMan            = 1 <<  6,
      ThereIsNoCowLevel      = 1 <<  7,
      StayingAlive           = 1 <<  8,
      Ophelia                = 1 <<  9,
      TheGathering           = 1 << 11,
      MedievalMan            = 1 << 12,
      ModifyThePhaseVariance = 1 << 13,
      WarAintWhatItUsedToBe  = 1 << 14,
      FoodForThought         = 1 << 17,
      WhatsMineIsMine        = 1 << 18,
      BreateDeep             = 1 << 19,
      NoGlues                = 1 << 29,
      None                   = 1 << 31
    };
  }
  typedef CheatFlags::Enum CheatFlag;
};
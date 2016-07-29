#pragma once
namespace BW
{
  /**
   * Enumeration of cheat codes
   */
  namespace CheatFlags
  {
    enum Enum
    {
      None                   = 0,
      BlackSheepWall         = 1 <<  0, // 0x1
      OperationCwal          = 1 <<  1, // 0x2
      PowerOverwelming       = 1 <<  2, // 0x4
      SomethingForNothing    = 1 <<  3, // 0x8
      ShowMeTheMoney         = 1 <<  4, // 0x10
      GameOverMan            = 1 <<  6, // 0x40
      ThereIsNoCowLevel      = 1 <<  7, // 0x80
      StayingAlive           = 1 <<  8, // 0x100
      Ophelia                = 1 <<  9, // 0x200
      TheGathering           = 1 << 11, // 0x800
      MedievalMan            = 1 << 12, // 0x1000
      ModifyThePhaseVariance = 1 << 13, // 0x2000
      WarAintWhatItUsedToBe  = 1 << 14, // 0x4000
      FoodForThought         = 1 << 17, // 0x20000
      WhatsMineIsMine        = 1 << 18, // 0x40000
      BreatheDeep            = 1 << 19, // 0x80000
      NoGlues                = 1 << 29  // 0x20000000
    };
  }
};
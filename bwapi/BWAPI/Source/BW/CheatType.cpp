#pragma once
#include "CheatType.h"
#include <string>
namespace BW
{
  CheatFlags::Enum getCheatFlag(const char* name)
  {
    if ( strcmpi(name, "black sheep wall") == 0 )
      return CheatFlags::BlackSheepWall;
    else if ( strcmpi(name, "breathe deep") == 0 )
      return CheatFlags::BreatheDeep;
    else if ( strcmpi(name, "food for thought") == 0 )
      return CheatFlags::FoodForThought;
    else if ( strcmpi(name, "game over man") == 0 )
      return CheatFlags::GameOverMan;
    else if ( strcmpi(name, "medieval man") == 0 )
      return CheatFlags::MedievalMan;
    else if ( strcmpi(name, "modify the phase variance") == 0 )
      return CheatFlags::ModifyThePhaseVariance;
    else if ( strcmpi(name, "noglues") == 0 )
      return CheatFlags::NoGlues;
    else if ( strcmpi(name, "operation cwal") == 0 )
      return CheatFlags::OperationCwal;
    else if ( strcmpi(name, "ophelia") == 0 )
      return CheatFlags::Ophelia;
    else if ( strcmpi(name, "power overwhelming") == 0 )
      return CheatFlags::PowerOverwelming;
    else if ( strcmpi(name, "show me the money") == 0 )
      return CheatFlags::ShowMeTheMoney;
    else if ( strcmpi(name, "something for nothing") == 0 )
      return CheatFlags::SomethingForNothing;
    else if ( strcmpi(name, "staying alive") == 0 )
      return CheatFlags::StayingAlive;
    else if ( strcmpi(name, "the gathering") == 0 )
      return CheatFlags::TheGathering;
    else if ( strcmpi(name, "there is no cow level") == 0 )
      return CheatFlags::ThereIsNoCowLevel;
    else if ( strcmpi(name, "war aint what it used to be") == 0 )
      return CheatFlags::WarAintWhatItUsedToBe;
    else if ( strcmpi(name, "whats mine is mine") == 0 )
      return CheatFlags::WhatsMineIsMine;
    return CheatFlags::None;
  }
}

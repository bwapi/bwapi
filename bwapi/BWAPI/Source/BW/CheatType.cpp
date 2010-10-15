#pragma once
#include "CheatType.h"
#include <string>
namespace BW
{
  CheatFlags::Enum getCheatFlag(const char* name)
  {
    std::string name2 = std::string(name);
    if (name2 == std::string("black sheep wall"))
      return CheatFlags::BlackSheepWall;
    if (name2 == std::string("operation cwal"))
      return CheatFlags::OperationCwal;
    if (name2 == std::string("power overwhelming"))
      return CheatFlags::PowerOverwelming;
    if (name2 == std::string("something for nothing"))
      return CheatFlags::SomethingForNothing;
    if (name2 == std::string("show me the money"))
      return CheatFlags::ShowMeTheMoney;
    if (name2 == std::string("game over man"))
      return CheatFlags::GameOverMan;
    if (name2 == std::string("there is no cow level"))
      return CheatFlags::ThereIsNoCowLevel;
    if (name2 == std::string("staying alive"))
      return CheatFlags::StayingAlive;
    if (name2 == std::string("ophelia"))
      return CheatFlags::Ophelia;
    if (name2 == std::string("the gathering"))
      return CheatFlags::TheGathering;
    if (name2 == std::string("medieval man"))
      return CheatFlags::MedievalMan;
    if (name2 == std::string("modify the phase variance"))
      return CheatFlags::ModifyThePhaseVariance;
    if (name2 == std::string("war aint what it used to be"))
      return CheatFlags::WarAintWhatItUsedToBe;
    if (name2 == std::string("food for thought"))
      return CheatFlags::FoodForThought;
    if (name2 == std::string("whats mine is mine"))
      return CheatFlags::WhatsMineIsMine;
    if (name2 == std::string("breathe deep"))
      return CheatFlags::BreatheDeep;
    if (name2 == std::string("noglues"))
      return CheatFlags::NoGlues;
    return CheatFlags::None;
  }
}

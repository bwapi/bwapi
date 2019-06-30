#include "CheatType.h"
#include <string>
#include "../StringUtil.h"

namespace BW
{
  CheatFlags::Enum getCheatFlag(const std::string &str)
  {
    // Convert to lowercase in a new string
    std::string name = Util::to_lower_copy(str);

    // Get the cheat flag
    if      ( name == "black sheep wall" )            return CheatFlags::BlackSheepWall;
    else if ( name == "breathe deep" )                return CheatFlags::BreatheDeep;
    else if ( name == "food for thought" )            return CheatFlags::FoodForThought;
    else if ( name == "game over man" )               return CheatFlags::GameOverMan;
    else if ( name == "medieval man" )                return CheatFlags::MedievalMan;
    else if ( name == "modify the phase variance" )   return CheatFlags::ModifyThePhaseVariance;
    else if ( name == "noglues" )                     return CheatFlags::NoGlues;
    else if ( name == "operation cwal" )              return CheatFlags::OperationCwal;
    else if ( name == "ophelia" )                     return CheatFlags::Ophelia;
    else if ( name == "power overwhelming" )          return CheatFlags::PowerOverwelming;
    else if ( name == "show me the money" )           return CheatFlags::ShowMeTheMoney;
    else if ( name == "something for nothing" )       return CheatFlags::SomethingForNothing;
    else if ( name == "staying alive" )               return CheatFlags::StayingAlive;
    else if ( name == "the gathering" )               return CheatFlags::TheGathering;
    else if ( name == "there is no cow level" )       return CheatFlags::ThereIsNoCowLevel;
    else if ( name == "war aint what it used to be" ) return CheatFlags::WarAintWhatItUsedToBe;
    else if ( name == "whats mine is mine" )          return CheatFlags::WhatsMineIsMine;
    else return CheatFlags::None;
  }
}

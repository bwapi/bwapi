#pragma once
namespace BWAPI
{
  namespace EventType
  {
    enum Enum
    {
      Disconnect,
      MatchStart,
      MatchEnd,
      MatchFrame,
      MenuFrame,
      SendText,
      PlayerLeft,
      NukeDetect,
      UnitCreate,
      UnitDestroy,
      UnitMorph,
      UnitShow,
      UnitHide,
      UnitRenegade,
      SaveGame
    };
  }
  static const int EVENT_TYPE_COUNT  =  13;
}

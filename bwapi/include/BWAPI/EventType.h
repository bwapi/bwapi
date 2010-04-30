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
      Frame,
      SendText,
      PlayerLeft,
      NukeDetect,
      UnitCreate,
      UnitDestroy,
      UnitDiscover,
      UnitMorph,
      UnitShow,
      UnitHide,
      UnitRenegade,
      SaveGame
    };
  }
  static const int EVENT_TYPE_COUNT  =  13;
}

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
      ReceiveText,
      PlayerLeft,
      NukeDetect,
      UnitCreate,
      UnitDestroy,
      UnitMorph,
      UnitShow,
      UnitHide,
      UnitRenegade,
      SaveGame,
      None
    };
  }
}

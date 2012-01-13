#pragma once
namespace BWAPI
{
  namespace EventType
  {
    enum Enum
    {
      MatchStart,
      MatchEnd,
      MatchFrame,
      MenuFrame,
      SendText,
      ReceiveText,
      PlayerLeft,
      NukeDetect,
      UnitDiscover,
      UnitEvade,
      UnitShow,
      UnitHide,
      UnitCreate,
      UnitDestroy,
      UnitMorph,
      UnitRenegade,
      SaveGame,
      UnitComplete,
      //TriggerAction,
      PlayerDropped,
      None
    };
  }
}

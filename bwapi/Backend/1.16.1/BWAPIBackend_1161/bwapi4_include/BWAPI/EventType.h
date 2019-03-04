#pragma once
namespace BWAPI
{
  /// <summary>Contains a list of event types supported by BWAPI.</summary>
  namespace EventType
  {
    /// <summary>Enumeration of callback event types.</summary>
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
      None
    };
  }
}

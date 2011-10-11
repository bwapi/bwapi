#pragma once
/**
 *  Used in UnitCommand
 */

namespace BWAPIC
{
  namespace CommandType
  {
    enum Enum
    {
      None,
      SetScreenPosition,
      PingMinimap,
      EnableFlag,
      Printf,
      SendText,
      ChangeRace,
      StartGame,
      PauseGame,
      ResumeGame,
      LeaveGame,
      RestartGame,
      SetLocalSpeed,
      SetLatCom,
      SetGui,
      SetFrameSkip,
      SetMap,
      SetAllies,
      SetVision,
      SetCommandOptimizerLevel,
      SetReplayVision,
      SetRevealAll
    };
  }
}
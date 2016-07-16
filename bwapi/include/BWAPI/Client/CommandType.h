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
      SetRevealAll
    };
  }
}
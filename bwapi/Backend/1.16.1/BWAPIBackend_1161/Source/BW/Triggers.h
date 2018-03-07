#pragma once

namespace BW
{
  /**
   * Enumeration of the Starcraft game triggers (or "rules")
   */
  namespace Actions
  {
    enum Enum
    {
      None,
      Victory,
      Defeat,
      PreserveTrigger,
      Wait,
      PauseGame,
      UnpauseGame,
      Transmission,
      PlayWAV,
      DisplayTextMessage,
      CenterView,
      CreateUnitwithProperties,
      SetMissionObjectives,
      SetSwitch,
      SetCountdownTimer,
      RunAIScript,
      RunAIScriptAtLocation,
      LeaderBoardControl,
      LeaderBoardControlAt,
      LeaderBoardResources,
      LeaderBoardKills,
      LeaderBoardPoints,
      KillUnit,
      KillUnitAtLocation,
      RemoveUnit,
      RemoveUnitAtLocation,
      SetResources,
      SetScore,
      MinimapPing,
      TalkingPortrait,
      MuteUnitSpeech,
      UnmuteUnitSpeech,
      LeaderboardComputerPlayers,
      LeaderboardGoalControl,
      LeaderboardGoalControlAt,
      LeaderboardGoalResources,
      LeaderboardGoalKills,
      LeaderboardGoalPoints,
      MoveLocation,
      MoveUnit,
      LeaderboardGreed,
      SetNextScenario,
      SetDoodadState,
      SetInvincibility,
      CreateUnit,
      SetDeaths,
      Order,
      Comment,
      GiveUnitstoPlayer,
      ModifyUnitHitPoints,
      ModifyUnitEnergy,
      ModifyUnitShieldPoints,
      ModifyUnitResourceAmount,
      ModifyUnitHangerCount,
      PauseTimer,
      UnpauseTimer,
      Draw,
      SetAllianceStatus,
      DisableDebugMode,
      EnableDebugMode
    };
  };
  namespace Conditions
  {
    enum Enum
    {
      None,
      CountdownTimer,
      Command,
      Bring,
      Accumulate,
      Kill,
      CommandTheMost,
      CommandsTheMostAt,
      MostKills,
      HighestScore,
      MostResources,
      Switch,
      ElapsedTime,
      MissionBriefing,
      Opponents,
      Deaths,
      CommandTheLeast,
      CommandTheLeastAt,
      LeastKills,
      LowestScore,
      LeastResources,
      Score,
      Always,
      Never
    };
  };

  namespace TriggerEntryFlags
  {
    enum Enum
    {
      WaitExecute       = 0x01,
      IgnoreExecution   = 0x02,
      AlwaysDisplay     = 0x04
    };
  };
  namespace TriggerFlags
  {
    enum Enum
    {
      ExecuteActions    = 0x01,
      IgnoreDefeat      = 0x02,
      PreserveTrigger   = 0x04,
      IgnoreExecution   = 0x08,
      SkipUIActions     = 0x10,
      PausedGame        = 0x20,
      DisableWaitSkip   = 0x40
    };
  };

  namespace PlayerGroups
  {
    enum Enum
    {
      Player1,
      Player2,
      Player3,
      Player4,
      Player5,
      Player6,
      Player7,
      Player8,
      Player9,
      Player10,
      Player11,
      Player12,
      None,
      CurrentPlayer,
      Foes,
      Allies,
      NeutralPlayers,
      AllPlayers,
      Force1,
      Force2,
      Force3,
      Force4,
      Unused1,
      Unused2,
      Unused3,
      Unused4,
      NonAlliedVictoryPlayers,
      Max
    };
  };
};

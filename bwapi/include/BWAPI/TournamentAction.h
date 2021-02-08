#pragma once

namespace BWAPI
{
  /// <summary>Contains a list of interruptable actions that the tournament module can allow or deny.</summary>
  /// @see TournamentModule::onAction
  namespace Tournament
  {
    /// <summary>Contains a list of interruptable actions that the tournament module can allow or deny.</summary>
    /// @see TournamentModule::onAction
    enum ActionID
    {
      /// @see Game::enableFlag
      EnableFlag,

      /// @see Game::pauseGame
      PauseGame,
      
      /// @see Game::resumeGame
      ResumeGame,

      /// @see Game::leaveGame
      LeaveGame,

      /// @see Game::setLocalSpeed
      SetLocalSpeed,

      /// @see Game::setTextSize
      SetTextSize,

      /// @see Game::setLatCom
      SetLatCom,

      /// @see Game::setGUI
      SetGUI,

      /// @see Game::setMap
      SetMap,

      /// @see Game::setFrameSkip
      SetFrameSkip,

      /// @see Game::printf
      Printf,

      /// @see Game::sendText
      SendText,

      /// @see Game::setCommandOptimizationLevel
      SetCommandOptimizationLevel
    };

  }
}

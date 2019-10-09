#pragma once
namespace BWAPI4
{
  /// <summary>Contains enumeration of known latency values.</summary>
  /// @see Game::getLatency
  namespace Latency
  {
    /// <summary>Contains enumeration of known latency values.</summary>
    /// @see Game::getLatency
    enum Enum
    {
      SinglePlayer    = 2,
      LanLow          = 5,
      LanMedium       = 7,
      LanHigh         = 9,
      BattlenetLow    = 14,
      BattlenetMedium = 19,
      BattlenetHigh   = 24
    };
  }
}

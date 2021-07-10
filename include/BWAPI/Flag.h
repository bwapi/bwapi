#pragma once
namespace BWAPI
{
  /// <summary>Contains flag enumerations for BWAPI.</summary>
  /// @see Game::isFlagEnabled
  namespace Flag
  {
    /// <summary>Contains flag enumerations for BWAPI.</summary>
    /// @see Game::isFlagEnabled
    enum Enum
    {
      /// <summary>Enable to get information about all units on the map, not just the visible units.</summary>
      CompleteMapInformation = 0,

      /// <summary>Enable to get information from the user (what units are selected, chat messages
      /// the user enters, etc)</summary>
      UserInput              = 1,

      /// <summary>The maximum number of different flags available.</summary>
      Max
    };
  }
}

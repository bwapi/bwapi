#pragma once
#include <iostream>

namespace BWAPI
{
  /// <summary>Tries to open a console for displaying text written to stdout and stderr, and read from stdin.</summary>
  /// 
  /// <param name="allowAttach">
  ///   If true, try to attach to the parents' console.
  /// </param>
  /// <param name="allowAlloc">
  ///   If true, and attaching is disabled or fails, allocate a new console (which should almost always succeed).
  /// </param>
  ///
  /// @returns
  ///   True if a console was successfully attached/allocated
  ///
  /// @see AttachConsole, AllocConsole
  /// @since 4.2.0
  bool openConsole(bool allowAttach, bool allowAlloc);

  /// <summary>Make openConsole be called when something is written to std::cout or std::cerr.</summary>
  /// @see openConsole
  /// @since 4.2.0
  void autoOpenConsole(bool allowAttach, bool allowAlloc);

  /// <summary>Output stream that writes text to the screen as a notification.</summary>
  /// Colored Text::Yellow by default, but preserves new Text::Enum colors written to it between lines.
  /// Wraps text when needed, not on word boundaries.
  /// 
  /// @note Text printed from this stream is not seen by other players or in replays.
  ///
  /// @see Text::Enum, BroodwarOutputDevice
  /// @since 4.2.0
  extern std::ostream& bwout;
  
  /// <summary>An output stream like bwout, but colored Text::BrightRed by default</summary>
  /// @see bwout
  /// @since 4.2.0
  extern std::ostream& bwerr;
  
  /// <summary>An output stream that writes to both std::cout and BWAPI::bwout.</summary>
  /// @since 4.2.0
  extern std::ostream& out;
  
  /// <summary>An output stream that writes to both std::cerr and BWAPI::bwerr.</summary>
  /// @since 4.2.0
  extern std::ostream& err;
}

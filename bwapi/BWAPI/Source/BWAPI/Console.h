#pragma once

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
}

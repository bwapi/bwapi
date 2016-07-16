#pragma once
#include <ostream>

namespace BWAPI
{
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

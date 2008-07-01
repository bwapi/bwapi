#pragma once

namespace BW
{
  /** Mapping of bit meaning for the MiniTile Flag property.*/
  namespace MiniTileFlags
  {
    enum Enum : u16
    {
      Walkable   = 1 << 0,
      Middle     = 1 << 1,
      High       = 1 << 2, /**< (Mid and High unchecked = Low) */
      BlocksView = 1 << 3,
      Ramp       = 1 << 4, /**< Appears on the middle minitiles of most ramps/stairs. */
    };
  }
}

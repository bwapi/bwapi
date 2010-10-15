#pragma once
namespace BWAPI
{
  namespace CoordinateType
  {
    enum Enum
    {
      Screen    = 1, /**< (0,0) corresponds to the top left corner of the screen. */
      Map       = 2, /**< (0,0) corresponds to the top left corner of the map. */
      Mouse     = 3, /**< (0,0) corresponds to the tip of the mouse . */
    };
  }
}

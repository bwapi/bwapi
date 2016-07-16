#pragma once
namespace BWAPI
{
  /// <summary>Contains the coordinate type enumeration for relative drawing positions.</summary>
  namespace CoordinateType
  {
    /// <summary>The coordinate type enumeration, used to indicate relative drawing positions.</summary>
    enum Enum
    {
      /// <summary>A default value for uninitialized coordinate types.</summary>
      None      = 0,

      /// <summary>Positions::Origin (0,0) corresponds to the top left corner of the <b>screen</b>.</summary>
      Screen    = 1,

      /// <summary>Positions::Origin (0,0) corresponds to the top left corner of the <b>map</b>.</summary>
      Map       = 2,

      /// <summary>Positions::Origin (0,0) corresponds to the location of the <b>mouse cursor</b>.</summary>
      Mouse     = 3,
    };
  }
}

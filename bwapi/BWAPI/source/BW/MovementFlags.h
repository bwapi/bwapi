#pragma once

namespace BW
{
  /** Definition of Flags specifying movement type - used in BW#Unit#movementFlags */
  namespace MovementFlags
  {
    enum Enum : u8
    {
      OrderedAtLeastOnce  = 1 << 0, /**< @todo investigate more.
                                     * It get's to 0 after the first move-involved command - sometimes get's to 1 again
                                     */
      Moving              = 1 << 1, /**< Terran building is lifting  */
      _alwaysZero1        = 1 << 2,
      _alwaysZero2        = 1 << 3,
      AlsoMoving          = 1 << 4, /**< (normal move, can't get there move, Location move, target move, mineral gather move)  */
      Lifted              = 1 << 5, /**< Verify */
      _alwaysZero3        = 1 << 6,
      _alwaysZero4        = 1 << 7

    };
  };
};


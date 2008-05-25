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
      Accelerating        = 1 << 1, /**< Unit is accelerating  */
      Braking             = 1 << 2, /**< Not when unit reaches destination, just when you hit stop, or building is finished */
      StartingAttack      = 1 << 3, /** Always 1 for a moment when the unit is starting new attack cycle*/
      Moving              = 1 << 4, /**< Still moving, but not accelerating */
      Lifted              = 1 << 5, /**< Lifted cc has this on (just before it moves) */
      unknown1            = 1 << 6, /**< The value is unknown, but it has something to do with chaning direction& accelerating */
      _alwaysZero1        = 1 << 7

    };
  };
};


#pragma once

namespace BW
{
  /** Definition of Flags specifying movement type - used in BW#Unit#movementFlags */
  namespace MovementFlags
  {
  enum Enum
    {
      OrderedAtLeastOnce  = 0x01, /**< @todo investigate more.
                                     * It get's to 0 after the first move-involved command - sometimes get's to 1 again
                                     */
      Accelerating        = 0x02, /**< Unit is accelerating  */
      Braking             = 0x04, /**< Not when unit reaches destination, just when you hit stop, or building is finished */
      StartingAttack      = 0x08, /** Always 1 for a moment when the unit is starting new attack cycle*/
      Moving              = 0x10, /**< Still moving, but not accelerating */
      Lifted              = 0x20, /**< Lifted cc has this on (just before it moves) */
      unknown1            = 0x40, /**< The value is unknown, but it has something to do with changing direction& accelerating */
      _alwaysZero1        = 0x80
    };
  };
};


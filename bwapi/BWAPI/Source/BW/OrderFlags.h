#pragma once

namespace BW
{
  /** Some additional info concering unit movement, mostly unknown */
  namespace OrderFlags
  {
  enum Enum
    {
      willWanderAgain   = 1 << 0, /**< Specifies if the unit will wander again after the current command is finished
                                   * Larva has this at 1 , but when you stop it (using the trick), it will become 0
                                   * Note, that mineral mined out (and invalid has this flag set (why? who knows)
                                   * Also Drone have this to 1 when it gathers- so it will wander when it stops and colide with other drones
                                   * Also Eggs have it
                                   * Also Zerg building under construction have it
                                   */
      autoWander        = 1 << 1, /**< Unit is auto wandering (larva, critter, stopped drone in gather, any unit with collistion )*/
      unknown1          = 1 << 2,
      unknown2          = 1 << 3
    };
  }
}
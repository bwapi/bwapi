#pragma once

namespace BW
{
  /** Bitmap specifying certain unit states */
  namespace StatusFlags
  {
  enum Enum
    {
      /*0x00000001*/Completed           = 1 <<  0,
      /*0x00000002*/OnGround            = 1 <<  1,
      /*0x00000004*/InAir               = 1 <<  2,
      /*0x00000008*/Disabled            = 1 <<  3,  /**< Protoss Unpowered */
      /*0x00000010*/Burrowed            = 1 <<  4,
      /*0x00000020*/InBuilding          = 1 <<  5,
      /*0x00000040*/InTransport         = 1 <<  6,
      /*0x00000080*/UNKNOWN1            = 1 <<  7,  /**< @todo Unknown */
      /*0x00000100*/RequiresDetection   = 1 <<  8,
      /*0x00000200*/Cloaked             = 1 <<  9,
      /*0x00000400*/DoodadStatesThing   = 1 << 10,  /**< @todo Unknown */
      /*0x00000800*/CloakingForFree     = 1 << 11,  /**< Requires no energy to cloak */
      /*0x00001000*/CanNotReceiveOrders = 1 << 12,
      /*0x00002000*/NoBrkCodeStart      = 1 << 13,  /**< Unbreakable code section in iscript */
      /*0x00004000*/UNKNOWN2            = 1 << 14,  /**< @todo Unknown */
      /*0x00008000*/CanNotAttack        = 1 << 15,  /**< @todo Unknown */
      /*0x00010000*/UNKNOWN3            = 1 << 16,  /**< @todo Unknown */
      /*0x00020000*/IsABuilding         = 1 << 17,
      /*0x00040000*/Turret              = 1 << 18,
      /*0x00080000*/
      /*0x00100000*/IsNormal            = 1 << 20,  /**< 1 for "normal" units, 0 for hallucinated units */
      /*0x00200000*/NoCollide           = 1 << 21,
      /*0x00400000*/
      /*0x00800000*/IsGathering         = 1 << 23,
      /*0x01000000*/
      /*0x02000000*/
      /*0x04000000*/Invincible          = 1 << 26,
      /*0x08000000*/
      /*0x10000000*/SpeedUpgrade        = 1 << 28,
      /*0x20000000*/CooldownUpgrade     = 1 << 29,
      /*0x40000000*/IsHallucination     = 1 << 30,  /**< 1 for hallucinated units, 0 for "normal" units */
      /*0x80000000*/
    };
  };
};


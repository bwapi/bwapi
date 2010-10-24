#pragma once

namespace BW
{
  /** Bitmap specifying certain unit states */
  namespace StatusFlags
  {
    enum Enum
    {
      Completed             = 0x00000001,
      GoundedBuilding       = 0x00000002, // a building that is on the ground
      InAir                 = 0x00000004,
      Disabled              = 0x00000008,  /**< Protoss Unpowered */
      Burrowed              = 0x00000010,
      InBuilding            = 0x00000020,
      InTransport           = 0x00000040,
      UNKNOWN1              = 0x00000080,  /**< @todo Unknown */
      RequiresDetection     = 0x00000100,
      Cloaked               = 0x00000200,
      DoodadStatesThing     = 0x00000400,  /**< @todo Unknown */
      CloakingForFree       = 0x00000800,  /**< Requires no energy to cloak */
      CanNotReceiveOrders   = 0x00001000,
      NoBrkCodeStart        = 0x00002000,  /**< Unbreakable code section in iscript */
      UNKNOWN2              = 0x00004000,  /**< @todo Unknown */
      CanNotAttack          = 0x00008000,  /**< @todo Unknown */
      IsAUnit               = 0x00010000,  /**< @todo Unknown */
      IsABuilding           = 0x00020000,
      IsgnoreTileCollision  = 0x00040000,
      UNKNOWN4              = 0x00080000,
      IsNormal              = 0x00100000,  /**< 1 for "normal" units, 0 for hallucinated units */
      NoCollide             = 0x00200000,
      UNKNOWN5              = 0x00400000,
      IsGathering           = 0x00800000,
      UNKNOWN6              = 0x01000000,
      UNKNOWN7              = 0x02000000,
      Invincible            = 0x04000000,
      UNKNOWN8              = 0x08000000,
      SpeedUpgrade          = 0x10000000,
      CooldownUpgrade       = 0x20000000,
      IsHallucination       = 0x40000000,  /**< 1 for hallucinated units, 0 for "normal" units */
      UNKNOWN9              = 0x80000000,
    };
  };
};


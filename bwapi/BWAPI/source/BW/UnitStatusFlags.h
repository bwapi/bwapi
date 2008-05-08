#pragma once

namespace BW
{
  /** */
  namespace StatusFlags
  {
    enum Enum : u32
    {
      Completed           = 1 << 0,
      OnGround            = 1 << 1,
      InAir               = 1 << 2,
      Disabled            = 1 << 3,   // Protoss Un-powered?
      Burrowed            = 1 << 4,
      EnteringBuilding    = 1 << 5,
      EnteringTransport   = 1 << 6,
      UNKNOWN1            = 1 << 7,   // @todo Unknown
      RequiresDetection   = 1 << 8,
      Cloaked             = 1 << 9,
      DoodadStatesThing   = 1 << 10,  // @todo Unknown
      CloakingForFree     = 1 << 11,  // Requires no energy to cloak
      CanNotReceiveOrders = 1 << 12,
      NoBrkCodeStartThing = 1 << 13,  // @todo Unknown
      UNKNOWN2            = 1 << 14,  // @todo Unknown
      CanNotAttack        = 1 << 15,
      UNKNOWN3            = 1 << 16,  // @todo Unknown
      IsABuilding         = 1 << 17,

      Invincible          = 1 << 26,

      SpeedUpgrade        = 1 << 28,
      CooldownUpgrade     = 1 << 29
    };
  };
};


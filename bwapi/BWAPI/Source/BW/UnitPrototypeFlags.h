#pragma once
namespace BW
{
  /** Flags specifying certain characteristics of unit prototype */
  namespace UnitPrototypeFlags
  {
  enum Enum
    {
      Building                = 1 << 0x00,
      Addon                   = 1 << 0x01,
      Flyer                   = 1 << 0x02,
      Worker                  = 1 << 0x03,
      Subunit                 = 1 << 0x04,
      FlyingBuilding          = 1 << 0x05,
      Hero                    = 1 << 0x06,
      RegeneratesHP           = 1 << 0x07,
      AnimatedIdle            = 1 << 0x08,
      Cloakable               = 1 << 0x09,
      TwoUnitsIn1Egg          = 1 << 0x0A,
      NeutralAccessories      = 1 << 0x0B,
      ResourceDepot           = 1 << 0x0C,
      ResourceContainer       = 1 << 0x0D,
      RoboticUnit             = 1 << 0x0E,
      Detector                = 1 << 0x0F,
      Organicunit             = 1 << 0x10,
      CreepBuilding           = 1 << 0x11,
      Unused                  = 1 << 0x12,
      RequiredPsi             = 1 << 0x13,
      Burrowable              = 1 << 0x14,
      Spellcaster             = 1 << 0x15,
      PermanentCloak          = 1 << 0x16,
      NPCOrAccessories        = 1 << 0x17,
      MorphFromOtherUnit      = 1 << 0x18,
      LargeUnit               = 1 << 0x19,
      HugeUnit                = 1 << 0x1A,
      AutoAttackAndMove       = 1 << 0x1B,
      Attack                  = 1 << 0x1C, /**< Can attack */
      Invincible              = 1 << 0x1D,
      Mechanical              = 1 << 0x1E,
      ProducesUnits           = (u32) 1 << 0x1F /**< It can produce units directly (making buildings doesn't count) */
    };
  }
}
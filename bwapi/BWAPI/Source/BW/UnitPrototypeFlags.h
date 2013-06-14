#pragma once
namespace BW
{
  /** Flags specifying certain characteristics of unit prototype */
  namespace UnitPrototypeFlags
  {
    enum Enum
    {
      Building                = 0x00000001,
      Addon                   = 0x00000002,
      Flyer                   = 0x00000004,
      Worker                  = 0x00000008,
      Subunit                 = 0x00000010,
      FlyingBuilding          = 0x00000020,
      Hero                    = 0x00000040,
      RegeneratesHP           = 0x00000080,
      AnimatedIdle            = 0x00000100,
      Cloakable               = 0x00000200,
      TwoUnitsIn1Egg          = 0x00000400,
      NeutralAccessories      = 0x00000800,
      ResourceDepot           = 0x00001000,
      ResourceContainer       = 0x00002000,
      RoboticUnit             = 0x00004000,
      Detector                = 0x00008000,
      OrganicUnit             = 0x00010000,
      CreepBuilding           = 0x00020000,
      Unused                  = 0x00040000,
      RequiresPsi             = 0x00080000,
      Burrowable              = 0x00100000,
      Spellcaster             = 0x00200000,
      PermanentCloak          = 0x00400000,
      NPCOrAccessories        = 0x00800000,
      MorphFromOtherUnit      = 0x01000000,
      LargeUnit               = 0x02000000,
      HugeUnit                = 0x04000000,
      AutoAttackAndMove       = 0x08000000,
      Attack                  = 0x10000000, /**< Can attack */
      Invincible              = 0x20000000,
      Mechanical              = 0x40000000,
      ProducesUnits           = 0x80000000 /**< It can produce units directly (making buildings doesn't count) */
    };
  }
};

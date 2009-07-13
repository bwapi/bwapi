#include <Util/Types.h>

namespace BW
{
  /**
   * Enumeration of the damage types. From DatEdit x1.5
   */
  namespace DamageID
  {
    enum Enum : u8
    {
      Independent = 0x00,
      Explosive   = 0x01,
      Concussive  = 0x02,
      Normal      = 0x03,
      IgnoreArmor = 0x04
    };
  };
};

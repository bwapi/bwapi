#include <Util/Types.h>

namespace BW
{
  /**
   * Enumeration of the explosion types. From DatEdit x1.5
   */
  namespace ExplosionID
  {
  enum Enum
    {
      None            = 0x00,
      Normal          = 0x01,
      Radial_Splash   = 0x02,
      Enemy_Splash    = 0x03,
      Lockdown        = 0x04,
      Nuclear_Missile = 0x05,
      Parasite        = 0x06,
      Broodlings      = 0x07,
      EMP_Shockwave   = 0x08,
      Irradiate       = 0x09,
      Ensnare         = 0x0A,
      Plague          = 0x0B,
      Stasis_Field    = 0x0C,
      Dark_Swarm      = 0x0D,
      Consume         = 0x0E,
      Yamato_Gun      = 0x0F,
      Restoration     = 0x10,
      Disruption_Web  = 0x11,
      Corrosive_Acid  = 0x12,
      Mind_Control    = 0x13,
      Feedback        = 0x14,
      Optical_Flare   = 0x15,
      Maelstrom       = 0x16,
      Unused          = 0x17,
      Air_Splash      = 0x18
    };
  };
};

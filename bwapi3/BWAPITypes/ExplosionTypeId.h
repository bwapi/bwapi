#pragma once

namespace BWAPI
{
  namespace ExplosionTypeIds
  {
    enum Enum
    {
      None = 0,
      Normal = 1,
      Radial_Splash = 2,
      Enemy_Splash = 3,
      Lockdown = 4,
      Nuclear_Missile = 5,
      Parasite = 6,
      Broodlings = 7,
      EMP_Shockwave = 8,
      Irradiate = 9,
      Ensnare = 10,
      Plague = 11,
      Stasis_Field = 12,
      Dark_Swarm = 13,
      Consume = 14,
      Yamato_Gun = 15,
      Restoration = 16,
      Disruption_Web = 17,
      Corrosive_Acid = 18,
      Mind_Control = 19,
      Feedback = 20,
      Optical_Flare = 21,
      Maelstrom = 22,
      Air_Splash = 24,
      count = 26
    };
  }
  typedef ExplosionTypeIds::Enum ExplosionTypeId;
}

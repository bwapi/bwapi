#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  namespace WeaponTypeIds
  {
    enum Enum
    {
      Gauss_Rifle = 0,
      C_10_Canister_Rifle = 2,
      Fragmentation_Grenade = 4,
      Spider_Mines = 6,
      Twin_Autocannons = 7,
      Hellfire_Missile_Pack = 8,
      Arclite_Cannon = 11,
      Fusion_Cutter = 13,
      Gemini_Missiles = 15,
      Burst_Lasers = 16,
      ATS_Laser_Battery = 19,
      ATA_Laser_Battery = 20,
      Flame_Thrower = 25,
      Arclite_Shock_Cannon = 27,
      Longbolt_Missile = 29,
      Claws = 35,
      Needle_Spines = 38,
      Kaiser_Blades = 40,
      Toxic_Spores = 42,
      Spines = 43,
      Acid_Spore = 46,
      Glave_Wurm = 48,
      Seeker_Spores = 52,
      Subterranean_Tentacle = 53,
      Suicide_Infested_Terran = 54,
      Suicide_Scourge = 55,
      Particle_Beam = 62,
      Psi_Blades = 64,
      Phase_Disruptor = 66,
      Psionic_Shockwave = 70,
      Dual_Photon_Blasters = 73,
      Anti_Matter_Missiles = 74,
      Phase_Disruptor_Cannon = 77,
      Pulse_Cannon = 79,
      STS_Photon_Cannon = 80,
      STA_Photon_Cannon = 81,
      Scarab = 82,
      Neutron_Flare = 100,
      Halo_Rockets = 103,
      Corrosive_Acid = 104,
      Subterranean_Spines = 109,
      Warp_Blades = 111,

      Yamato_Gun = 30,
      Nuclear_Strike = 31,
      Lockdown = 32,
      EMP_Shockwave = 33,
      Irradiate = 34,
      Parasite = 56,
      Spawn_Broodlings = 57,
      Ensnare = 58,
      Dark_Swarm = 59,
      Plague = 60,
      Consume = 61,
      Stasis_Field = 83,
      Psionic_Storm = 84,
      Disruption_Web = 101,
      Restoration = 102,
      Mind_Control = 105,
      Feedback = 106,
      Optical_Flare = 107,
      Maelstrom = 108,

      None = 130,
      count
    };
  }
  typedef WeaponTypeIds::Enum WeaponTypeId;
}

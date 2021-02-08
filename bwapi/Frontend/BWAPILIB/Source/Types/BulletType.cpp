#include <string>
#include <BWAPI/BulletType.h>

namespace BWAPI
{
  template <>
  const std::string Type<BulletType, BulletTypes::Enum::Unknown>::typeNames[BulletTypes::Enum::MAX] =
  {
    "Melee", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
    "", "", "", "", "", "", "", "", "", "", "", "", "", "Fusion_Cutter_Hit",
    "Gauss_Rifle_Hit", "C_10_Canister_Rifle_Hit", "Gemini_Missiles", "Fragmentation_Grenade",
    "Longbolt_Missile", "", "ATS_ATA_Laser_Battery", "Burst_Lasers", "Arclite_Shock_Cannon_Hit",
    "EMP_Missile", "Dual_Photon_Blasters_Hit", "Particle_Beam_Hit", "Anti_Matter_Missile", "Pulse_Cannon",
    "Psionic_Shockwave_Hit", "Psionic_Storm", "Yamato_Gun", "Phase_Disruptor", "STA_STS_Cannon_Overlay",
    "Sunken_Colony_Tentacle", "", "Acid_Spore", "", "Glave_Wurm", "Seeker_Spores", "Queen_Spell_Carrier",
    "Plague_Cloud", "Consume", "Ensnare", "Needle_Spine_Hit", "Invisible", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Optical_Flare_Grenade",
    "Halo_Rockets", "Subterranean_Spines", "Corrosive_Acid_Shot", "Corrosive_Acid_Hit", "Neutron_Flare", 
    "", "", "None", "Unknown"
  };
  //const std::string (&BulletType::typeNames)[BulletTypes::Enum::MAX] = bulletTypeNames;

  namespace BulletTypeSet
  {
    using namespace BulletTypes;
    const BulletType::set bulletTypeSet = { Melee, Fusion_Cutter_Hit, Gauss_Rifle_Hit, C_10_Canister_Rifle_Hit,
      Gemini_Missiles, Fragmentation_Grenade, Longbolt_Missile, ATS_ATA_Laser_Battery,
      Burst_Lasers, Arclite_Shock_Cannon_Hit, EMP_Missile, Dual_Photon_Blasters_Hit,
      Particle_Beam_Hit, Anti_Matter_Missile, Pulse_Cannon, Psionic_Shockwave_Hit, Psionic_Storm,
      Yamato_Gun, Phase_Disruptor, STA_STS_Cannon_Overlay, Sunken_Colony_Tentacle, Acid_Spore,
      Glave_Wurm, Seeker_Spores, Queen_Spell_Carrier, Plague_Cloud, Consume, Ensnare,
      Needle_Spine_Hit, Invisible, Optical_Flare_Grenade, Halo_Rockets, Subterranean_Spines,
      Corrosive_Acid_Shot, Neutron_Flare, None, Unknown };
  }
  const BulletType::set& BulletTypes::allBulletTypes()
  {
    return BulletTypeSet::bulletTypeSet;
  }
}

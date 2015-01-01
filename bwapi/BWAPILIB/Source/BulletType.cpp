#include <string>
#include <BWAPI/BulletType.h>

#include "Common.h"

#include "../../Debug.h"

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
    using namespace BulletTypes::Enum;
    const BulletType::set bulletTypeSet = { Melee, Fusion_Cutter_Hit, Gauss_Rifle_Hit, C_10_Canister_Rifle_Hit,
      Gemini_Missiles, Fragmentation_Grenade, Longbolt_Missile, ATS_ATA_Laser_Battery,
      Burst_Lasers, Arclite_Shock_Cannon_Hit, EMP_Missile, Dual_Photon_Blasters_Hit,
      Particle_Beam_Hit, Anti_Matter_Missile, Pulse_Cannon, Psionic_Shockwave_Hit, Psionic_Storm,
      Yamato_Gun, Phase_Disruptor, STA_STS_Cannon_Overlay, Sunken_Colony_Tentacle, Acid_Spore,
      Glave_Wurm, Seeker_Spores, Queen_Spell_Carrier, Plague_Cloud, Consume, Ensnare,
      Needle_Spine_Hit, Invisible, Optical_Flare_Grenade, Halo_Rockets, Subterranean_Spines,
      Corrosive_Acid_Shot, Neutron_Flare, None, Unknown };

  }
  namespace BulletTypes
  {
    BWAPI_TYPEDEF(BulletType,Melee);
    BWAPI_TYPEDEF(BulletType,Fusion_Cutter_Hit);
    BWAPI_TYPEDEF(BulletType,Gauss_Rifle_Hit);
    BWAPI_TYPEDEF(BulletType,C_10_Canister_Rifle_Hit);
    BWAPI_TYPEDEF(BulletType,Gemini_Missiles);
    BWAPI_TYPEDEF(BulletType,Fragmentation_Grenade);
    BWAPI_TYPEDEF(BulletType,Longbolt_Missile);
    BWAPI_TYPEDEF(BulletType,ATS_ATA_Laser_Battery);
    BWAPI_TYPEDEF(BulletType,Burst_Lasers);
    BWAPI_TYPEDEF(BulletType,Arclite_Shock_Cannon_Hit);
    BWAPI_TYPEDEF(BulletType,EMP_Missile);
    BWAPI_TYPEDEF(BulletType,Dual_Photon_Blasters_Hit);
    BWAPI_TYPEDEF(BulletType,Particle_Beam_Hit);
    BWAPI_TYPEDEF(BulletType,Anti_Matter_Missile);
    BWAPI_TYPEDEF(BulletType,Pulse_Cannon);
    BWAPI_TYPEDEF(BulletType,Psionic_Shockwave_Hit);
    BWAPI_TYPEDEF(BulletType,Psionic_Storm);
    BWAPI_TYPEDEF(BulletType,Yamato_Gun);
    BWAPI_TYPEDEF(BulletType,Phase_Disruptor);
    BWAPI_TYPEDEF(BulletType,STA_STS_Cannon_Overlay);
    BWAPI_TYPEDEF(BulletType,Sunken_Colony_Tentacle);
    BWAPI_TYPEDEF(BulletType,Acid_Spore);
    BWAPI_TYPEDEF(BulletType,Glave_Wurm);
    BWAPI_TYPEDEF(BulletType,Seeker_Spores);
    BWAPI_TYPEDEF(BulletType,Queen_Spell_Carrier);
    BWAPI_TYPEDEF(BulletType,Plague_Cloud);
    BWAPI_TYPEDEF(BulletType,Consume);
    BWAPI_TYPEDEF(BulletType,Ensnare);
    BWAPI_TYPEDEF(BulletType,Needle_Spine_Hit);
    BWAPI_TYPEDEF(BulletType,Invisible);
    BWAPI_TYPEDEF(BulletType,Optical_Flare_Grenade);
    BWAPI_TYPEDEF(BulletType,Halo_Rockets);
    BWAPI_TYPEDEF(BulletType,Subterranean_Spines);
    BWAPI_TYPEDEF(BulletType,Corrosive_Acid_Shot);
    BWAPI_TYPEDEF(BulletType,Neutron_Flare);
    BWAPI_TYPEDEF(BulletType,None);
    BWAPI_TYPEDEF(BulletType,Unknown);
  }

  BulletType::BulletType(int id) : Type( id )
  {}
  const BulletType::set& BulletTypes::allBulletTypes()
  {
    return BulletTypeSet::bulletTypeSet;
  }
}

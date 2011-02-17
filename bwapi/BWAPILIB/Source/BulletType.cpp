#include <string>
#include <map>
#include <set>
#include <BWAPI/BulletType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingBulletType = true;
  std::string bulletTypeName[211];
  std::map<std::string, BulletType> bulletTypeMap;
  std::set< BulletType > bulletTypeSet;
  namespace BulletTypes
  {
    const BulletType Melee(0);
    const BulletType Fusion_Cutter_Hit(141);
    const BulletType Gauss_Rifle_Hit(142);
    const BulletType C_10_Canister_Rifle_Hit(143);
    const BulletType Gemini_Missiles(144);
    const BulletType Fragmentation_Grenade(145);
    const BulletType Longbolt_Missile(146);
    const BulletType ATS_ATA_Laser_Battery(148);
    const BulletType Burst_Lasers(149);
    const BulletType Arclite_Shock_Cannon_Hit(150);
    const BulletType EMP_Missile(151);
    const BulletType Dual_Photon_Blasters_Hit(152);
    const BulletType Particle_Beam_Hit(153);
    const BulletType Anti_Matter_Missile(154);
    const BulletType Pulse_Cannon(155);
    const BulletType Psionic_Shockwave_Hit(156);
    const BulletType Psionic_Storm(157);
    const BulletType Yamato_Gun(158);
    const BulletType Phase_Disruptor(159);
    const BulletType STA_STS_Cannon_Overlay(160);
    const BulletType Sunken_Colony_Tentacle(161);
    const BulletType Acid_Spore(163);
    const BulletType Glave_Wurm(165);
    const BulletType Seeker_Spores(166);
    const BulletType Queen_Spell_Carrier(167);
    const BulletType Plague_Cloud(168);
    const BulletType Consume(169);
    const BulletType Needle_Spine_Hit(171);
    const BulletType Invisible(172);
    const BulletType Optical_Flare_Grenade(201);
    const BulletType Halo_Rockets(202);
    const BulletType Subterranean_Spines(203);
    const BulletType Corrosive_Acid_Shot(204);
    const BulletType Neutron_Flare(206);
    const BulletType None(209);
    const BulletType Unknown(210);

    void init()
    {
      bulletTypeName[Melee]                    = "Melee";
      bulletTypeName[Fusion_Cutter_Hit]        = "Fusion Cutter Hit";
      bulletTypeName[Gauss_Rifle_Hit]          = "Gauss Rifle Hit";
      bulletTypeName[C_10_Canister_Rifle_Hit]  = "C-10 Canister Rifle Hit";
      bulletTypeName[Gemini_Missiles]          = "Gemini Missiles";
      bulletTypeName[Fragmentation_Grenade]    = "Fragmentation Grenade";
      bulletTypeName[Longbolt_Missile]         = "Longbolt Missile";
      bulletTypeName[ATS_ATA_Laser_Battery]    = "ATS ATA Laser Battery";
      bulletTypeName[Burst_Lasers]             = "Burst Lasers";
      bulletTypeName[Arclite_Shock_Cannon_Hit] = "Arclite Shock Cannon Hit";
      bulletTypeName[EMP_Missile]              = "EMP Missile";
      bulletTypeName[Dual_Photon_Blasters_Hit] = "Dual Photon Blasters Hit";
      bulletTypeName[Particle_Beam_Hit]        = "Particle Beam Hit";
      bulletTypeName[Anti_Matter_Missile]      = "Anti-Matter Missile";
      bulletTypeName[Pulse_Cannon]             = "Pulse Cannon";
      bulletTypeName[Psionic_Shockwave_Hit]    = "Psionic Shockwave Hit";
      bulletTypeName[Psionic_Storm]            = "Psionic Storm";
      bulletTypeName[Yamato_Gun]               = "Yamato Gun";
      bulletTypeName[Phase_Disruptor]          = "Phase Disruptor";
      bulletTypeName[STA_STS_Cannon_Overlay]   = "STA STS Cannon Overlay";
      bulletTypeName[Sunken_Colony_Tentacle]   = "Sunken Colony Tentacle";
      bulletTypeName[Acid_Spore]               = "Acid Spore";
      bulletTypeName[Glave_Wurm]               = "Glave Wurm";
      bulletTypeName[Seeker_Spores]            = "Seeker Spores";
      bulletTypeName[Queen_Spell_Carrier]      = "Queen Spell Carrier";
      bulletTypeName[Plague_Cloud]             = "Plague Cloud";
      bulletTypeName[Consume]                  = "Consume";
      bulletTypeName[Needle_Spine_Hit]         = "Needle Spine Hit";
      bulletTypeName[Invisible]                = "Invisible";
      bulletTypeName[Optical_Flare_Grenade]    = "Optical Flare Grenade";
      bulletTypeName[Halo_Rockets]             = "Halo Rockets";
      bulletTypeName[Subterranean_Spines]      = "Subterranean Spines";
      bulletTypeName[Corrosive_Acid_Shot]      = "Corrosive Acid Shot";
      bulletTypeName[Neutron_Flare]            = "Neutron Flare";
      bulletTypeName[None]                     = "None";
      bulletTypeName[Unknown]                  = "Unknown";

      bulletTypeSet.insert(Melee);
      bulletTypeSet.insert(Fusion_Cutter_Hit);
      bulletTypeSet.insert(Gauss_Rifle_Hit);
      bulletTypeSet.insert(C_10_Canister_Rifle_Hit);
      bulletTypeSet.insert(Gemini_Missiles);
      bulletTypeSet.insert(Fragmentation_Grenade);
      bulletTypeSet.insert(Longbolt_Missile);
      bulletTypeSet.insert(ATS_ATA_Laser_Battery);
      bulletTypeSet.insert(Burst_Lasers);
      bulletTypeSet.insert(Arclite_Shock_Cannon_Hit);
      bulletTypeSet.insert(EMP_Missile);
      bulletTypeSet.insert(Dual_Photon_Blasters_Hit);
      bulletTypeSet.insert(Particle_Beam_Hit);
      bulletTypeSet.insert(Anti_Matter_Missile);
      bulletTypeSet.insert(Pulse_Cannon);
      bulletTypeSet.insert(Psionic_Shockwave_Hit);
      bulletTypeSet.insert(Psionic_Storm);
      bulletTypeSet.insert(Yamato_Gun);
      bulletTypeSet.insert(Phase_Disruptor);
      bulletTypeSet.insert(STA_STS_Cannon_Overlay);
      bulletTypeSet.insert(Sunken_Colony_Tentacle);
      bulletTypeSet.insert(Acid_Spore);
      bulletTypeSet.insert(Glave_Wurm);
      bulletTypeSet.insert(Seeker_Spores);
      bulletTypeSet.insert(Queen_Spell_Carrier);
      bulletTypeSet.insert(Plague_Cloud);
      bulletTypeSet.insert(Consume);
      bulletTypeSet.insert(Needle_Spine_Hit);
      bulletTypeSet.insert(Invisible);
      bulletTypeSet.insert(Optical_Flare_Grenade);
      bulletTypeSet.insert(Halo_Rockets);
      bulletTypeSet.insert(Subterranean_Spines);
      bulletTypeSet.insert(Corrosive_Acid_Shot);
      bulletTypeSet.insert(Neutron_Flare);
      bulletTypeSet.insert(None);
      bulletTypeSet.insert(Unknown);

      foreach(BulletType i, bulletTypeSet)
      {
        std::string name = i.getName();
        fixName(&name);
        bulletTypeMap.insert(std::make_pair(name, i));
      }
      initializingBulletType = false;
    }
  }

  BulletType::BulletType()
  {
    this->id = BulletTypes::None.id;
  }
  BulletType::BulletType(int id)
  {
    this->id = id;
    if (!initializingBulletType && (id < 0 || id >= 211 || bulletTypeName[id].length() == 0))
      this->id = BulletTypes::Unknown.id;
  }
  BulletType::BulletType(const BulletType& other)
  {
    this->id = other.id;
  }
  BulletType& BulletType::operator=(const BulletType& other)
  {
    this->id = other.id;
    return *this;
  }
  BulletType::operator int() const
  {
    return id;
  }
  int BulletType::getID() const
  {
    return this->id;
  }
  std::string BulletType::getName() const
  {
    return bulletTypeName[this->id];
  }
  BulletType BulletTypes::getBulletType(std::string name)
  {
    fixName(&name);
    std::map<std::string, BulletType>::iterator i = bulletTypeMap.find(name);
    if (i == bulletTypeMap.end())
      return BulletTypes::Unknown;
    return (*i).second;
  }
  std::set<BulletType>& BulletTypes::allBulletTypes()
  {
    return bulletTypeSet;
  }
}

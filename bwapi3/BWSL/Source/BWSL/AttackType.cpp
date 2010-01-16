#include "AttackType.h"

#include <string>
#include <map>
#include <set>

#include <BWAPI/all.h>

namespace BWSL
{
  bool initializingAttackType = true;
  std::string attackTypeName[211];
  std::map<std::string, AttackType> attackTypeMap;
  std::set< AttackType > attackTypeSet;
  namespace AttackTypes
  {
    BWSL_EXPORT const AttackType Melee(0);
    BWSL_EXPORT const AttackType Fusion_Cutter_Hit(141);
    BWSL_EXPORT const AttackType Gauss_Rifle_Hit(142);
    BWSL_EXPORT const AttackType C_10_Canister_Rifle_Hit(143);
    BWSL_EXPORT const AttackType Gemini_Missiles(144);
    BWSL_EXPORT const AttackType Fragmentation_Grenade(145);
    BWSL_EXPORT const AttackType Longbolt_Missile(146);
    BWSL_EXPORT const AttackType ATS_ATA_Laser_Battery(148);
    BWSL_EXPORT const AttackType Burst_Lasers(149);
    BWSL_EXPORT const AttackType Arclite_Shock_Cannon_Hit(150);
    BWSL_EXPORT const AttackType EMP_Missile(151);
    BWSL_EXPORT const AttackType Dual_Photon_Blasters_Hit(152);
    BWSL_EXPORT const AttackType Particle_Beam_Hit(153);
    BWSL_EXPORT const AttackType Anti_Matter_Missile(154);
    BWSL_EXPORT const AttackType Pulse_Cannon(155);
    BWSL_EXPORT const AttackType Psionic_Shockwave_Hit(156);
    BWSL_EXPORT const AttackType Psionic_Storm(157);
    BWSL_EXPORT const AttackType Yamato_Gun(158);
    BWSL_EXPORT const AttackType Phase_Disruptor(159);
    BWSL_EXPORT const AttackType STA_STS_Cannon_Overlay(160);
    BWSL_EXPORT const AttackType Sunken_Colony_Tentacle(161);
    BWSL_EXPORT const AttackType Acid_Spore(163);
    BWSL_EXPORT const AttackType Glave_Wurm(165);
    BWSL_EXPORT const AttackType Seeker_Spores(166);
    BWSL_EXPORT const AttackType Queen_Spell_Carrier(167);
    BWSL_EXPORT const AttackType Plague_Cloud(168);
    BWSL_EXPORT const AttackType Consume(169);
    BWSL_EXPORT const AttackType Needle_Spine_Hit(171);
    BWSL_EXPORT const AttackType Invisible(172);
    BWSL_EXPORT const AttackType Optical_Flare_Grenade(201);
    BWSL_EXPORT const AttackType Halo_Rockets(202);
    BWSL_EXPORT const AttackType Subterranean_Spines(203);
    BWSL_EXPORT const AttackType Corrosive_Acid_Shot(204);
    BWSL_EXPORT const AttackType Neutron_Flare(206);
    BWSL_EXPORT const AttackType None(209);
    BWSL_EXPORT const AttackType Unknown(210);
    void init()
    {
      attackTypeName[Melee.getID()] = "Melee";
      attackTypeName[Fusion_Cutter_Hit.getID()] = "Fusion Cutter Hit";
      attackTypeName[Gauss_Rifle_Hit.getID()] = "Gauss Rifle Hit";
      attackTypeName[C_10_Canister_Rifle_Hit.getID()] = "C-10 Canister Rifle Hit";
      attackTypeName[Gemini_Missiles.getID()] = "Gemini Missiles";
      attackTypeName[Fragmentation_Grenade.getID()] = "Fragmentation Grenade";
      attackTypeName[Longbolt_Missile.getID()] = "Longbolt Missile";
      attackTypeName[ATS_ATA_Laser_Battery.getID()] = "ATS ATA Laser Battery";
      attackTypeName[Burst_Lasers.getID()] = "Burst Lasers";
      attackTypeName[Arclite_Shock_Cannon_Hit.getID()] = "Arclite Shock Cannon Hit";
      attackTypeName[EMP_Missile.getID()] = "EMP Missile";
      attackTypeName[Dual_Photon_Blasters_Hit.getID()] = "Dual Photon Blasters Hit";
      attackTypeName[Particle_Beam_Hit.getID()] = "Particle Beam Hit";
      attackTypeName[Anti_Matter_Missile.getID()] = "Anti-Matter Missile";
      attackTypeName[Pulse_Cannon.getID()] = "Pulse Cannon";
      attackTypeName[Psionic_Shockwave_Hit.getID()] = "Psionic Shockwave Hit";
      attackTypeName[Psionic_Storm.getID()] = "Psionic Storm";
      attackTypeName[Yamato_Gun.getID()] = "Yamato Gun";
      attackTypeName[Phase_Disruptor.getID()] = "Phase Disruptor";
      attackTypeName[STA_STS_Cannon_Overlay.getID()] = "STA STS Cannon Overlay";
      attackTypeName[Sunken_Colony_Tentacle.getID()] = "Sunken Colony Tentacle";
      attackTypeName[Acid_Spore.getID()] = "Acid Spore";
      attackTypeName[Glave_Wurm.getID()] = "Glave Wurm";
      attackTypeName[Seeker_Spores.getID()] = "Seeker Spores";
      attackTypeName[Queen_Spell_Carrier.getID()] = "Queen Spell Carrier";
      attackTypeName[Plague_Cloud.getID()] = "Plague Cloud";
      attackTypeName[Consume.getID()] = "Consume";
      attackTypeName[Needle_Spine_Hit.getID()] = "Needle Spine Hit";
      attackTypeName[Invisible.getID()] = "Invisible";
      attackTypeName[Optical_Flare_Grenade.getID()] = "Optical Flare Grenade";
      attackTypeName[Halo_Rockets.getID()] = "Halo Rockets";
      attackTypeName[Subterranean_Spines.getID()] = "Subterranean Spines";
      attackTypeName[Corrosive_Acid_Shot.getID()] = "Corrosive Acid Shot";
      attackTypeName[Neutron_Flare.getID()] = "Neutron Flare";
      attackTypeName[None.getID()] = "None";
      attackTypeName[Unknown.getID()] = "Unknown";
      attackTypeSet.insert(Melee);
      attackTypeSet.insert(Fusion_Cutter_Hit);
      attackTypeSet.insert(Gauss_Rifle_Hit);
      attackTypeSet.insert(C_10_Canister_Rifle_Hit);
      attackTypeSet.insert(Gemini_Missiles);
      attackTypeSet.insert(Fragmentation_Grenade);
      attackTypeSet.insert(Longbolt_Missile);
      attackTypeSet.insert(ATS_ATA_Laser_Battery);
      attackTypeSet.insert(Burst_Lasers);
      attackTypeSet.insert(Arclite_Shock_Cannon_Hit);
      attackTypeSet.insert(EMP_Missile);
      attackTypeSet.insert(Dual_Photon_Blasters_Hit);
      attackTypeSet.insert(Particle_Beam_Hit);
      attackTypeSet.insert(Anti_Matter_Missile);
      attackTypeSet.insert(Pulse_Cannon);
      attackTypeSet.insert(Psionic_Shockwave_Hit);
      attackTypeSet.insert(Psionic_Storm);
      attackTypeSet.insert(Yamato_Gun);
      attackTypeSet.insert(Phase_Disruptor);
      attackTypeSet.insert(STA_STS_Cannon_Overlay);
      attackTypeSet.insert(Sunken_Colony_Tentacle);
      attackTypeSet.insert(Acid_Spore);
      attackTypeSet.insert(Glave_Wurm);
      attackTypeSet.insert(Seeker_Spores);
      attackTypeSet.insert(Queen_Spell_Carrier);
      attackTypeSet.insert(Plague_Cloud);
      attackTypeSet.insert(Consume);
      attackTypeSet.insert(Needle_Spine_Hit);
      attackTypeSet.insert(Invisible);
      attackTypeSet.insert(Optical_Flare_Grenade);
      attackTypeSet.insert(Halo_Rockets);
      attackTypeSet.insert(Subterranean_Spines);
      attackTypeSet.insert(Corrosive_Acid_Shot);
      attackTypeSet.insert(Neutron_Flare);
      attackTypeSet.insert(None);
      attackTypeSet.insert(Unknown);
      for(std::set<AttackType>::iterator i = attackTypeSet.begin(); i != attackTypeSet.end(); i++)
      {
        attackTypeMap.insert(std::make_pair((*i).getName(), *i));
      }
      initializingAttackType = false;
    }
  }

  AttackType::AttackType()
  {
    this->id = AttackTypes::None.id;
  }
  AttackType::AttackType(int id)
  {
    this->id = id;
    if (!initializingAttackType)
    {
      if (id < 0 || id >= 211 || attackTypeName[id].length() == 0)
      {
        this->id = AttackTypes::Unknown.id;
      }
    }
  }
  AttackType::AttackType(const AttackType& other)
  {
    this->id = other.id;
  }
  AttackType& AttackType::operator=(const AttackType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool AttackType::operator==(const AttackType& other) const
  {
    return this->id == other.id;
  }
  bool AttackType::operator!=(const AttackType& other) const
  {
    return this->id != other.id;
  }
  bool AttackType::operator<(const AttackType& other) const
  {
    return this->id < other.id;
  }
  int AttackType::getID() const
  {
    return this->id;
  }
  std::string AttackType::getName() const
  {
    return attackTypeName[this->id];
  }
  AttackType AttackTypes::getAttackType(std::string& name)
  {
    std::map<std::string, AttackType>::iterator i = attackTypeMap.find(name);
    if (i == attackTypeMap.end()) return AttackTypes::Unknown;
    return (*i).second;
  }
  std::set<AttackType>& AttackTypes::allAttackTypes()
  {
    return attackTypeSet;
  }
}

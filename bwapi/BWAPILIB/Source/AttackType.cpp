#include <string>
#include <map>
#include <set>
#include <BWAPI/AttackType.h>

namespace BWAPI
{
  bool initializingAttackType = true;
  std::string attackTypeName[211];
  std::map<std::string, AttackType> attackTypeMap;
  std::set< AttackType > attackTypeSet;
  namespace AttackTypes
  {
    const AttackType Archon_Energy(38);
    const AttackType Fusion_Cutter_Hit(141);
    const AttackType Gauss_Rifle_Hit(142);
    const AttackType C_10_Canister_Rifle_Hit(143);
    const AttackType Gemini_Missiles(144);
    const AttackType Fragmentation_Grenade(145);
    const AttackType Hellfire_Missile(146);
    const AttackType Unknown147(147);
    const AttackType ATS_ATA_Laser_Battery(148);
    const AttackType Burst_Lasers(149);
    const AttackType Arclite_Shock_Cannon_Hit(150);
    const AttackType EMP_Missile(151);
    const AttackType Dual_Photon_Blasters_Hit(152);
    const AttackType Particle_Beam_Hit(153);
    const AttackType Anti_Matter_Missile(154);
    const AttackType Pulse_Cannon(155);
    const AttackType Psionic_Shockwave_Hit(156);
    const AttackType Psionic_Storm(157);
    const AttackType Yamato_Gun(158);
    const AttackType Phase_Disruptor(159);
    const AttackType STA_STS_Cannon_Overlay(160);
    const AttackType Sunken_Colony_Tentacle(161);
    const AttackType Venom_Unused_Zerg_Weapon(162);
    const AttackType Acid_Spore(163);
    const AttackType Unknown164(164);
    const AttackType Glave_Wurm(165);
    const AttackType Seeker_Spores(166);
    const AttackType Queen_Spell_Carrier(167);
    const AttackType Plague_Cloud(168);
    const AttackType Consume(169);
    const AttackType Ensnare(170);
    const AttackType Needle_Spine_Hit(171);
    const AttackType White_Circle_Invisible(172);
    const AttackType Dark_Archon_Energy(187);
    const AttackType Optical_Flare_Grenade(201);
    const AttackType Halo_Rockets(202);
    const AttackType Subterranean_Spines(203);
    const AttackType Corrosive_Acid_Shot(204);
    const AttackType Corrosive_Acid_Hit(205);
    const AttackType Neutron_Flare(206);
    const AttackType None(209);
    const AttackType Unknown(210);
    void init()
    {
      attackTypeName[Archon_Energy.getID()] = "Archon Energy";
      attackTypeName[Fusion_Cutter_Hit.getID()] = "Fusion Cutter Hit";
      attackTypeName[Gauss_Rifle_Hit.getID()] = "Gauss Rifle Hit";
      attackTypeName[C_10_Canister_Rifle_Hit.getID()] = "C-10 Canister Rifle Hit";
      attackTypeName[Gemini_Missiles.getID()] = "Gemini Missiles";
      attackTypeName[Fragmentation_Grenade.getID()] = "Fragmentation Grenade";
      attackTypeName[Hellfire_Missile.getID()] = "Hellfire Missile";
      attackTypeName[Unknown147.getID()] = "Unknown147";
      attackTypeName[ATS_ATA_Laser_Battery.getID()] = "ATS ATA Laser Battery";
      attackTypeName[Burst_Lasers.getID()] = "Burst Lasers";
      attackTypeName[Arclite_Shock_Cannon_Hit.getID()] = "Arclite Shock Cannon Hit";
      attackTypeName[EMP_Missile.getID()] = "EMP Missile";
      attackTypeName[Dual_Photon_Blasters_Hit.getID()] = "Dual Photon Blasters Hit";
      attackTypeName[Particle_Beam_Hit.getID()] = "Particle Beam Hit";
      attackTypeName[Anti_Matter_Missile.getID()] = "Anti Matter Missile";
      attackTypeName[Pulse_Cannon.getID()] = "Pulse Cannon";
      attackTypeName[Psionic_Shockwave_Hit.getID()] = "Psionic Shockwave Hit";
      attackTypeName[Psionic_Storm.getID()] = "Psionic Storm";
      attackTypeName[Yamato_Gun.getID()] = "Yamato Gun";
      attackTypeName[Phase_Disruptor.getID()] = "Phase Disruptor";
      attackTypeName[STA_STS_Cannon_Overlay.getID()] = "STA STS Cannon Overlay";
      attackTypeName[Sunken_Colony_Tentacle.getID()] = "Sunken Colony Tentacle";
      attackTypeName[Venom_Unused_Zerg_Weapon.getID()] = "Venom Unused Zerg Weapon";
      attackTypeName[Acid_Spore.getID()] = "Acid Spore";
      attackTypeName[Unknown164.getID()] = "Unknown164";
      attackTypeName[Glave_Wurm.getID()] = "Glave Wurm";
      attackTypeName[Seeker_Spores.getID()] = "Seeker Spores";
      attackTypeName[Queen_Spell_Carrier.getID()] = "Queen Spell Carrier";
      attackTypeName[Plague_Cloud.getID()] = "Plague Cloud";
      attackTypeName[Consume.getID()] = "Consume";
      attackTypeName[Ensnare.getID()] = "Ensnare";
      attackTypeName[Needle_Spine_Hit.getID()] = "Needle Spine Hit";
      attackTypeName[White_Circle_Invisible.getID()] = "White Circle Invisible";//firebat
      attackTypeName[Dark_Archon_Energy.getID()] = "Dark Archon Energy";
      attackTypeName[Optical_Flare_Grenade.getID()] = "Optical Flare Grenade";
      attackTypeName[Halo_Rockets.getID()] = "Halo Rockets";
      attackTypeName[Subterranean_Spines.getID()] = "Subterranean Spines";
      attackTypeName[Corrosive_Acid_Shot.getID()] = "Corrosive Acid Shot";
      attackTypeName[Corrosive_Acid_Hit.getID()] = "Corrosive Acid Hit";
      attackTypeName[Neutron_Flare.getID()] = "Neutron Flare";
      attackTypeName[None.getID()] = "None";
      attackTypeName[Unknown.getID()] = "Unknown";
      attackTypeSet.insert(Archon_Energy);
      attackTypeSet.insert(Fusion_Cutter_Hit);
      attackTypeSet.insert(Gauss_Rifle_Hit);
      attackTypeSet.insert(C_10_Canister_Rifle_Hit);
      attackTypeSet.insert(Gemini_Missiles);
      attackTypeSet.insert(Fragmentation_Grenade);
      attackTypeSet.insert(Hellfire_Missile);
      attackTypeSet.insert(Unknown147);
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
      attackTypeSet.insert(Venom_Unused_Zerg_Weapon);
      attackTypeSet.insert(Acid_Spore);
      attackTypeSet.insert(Unknown164);
      attackTypeSet.insert(Glave_Wurm);
      attackTypeSet.insert(Seeker_Spores);
      attackTypeSet.insert(Queen_Spell_Carrier);
      attackTypeSet.insert(Plague_Cloud);
      attackTypeSet.insert(Consume);
      attackTypeSet.insert(Ensnare);
      attackTypeSet.insert(Needle_Spine_Hit);
      attackTypeSet.insert(White_Circle_Invisible);
      attackTypeSet.insert(Dark_Archon_Energy);
      attackTypeSet.insert(Optical_Flare_Grenade);
      attackTypeSet.insert(Halo_Rockets);
      attackTypeSet.insert(Subterranean_Spines);
      attackTypeSet.insert(Corrosive_Acid_Shot);
      attackTypeSet.insert(Corrosive_Acid_Hit);
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

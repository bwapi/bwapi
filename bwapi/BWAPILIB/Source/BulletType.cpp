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
    const BulletType Ensnare(170);
    const BulletType Needle_Spine_Hit(171);
    const BulletType Invisible(172);
    const BulletType Optical_Flare_Grenade(201);
    const BulletType Halo_Rockets(202);
    const BulletType Subterranean_Spines(203);
    const BulletType Corrosive_Acid_Shot(204);
    const BulletType Neutron_Flare(206);
    const BulletType None(209);
    const BulletType Unknown(210);

#define SET_BULLET_TYPE(x) bulletTypeName[x] = underscoresToSpaces(#x); bulletTypeSet.insert(x);
    void init()
    {
      SET_BULLET_TYPE(Melee);
      SET_BULLET_TYPE(Fusion_Cutter_Hit);
      SET_BULLET_TYPE(Gauss_Rifle_Hit);
      SET_BULLET_TYPE(C_10_Canister_Rifle_Hit);
      SET_BULLET_TYPE(Gemini_Missiles);
      SET_BULLET_TYPE(Fragmentation_Grenade);
      SET_BULLET_TYPE(Longbolt_Missile);
      SET_BULLET_TYPE(ATS_ATA_Laser_Battery);
      SET_BULLET_TYPE(Burst_Lasers);
      SET_BULLET_TYPE(Arclite_Shock_Cannon_Hit);
      SET_BULLET_TYPE(EMP_Missile);
      SET_BULLET_TYPE(Dual_Photon_Blasters_Hit);
      SET_BULLET_TYPE(Particle_Beam_Hit);
      SET_BULLET_TYPE(Anti_Matter_Missile);
      SET_BULLET_TYPE(Pulse_Cannon);
      SET_BULLET_TYPE(Psionic_Shockwave_Hit);
      SET_BULLET_TYPE(Psionic_Storm);
      SET_BULLET_TYPE(Yamato_Gun);
      SET_BULLET_TYPE(Phase_Disruptor);
      SET_BULLET_TYPE(STA_STS_Cannon_Overlay);
      SET_BULLET_TYPE(Sunken_Colony_Tentacle);
      SET_BULLET_TYPE(Acid_Spore);
      SET_BULLET_TYPE(Glave_Wurm);
      SET_BULLET_TYPE(Seeker_Spores);
      SET_BULLET_TYPE(Queen_Spell_Carrier);
      SET_BULLET_TYPE(Plague_Cloud);
      SET_BULLET_TYPE(Consume);
      SET_BULLET_TYPE(Ensnare);
      SET_BULLET_TYPE(Needle_Spine_Hit);
      SET_BULLET_TYPE(Invisible);
      SET_BULLET_TYPE(Optical_Flare_Grenade);
      SET_BULLET_TYPE(Halo_Rockets);
      SET_BULLET_TYPE(Subterranean_Spines);
      SET_BULLET_TYPE(Corrosive_Acid_Shot);
      SET_BULLET_TYPE(Neutron_Flare);
      SET_BULLET_TYPE(None);
      SET_BULLET_TYPE(Unknown);

      foreach(BulletType i, bulletTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        bulletTypeMap.insert(std::make_pair(name, i));
      }
      initializingBulletType = false;
    }
  }

  BulletType::BulletType()
  {
    this->id = BulletTypes::None;
  }
  BulletType::BulletType(int id)
  {
    this->id = id;
    if (!initializingBulletType && (id < 0 || id >= 211 || bulletTypeName[id].length() == 0))
      this->id = BulletTypes::Unknown;
  }
  BulletType::BulletType(const BulletType& other)
  {
    this->id = other;
  }
  BulletType& BulletType::operator=(const BulletType& other)
  {
    this->id = other;
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
  const char *BulletType::c_str() const
  {
    return bulletTypeName[this->id].c_str();
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

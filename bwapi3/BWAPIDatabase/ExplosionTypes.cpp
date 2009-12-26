#include "ExplosionTypes.h"

#include <string>
#include <map>
#include <set>

namespace BWAPI
{
  namespace ExplosionTypes
  {
    ExplosionTypeInternal explosionTypeData[ExplosionTypeIds::count];
    std::map<std::string, ExplosionTypeId> explosionTypeMap;
    std::set< ExplosionTypeId > explosionTypeSet;

    void fillExplosionType(ExplosionTypeId id, const char* name)
    {
      ExplosionTypeInternal &target = explosionTypeData[id];
      target.name = name;
    }

    void init()
    {
      fillExplosionType(ExplosionTypeIds::None, "None");
      fillExplosionType(ExplosionTypeIds::Normal, "Normal");
      fillExplosionType(ExplosionTypeIds::Radial_Splash, "Radial Splash");
      fillExplosionType(ExplosionTypeIds::Enemy_Splash, "Enemy Splash");
      fillExplosionType(ExplosionTypeIds::Lockdown, "Lockdown");
      fillExplosionType(ExplosionTypeIds::Nuclear_Missile, "Nuclear Missile");
      fillExplosionType(ExplosionTypeIds::Parasite, "Parasite");
      fillExplosionType(ExplosionTypeIds::Broodlings, "Broodlings");
      fillExplosionType(ExplosionTypeIds::EMP_Shockwave, "EMP Shockwave");
      fillExplosionType(ExplosionTypeIds::Irradiate, "Irradiate");
      fillExplosionType(ExplosionTypeIds::Ensnare, "Ensnare");
      fillExplosionType(ExplosionTypeIds::Plague, "Plague");
      fillExplosionType(ExplosionTypeIds::Stasis_Field, "Stasis Field");
      fillExplosionType(ExplosionTypeIds::Dark_Swarm, "Dark Swarm");
      fillExplosionType(ExplosionTypeIds::Consume, "Consume");
      fillExplosionType(ExplosionTypeIds::Yamato_Gun, "Yamato Gun");
      fillExplosionType(ExplosionTypeIds::Restoration, "Restoration");
      fillExplosionType(ExplosionTypeIds::Disruption_Web, "Disruption Web");
      fillExplosionType(ExplosionTypeIds::Corrosive_Acid, "Corrosive Acid");
      fillExplosionType(ExplosionTypeIds::Mind_Control, "Mind Control");
      fillExplosionType(ExplosionTypeIds::Feedback, "Feedback");
      fillExplosionType(ExplosionTypeIds::Optical_Flare, "Optical Flare");
      fillExplosionType(ExplosionTypeIds::Maelstrom, "Maelstrom");
      fillExplosionType(ExplosionTypeIds::Air_Splash, "Air Splash");

      explosionTypeSet.insert(ExplosionTypeIds::None);
      explosionTypeSet.insert(ExplosionTypeIds::Normal);
      explosionTypeSet.insert(ExplosionTypeIds::Radial_Splash);
      explosionTypeSet.insert(ExplosionTypeIds::Enemy_Splash);
      explosionTypeSet.insert(ExplosionTypeIds::Lockdown);
      explosionTypeSet.insert(ExplosionTypeIds::Nuclear_Missile);
      explosionTypeSet.insert(ExplosionTypeIds::Parasite);
      explosionTypeSet.insert(ExplosionTypeIds::Broodlings);
      explosionTypeSet.insert(ExplosionTypeIds::EMP_Shockwave);
      explosionTypeSet.insert(ExplosionTypeIds::Irradiate);
      explosionTypeSet.insert(ExplosionTypeIds::Ensnare);
      explosionTypeSet.insert(ExplosionTypeIds::Plague);
      explosionTypeSet.insert(ExplosionTypeIds::Stasis_Field);
      explosionTypeSet.insert(ExplosionTypeIds::Dark_Swarm);
      explosionTypeSet.insert(ExplosionTypeIds::Consume);
      explosionTypeSet.insert(ExplosionTypeIds::Yamato_Gun);
      explosionTypeSet.insert(ExplosionTypeIds::Restoration);
      explosionTypeSet.insert(ExplosionTypeIds::Disruption_Web);
      explosionTypeSet.insert(ExplosionTypeIds::Corrosive_Acid);
      explosionTypeSet.insert(ExplosionTypeIds::Mind_Control);
      explosionTypeSet.insert(ExplosionTypeIds::Feedback);
      explosionTypeSet.insert(ExplosionTypeIds::Optical_Flare);
      explosionTypeSet.insert(ExplosionTypeIds::Maelstrom);
      explosionTypeSet.insert(ExplosionTypeIds::Air_Splash);

      for(std::set<ExplosionTypeId>::iterator i = explosionTypeSet.begin(); i != explosionTypeSet.end(); i++)
      {
        explosionTypeMap.insert(std::make_pair(std::string(explosionTypeData[*i].name), *i));
      }
    }

    ExplosionTypeId getIdByName(const std::string& name)
    {
      std::map<std::string, ExplosionTypeId>::iterator i = explosionTypeMap.find(name);
      if (i == explosionTypeMap.end()) return ExplosionTypeIds::None;
      return (*i).second;
    }
  }
}

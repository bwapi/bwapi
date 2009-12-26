#include "ExplosionTypes.h"

#include <string>
#include <map>
#include <set>

namespace BWAPI
{
  ExplosionType explosionTypeName[26];
  std::map<std::string, ExplosionType> explosionTypeMap;
  std::set< ExplosionType > explosionTypeSet;

  namespace ExplosionTypes
  {
    void init()
    {
      explosionTypeName[None.getID()] = "None";
      explosionTypeName[Normal.getID()] = "Normal";
      explosionTypeName[Radial_Splash.getID()] = "Radial Splash";
      explosionTypeName[Enemy_Splash.getID()] = "Enemy Splash";
      explosionTypeName[Lockdown.getID()] = "Lockdown";
      explosionTypeName[Nuclear_Missile.getID()] = "Nuclear Missile";
      explosionTypeName[Parasite.getID()] = "Parasite";
      explosionTypeName[Broodlings.getID()] = "Broodlings";
      explosionTypeName[EMP_Shockwave.getID()] = "EMP Shockwave";
      explosionTypeName[Irradiate.getID()] = "Irradiate";
      explosionTypeName[Ensnare.getID()] = "Ensnare";
      explosionTypeName[Plague.getID()] = "Plague";
      explosionTypeName[Stasis_Field.getID()] = "Stasis Field";
      explosionTypeName[Dark_Swarm.getID()] = "Dark Swarm";
      explosionTypeName[Consume.getID()] = "Consume";
      explosionTypeName[Yamato_Gun.getID()] = "Yamato Gun";
      explosionTypeName[Restoration.getID()] = "Restoration";
      explosionTypeName[Disruption_Web.getID()] = "Disruption Web";
      explosionTypeName[Corrosive_Acid.getID()] = "Corrosive Acid";
      explosionTypeName[Mind_Control.getID()] = "Mind Control";
      explosionTypeName[Feedback.getID()] = "Feedback";
      explosionTypeName[Optical_Flare.getID()] = "Optical Flare";
      explosionTypeName[Maelstrom.getID()] = "Maelstrom";
      explosionTypeName[Air_Splash.getID()] = "Air Splash";
      explosionTypeName[Unknown.getID()] = "Unknown";

      explosionTypeSet.insert(None);
      explosionTypeSet.insert(Normal);
      explosionTypeSet.insert(Radial_Splash);
      explosionTypeSet.insert(Enemy_Splash);
      explosionTypeSet.insert(Lockdown);
      explosionTypeSet.insert(Nuclear_Missile);
      explosionTypeSet.insert(Parasite);
      explosionTypeSet.insert(Broodlings);
      explosionTypeSet.insert(EMP_Shockwave);
      explosionTypeSet.insert(Irradiate);
      explosionTypeSet.insert(Ensnare);
      explosionTypeSet.insert(Plague);
      explosionTypeSet.insert(Stasis_Field);
      explosionTypeSet.insert(Dark_Swarm);
      explosionTypeSet.insert(Consume);
      explosionTypeSet.insert(Yamato_Gun);
      explosionTypeSet.insert(Restoration);
      explosionTypeSet.insert(Disruption_Web);
      explosionTypeSet.insert(Corrosive_Acid);
      explosionTypeSet.insert(Mind_Control);
      explosionTypeSet.insert(Feedback);
      explosionTypeSet.insert(Optical_Flare);
      explosionTypeSet.insert(Maelstrom);
      explosionTypeSet.insert(Air_Splash);
      explosionTypeSet.insert(Unknown);

      for(std::set<ExplosionType>::iterator i = explosionTypeSet.begin(); i != explosionTypeSet.end(); i++)
      {
        explosionTypeMap.insert(std::make_pair((*i).getName(), *i));
      }
    }
  }

  namespace ExplosionTypes
  {
    ExplosionType getIdByName(std::string& name)
    {
      std::map<std::string, ExplosionType>::iterator i = explosionTypeMap.find(name);
      if (i == explosionTypeMap.end()) return ExplosionTypes::Unknown;
      return (*i).second;
    }
    std::set<ExplosionType>& allExplosionTypes()
    {
      return explosionTypeSet;
    }
  }
}

#include <string>
#include <map>
#include <set>
#include <BWAPI/ExplosionType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingExplosionType = true;
  std::string explosionTypeName[26];
  std::map<std::string, ExplosionType> explosionTypeMap;
  std::set< ExplosionType > explosionTypeSet;
  namespace ExplosionTypes
  {
    const ExplosionType None(0);
    const ExplosionType Normal(1);
    const ExplosionType Radial_Splash(2);
    const ExplosionType Enemy_Splash(3);
    const ExplosionType Lockdown(4);
    const ExplosionType Nuclear_Missile(5);
    const ExplosionType Parasite(6);
    const ExplosionType Broodlings(7);
    const ExplosionType EMP_Shockwave(8);
    const ExplosionType Irradiate(9);
    const ExplosionType Ensnare(10);
    const ExplosionType Plague(11);
    const ExplosionType Stasis_Field(12);
    const ExplosionType Dark_Swarm(13);
    const ExplosionType Consume(14);
    const ExplosionType Yamato_Gun(15);
    const ExplosionType Restoration(16);
    const ExplosionType Disruption_Web(17);
    const ExplosionType Corrosive_Acid(18);
    const ExplosionType Mind_Control(19);
    const ExplosionType Feedback(20);
    const ExplosionType Optical_Flare(21);
    const ExplosionType Maelstrom(22);
    const ExplosionType Air_Splash(24);
    const ExplosionType Unknown(25);

    void init()
    {
      explosionTypeName[None]            = "None";
      explosionTypeName[Normal]          = "Normal";
      explosionTypeName[Radial_Splash]   = "Radial Splash";
      explosionTypeName[Enemy_Splash]    = "Enemy Splash";
      explosionTypeName[Lockdown]        = "Lockdown";
      explosionTypeName[Nuclear_Missile] = "Nuclear Missile";
      explosionTypeName[Parasite]        = "Parasite";
      explosionTypeName[Broodlings]      = "Broodlings";
      explosionTypeName[EMP_Shockwave]   = "EMP Shockwave";
      explosionTypeName[Irradiate]       = "Irradiate";
      explosionTypeName[Ensnare]         = "Ensnare";
      explosionTypeName[Plague]          = "Plague";
      explosionTypeName[Stasis_Field]    = "Stasis Field";
      explosionTypeName[Dark_Swarm]      = "Dark Swarm";
      explosionTypeName[Consume]         = "Consume";
      explosionTypeName[Yamato_Gun]      = "Yamato Gun";
      explosionTypeName[Restoration]     = "Restoration";
      explosionTypeName[Disruption_Web]  = "Disruption Web";
      explosionTypeName[Corrosive_Acid]  = "Corrosive Acid";
      explosionTypeName[Mind_Control]    = "Mind Control";
      explosionTypeName[Feedback]        = "Feedback";
      explosionTypeName[Optical_Flare]   = "Optical Flare";
      explosionTypeName[Maelstrom]       = "Maelstrom";
      explosionTypeName[Air_Splash]      = "Air Splash";
      explosionTypeName[Unknown]         = "Unknown";

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

      foreach(ExplosionType i, explosionTypeSet)
      {
        std::string name = i.getName();
        fixName(&name);
        explosionTypeMap.insert(std::make_pair(name, i));
      }
      initializingExplosionType = false;
    }
  }
  ExplosionType::ExplosionType()
  {
    this->id = ExplosionTypes::None.id;
  }
  ExplosionType::ExplosionType(int id)
  {
    this->id = id;
    if (!initializingExplosionType && (id < 0 || id >= 26))
      this->id = ExplosionTypes::Unknown.id;
  }
  ExplosionType::ExplosionType(const ExplosionType& other)
  {
    this->id = other.id;
  }
  ExplosionType& ExplosionType::operator=(const ExplosionType& other)
  {
    this->id = other.id;
    return *this;
  }
  ExplosionType::operator int() const
  {
    return id;
  }
  int ExplosionType::getID() const
  {
    return this->id;
  }
  std::string ExplosionType::getName() const
  {
    return explosionTypeName[this->id];
  }

  ExplosionType ExplosionTypes::getExplosionType(std::string name)
  {
    fixName(&name);
    std::map<std::string, ExplosionType>::iterator i = explosionTypeMap.find(name);
    if (i == explosionTypeMap.end())
      return ExplosionTypes::Unknown;
    return (*i).second;
  }
  std::set<ExplosionType>& ExplosionTypes::allExplosionTypes()
  {
    return explosionTypeSet;
  }
}

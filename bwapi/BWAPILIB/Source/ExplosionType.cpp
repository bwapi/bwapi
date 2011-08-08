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

#define SET_EXPLOSION_TYPE(x) explosionTypeName[x] = underscoresToSpaces(#x); explosionTypeSet.insert(x);
    void init()
    {
      SET_EXPLOSION_TYPE(None);
      SET_EXPLOSION_TYPE(Normal);
      SET_EXPLOSION_TYPE(Radial_Splash);
      SET_EXPLOSION_TYPE(Enemy_Splash);
      SET_EXPLOSION_TYPE(Lockdown);
      SET_EXPLOSION_TYPE(Nuclear_Missile);
      SET_EXPLOSION_TYPE(Parasite);
      SET_EXPLOSION_TYPE(Broodlings);
      SET_EXPLOSION_TYPE(EMP_Shockwave);
      SET_EXPLOSION_TYPE(Irradiate);
      SET_EXPLOSION_TYPE(Ensnare);
      SET_EXPLOSION_TYPE(Plague);
      SET_EXPLOSION_TYPE(Stasis_Field);
      SET_EXPLOSION_TYPE(Dark_Swarm);
      SET_EXPLOSION_TYPE(Consume);
      SET_EXPLOSION_TYPE(Yamato_Gun);
      SET_EXPLOSION_TYPE(Restoration);
      SET_EXPLOSION_TYPE(Disruption_Web);
      SET_EXPLOSION_TYPE(Corrosive_Acid);
      SET_EXPLOSION_TYPE(Mind_Control);
      SET_EXPLOSION_TYPE(Feedback);
      SET_EXPLOSION_TYPE(Optical_Flare);
      SET_EXPLOSION_TYPE(Maelstrom);
      SET_EXPLOSION_TYPE(Air_Splash);
      SET_EXPLOSION_TYPE(Unknown);

      foreach(ExplosionType i, explosionTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        explosionTypeMap.insert(std::make_pair(name, i));
      }
      initializingExplosionType = false;
    }
  }
  ExplosionType::ExplosionType()
  {
    this->id = ExplosionTypes::None;
  }
  ExplosionType::ExplosionType(int id)
  {
    this->id = id;
    if (!initializingExplosionType && (id < 0 || id >= 26))
      this->id = ExplosionTypes::Unknown;
  }
  ExplosionType::ExplosionType(const ExplosionType& other)
  {
    this->id = other;
  }
  ExplosionType& ExplosionType::operator=(const ExplosionType& other)
  {
    this->id = other;
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
  const char *ExplosionType::c_str() const
  {
    return explosionTypeName[this->id].c_str();
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

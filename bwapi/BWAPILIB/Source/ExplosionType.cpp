#include <string>
#include <map>
#include <set>
#include <BWAPI/ExplosionType.h>

namespace BWAPI
{
  bool initializingExplosionType=true;
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
      explosionTypeName[None.getID()]="None";
      explosionTypeName[Normal.getID()]="Normal";
      explosionTypeName[Radial_Splash.getID()]="Radial Splash";
      explosionTypeName[Enemy_Splash.getID()]="Enemy Splash";
      explosionTypeName[Lockdown.getID()]="Lockdown";
      explosionTypeName[Nuclear_Missile.getID()]="Nuclear Missile";
      explosionTypeName[Parasite.getID()]="Parasite";
      explosionTypeName[Broodlings.getID()]="Broodlings";
      explosionTypeName[EMP_Shockwave.getID()]="EMP Shockwave";
      explosionTypeName[Irradiate.getID()]="Irradiate";
      explosionTypeName[Ensnare.getID()]="Ensnare";
      explosionTypeName[Plague.getID()]="Plague";
      explosionTypeName[Stasis_Field.getID()]="Stasis Field";
      explosionTypeName[Dark_Swarm.getID()]="Dark Swarm";
      explosionTypeName[Consume.getID()]="Consume";
      explosionTypeName[Yamato_Gun.getID()]="Yamato Gun";
      explosionTypeName[Restoration.getID()]="Restoration";
      explosionTypeName[Disruption_Web.getID()]="Disruption Web";
      explosionTypeName[Corrosive_Acid.getID()]="Corrosive Acid";
      explosionTypeName[Mind_Control.getID()]="Mind Control";
      explosionTypeName[Feedback.getID()]="Feedback";
      explosionTypeName[Optical_Flare.getID()]="Optical Flare";
      explosionTypeName[Maelstrom.getID()]="Maelstrom";
      explosionTypeName[Air_Splash.getID()]="Air Splash";
      explosionTypeName[Unknown.getID()]="Unknown";

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

      for(std::set<ExplosionType>::iterator i=explosionTypeSet.begin();i!=explosionTypeSet.end();i++)
      {
        explosionTypeMap.insert(std::make_pair((*i).getName(),*i));
      }
      initializingExplosionType=false;
    }
  }
  ExplosionType::ExplosionType()
  {
    this->id=ExplosionTypes::None.id;
  }
  ExplosionType::ExplosionType(int id)
  {
    this->id=id;
    if (!initializingExplosionType)
    {
      if (id<0 || id>=26)
      {
        this->id=ExplosionTypes::Unknown.id;
      }
    }
  }
  ExplosionType::ExplosionType(const ExplosionType &other)
  {
    this->id=other.id;
  }
  ExplosionType& ExplosionType::operator=(const ExplosionType &other)
  {
    this->id=other.id;
    return *this;
  }
  bool ExplosionType::operator==(const ExplosionType &other) const
  {
    return this->id==other.id;
  }
  bool ExplosionType::operator!=(const ExplosionType &other) const
  {
    return this->id!=other.id;
  }
  bool ExplosionType::operator<(const ExplosionType &other) const
  {
    return this->id<other.id;
  }
  int ExplosionType::getID() const
  {
    return this->id;
  }
  std::string ExplosionType::getName() const
  {
    return explosionTypeName[this->id];
  }
  
  ExplosionType ExplosionTypes::getExplosionType(std::string &name)
  {
    std::map<std::string, ExplosionType>::iterator i=explosionTypeMap.find(name);
    if (i==explosionTypeMap.end()) return ExplosionTypes::Unknown;
    return (*i).second;
  }
  std::set<ExplosionType>& ExplosionTypes::allExplosionTypes()
  {
    return explosionTypeSet;
  }
}

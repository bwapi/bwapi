#include "TechType.h"
#include "Race.h"
#include "UnitType.h"
#include "WeaponType.h"

#include <string>
#include <map>
#include <set>

namespace BWAPI2
{
  bool initializingTechType = true;
  class TechTypeInternal
  {
    public:
      TechTypeInternal() {valid = false;}
      void set(const char* name, int mineralPrice, int gasPrice, int energyUsed, const UnitType* whatResearches, Race race, const WeaponType* weapon, const UnitType* whatUses1, const UnitType* whatUses2=NULL, const UnitType* whatUses3=NULL, const UnitType* whatUses4=NULL)
      {
        this->name=name;
        this->mineralPrice=mineralPrice;
        this->gasPrice=gasPrice;
        this->energyUsed=energyUsed;
        this->whatResearches=whatResearches;
        this->race=race;
        this->weapon=weapon;
        if (whatUses1!=NULL && whatUses1!=&(UnitTypes::None))
        {
          this->whatUses.insert(whatUses1);
        }
        if (whatUses2!=NULL && whatUses2!=&(UnitTypes::None))
        {
          this->whatUses.insert(whatUses2);
        }
        if (whatUses3!=NULL && whatUses3!=&(UnitTypes::None))
        {
          this->whatUses.insert(whatUses3);
        }
        if (whatUses4!=NULL && whatUses4!=&(UnitTypes::None))
        {
          this->whatUses.insert(whatUses4);
        }
        this->valid = true;
      }
      std::string name;
      int mineralPrice;
      int gasPrice;
      int energyUsed;
      const UnitType* whatResearches;
      Race race;
      const WeaponType* weapon;
      std::set<const UnitType*> whatUses;

      bool valid;
  };
  TechTypeInternal techTypeData[47];
  std::map<std::string, TechType> techTypeMap;
  std::set< TechType > techTypeSet;
  namespace TechTypes
  {
    BWAPI2_EXPORT const TechType Stim_Packs(0);
    BWAPI2_EXPORT const TechType Lockdown(1);
    BWAPI2_EXPORT const TechType EMP_Shockwave(2);
    BWAPI2_EXPORT const TechType Spider_Mines(3);
    BWAPI2_EXPORT const TechType Scanner_Sweep(4);
    BWAPI2_EXPORT const TechType Tank_Siege_Mode(5);
    BWAPI2_EXPORT const TechType Defensive_Matrix(6);
    BWAPI2_EXPORT const TechType Irradiate(7);
    BWAPI2_EXPORT const TechType Yamato_Gun(8);
    BWAPI2_EXPORT const TechType Cloaking_Field(9);
    BWAPI2_EXPORT const TechType Personnel_Cloaking(10);
    BWAPI2_EXPORT const TechType Burrowing(11);
    BWAPI2_EXPORT const TechType Infestation(12);
    BWAPI2_EXPORT const TechType Spawn_Broodlings(13);
    BWAPI2_EXPORT const TechType Dark_Swarm(14);
    BWAPI2_EXPORT const TechType Plague(15);
    BWAPI2_EXPORT const TechType Consume(16);
    BWAPI2_EXPORT const TechType Ensnare(17);
    BWAPI2_EXPORT const TechType Parasite(18);
    BWAPI2_EXPORT const TechType Psionic_Storm(19);
    BWAPI2_EXPORT const TechType Hallucination(20);
    BWAPI2_EXPORT const TechType Recall(21);
    BWAPI2_EXPORT const TechType Stasis_Field(22);
    BWAPI2_EXPORT const TechType Archon_Warp(23);
    BWAPI2_EXPORT const TechType Restoration(24);
    BWAPI2_EXPORT const TechType Disruption_Web(25);
    BWAPI2_EXPORT const TechType Mind_Control(27);
    BWAPI2_EXPORT const TechType Dark_Archon_Meld(28);
    BWAPI2_EXPORT const TechType Feedback(29);
    BWAPI2_EXPORT const TechType Optical_Flare(30);
    BWAPI2_EXPORT const TechType Maelstrom(31);
    BWAPI2_EXPORT const TechType Lurker_Aspect(32);
    BWAPI2_EXPORT const TechType Healing(34);
    BWAPI2_EXPORT const TechType None(44);
    BWAPI2_EXPORT const TechType Unknown(45);
    BWAPI2_EXPORT const TechType Nuclear_Strike(46);
    void init()
    {
      techTypeData[Stim_Packs.getID()].set("Stim Packs",100,100,0,&(UnitTypes::Terran_Academy),Races::Terran,&(WeaponTypes::None),&(UnitTypes::Terran_Marine),&(UnitTypes::Terran_Firebat));
      techTypeData[Lockdown.getID()].set("Lockdown",200,200,100,&(UnitTypes::Terran_Covert_Ops),Races::Terran,&(WeaponTypes::Lockdown),&(UnitTypes::Terran_Ghost));
      techTypeData[EMP_Shockwave.getID()].set("EMP Shockwave",200,200,100,&(UnitTypes::Terran_Science_Facility),Races::Terran,&(WeaponTypes::EMP_Shockwave),&(UnitTypes::Terran_Science_Vessel));
      techTypeData[Spider_Mines.getID()].set("Spider Mines",100,100,0,&(UnitTypes::Terran_Machine_Shop),Races::Terran,&(WeaponTypes::Spider_Mines),&(UnitTypes::Terran_Vulture));
      techTypeData[Scanner_Sweep.getID()].set("Scanner Sweep",0,0,50,&(UnitTypes::None),Races::Terran,&(WeaponTypes::None),&(UnitTypes::Terran_Comsat_Station));
      techTypeData[Tank_Siege_Mode.getID()].set("Tank Siege Mode",150,150,0,&(UnitTypes::Terran_Machine_Shop),Races::Terran,&(WeaponTypes::None),&(UnitTypes::Terran_Siege_Tank_Tank_Mode),&(UnitTypes::Terran_Siege_Tank_Siege_Mode));
      techTypeData[Defensive_Matrix.getID()].set("Defensive Matrix",0,0,100,&(UnitTypes::None),Races::Terran,&(WeaponTypes::None),&(UnitTypes::Terran_Science_Vessel));
      techTypeData[Irradiate.getID()].set("Irradiate",200,200,75,&(UnitTypes::Terran_Science_Facility),Races::Terran,&(WeaponTypes::Irradiate),&(UnitTypes::Terran_Science_Vessel));
      techTypeData[Yamato_Gun.getID()].set("Yamato Gun",100,100,150,&(UnitTypes::Terran_Physics_Lab),Races::Terran,&(WeaponTypes::Yamato_Gun),&(UnitTypes::Terran_Battlecruiser));
      techTypeData[Cloaking_Field.getID()].set("Cloaking Field",150,150,25,&(UnitTypes::Terran_Control_Tower),Races::Terran,&(WeaponTypes::None),&(UnitTypes::Terran_Wraith));
      techTypeData[Personnel_Cloaking.getID()].set("Personnel Cloaking",100,100,25,&(UnitTypes::Terran_Covert_Ops),Races::Terran,&(WeaponTypes::None),&(UnitTypes::Terran_Ghost));
      techTypeData[Burrowing.getID()].set("Burrowing",100,100,0,&(UnitTypes::Zerg_Hatchery),Races::Zerg,&(WeaponTypes::None),&(UnitTypes::Zerg_Drone),&(UnitTypes::Zerg_Zergling),&(UnitTypes::Zerg_Hydralisk),&(UnitTypes::Zerg_Defiler));
      techTypeData[Infestation.getID()].set("Infestation",0,0,0,&(UnitTypes::None),Races::Zerg,&(WeaponTypes::None),&(UnitTypes::Zerg_Queen));
      techTypeData[Spawn_Broodlings.getID()].set("Spawn Broodlings",100,100,150,&(UnitTypes::Zerg_Queens_Nest),Races::Zerg,&(WeaponTypes::Spawn_Broodlings),&(UnitTypes::Zerg_Queen));
      techTypeData[Dark_Swarm.getID()].set("Dark Swarm",0,0,100,&(UnitTypes::None),Races::Zerg,&(WeaponTypes::Dark_Swarm),&(UnitTypes::Zerg_Defiler));
      techTypeData[Plague.getID()].set("Plague",200,200,150,&(UnitTypes::Zerg_Defiler_Mound),Races::Zerg,&(WeaponTypes::Plague),&(UnitTypes::Zerg_Defiler));
      techTypeData[Consume.getID()].set("Consume",100,100,0,&(UnitTypes::Zerg_Defiler_Mound),Races::Zerg,&(WeaponTypes::Consume),&(UnitTypes::Zerg_Defiler));
      techTypeData[Ensnare.getID()].set("Ensnare",100,100,75,&(UnitTypes::Zerg_Queens_Nest),Races::Zerg,&(WeaponTypes::Ensnare),&(UnitTypes::Zerg_Queen));
      techTypeData[Parasite.getID()].set("Parasite",0,0,75,&(UnitTypes::None),Races::Zerg,&(WeaponTypes::Parasite),&(UnitTypes::Zerg_Queen));
      techTypeData[Psionic_Storm.getID()].set("Psionic Storm",200,200,75,&(UnitTypes::Protoss_Templar_Archives),Races::Protoss,&(WeaponTypes::Psionic_Storm),&(UnitTypes::Protoss_High_Templar));
      techTypeData[Hallucination.getID()].set("Hallucination",150,150,100,&(UnitTypes::Protoss_Templar_Archives),Races::Protoss,&(WeaponTypes::None),&(UnitTypes::Protoss_High_Templar));
      techTypeData[Recall.getID()].set("Recall",150,150,150,&(UnitTypes::Protoss_Arbiter_Tribunal),Races::Protoss,&(WeaponTypes::None),&(UnitTypes::Protoss_Arbiter));
      techTypeData[Stasis_Field.getID()].set("Stasis Field",150,150,100,&(UnitTypes::Protoss_Arbiter_Tribunal),Races::Protoss,&(WeaponTypes::Stasis_Field),&(UnitTypes::Protoss_Arbiter));
      techTypeData[Archon_Warp.getID()].set("Archon Warp",0,0,0,&(UnitTypes::None),Races::Protoss,&(WeaponTypes::None),&(UnitTypes::Protoss_High_Templar));
      techTypeData[Restoration.getID()].set("Restoration",100,100,50,&(UnitTypes::Terran_Academy),Races::Terran,&(WeaponTypes::Restoration),&(UnitTypes::Terran_Medic));
      techTypeData[Disruption_Web.getID()].set("Disruption Web",200,200,125,&(UnitTypes::Protoss_Fleet_Beacon),Races::Protoss,&(WeaponTypes::Disruption_Web),&(UnitTypes::Protoss_Corsair));
      techTypeData[Mind_Control.getID()].set("Mind Control",200,200,150,&(UnitTypes::Protoss_Templar_Archives),Races::Protoss,&(WeaponTypes::Mind_Control),&(UnitTypes::Protoss_Dark_Archon));
      techTypeData[Dark_Archon_Meld.getID()].set("Dark Archon Meld",0,0,0,&(UnitTypes::None),Races::Protoss,&(WeaponTypes::None),&(UnitTypes::Protoss_Dark_Templar));
      techTypeData[Feedback.getID()].set("Feedback",0,0,50,&(UnitTypes::None),Races::Protoss,&(WeaponTypes::Feedback),&(UnitTypes::Protoss_Dark_Archon));
      techTypeData[Optical_Flare.getID()].set("Optical Flare",100,100,75,&(UnitTypes::Terran_Academy),Races::Terran,&(WeaponTypes::Optical_Flare),&(UnitTypes::Terran_Medic));
      techTypeData[Maelstrom.getID()].set("Maelstrom",100,100,100,&(UnitTypes::Protoss_Templar_Archives),Races::Protoss,&(WeaponTypes::Maelstrom),&(UnitTypes::Protoss_Dark_Archon));
      techTypeData[Lurker_Aspect.getID()].set("Lurker Aspect",200,200,0,&(UnitTypes::Zerg_Hydralisk_Den),Races::Zerg,&(WeaponTypes::None),&(UnitTypes::Zerg_Lurker));
      techTypeData[Healing.getID()].set("Healing",0,0,1,&(UnitTypes::None),Races::Terran,&(WeaponTypes::None),&(UnitTypes::Terran_Medic));
      techTypeData[None.getID()].set("None",0,0,0,&(UnitTypes::None),Races::None,&(WeaponTypes::None),&(UnitTypes::None));
      techTypeData[Unknown.getID()].set("Unknown",0,0,0,&(UnitTypes::None),Races::Unknown,&(WeaponTypes::None),&(UnitTypes::None));
      techTypeData[Nuclear_Strike.getID()].set("Nuclear Strike",0,0,0,&(UnitTypes::None),Races::Terran,&(WeaponTypes::Nuclear_Strike),&(UnitTypes::Terran_Ghost));
      techTypeSet.insert(Stim_Packs);
      techTypeSet.insert(Lockdown);
      techTypeSet.insert(EMP_Shockwave);
      techTypeSet.insert(Spider_Mines);
      techTypeSet.insert(Scanner_Sweep);
      techTypeSet.insert(Tank_Siege_Mode);
      techTypeSet.insert(Defensive_Matrix);
      techTypeSet.insert(Irradiate);
      techTypeSet.insert(Yamato_Gun);
      techTypeSet.insert(Cloaking_Field);
      techTypeSet.insert(Personnel_Cloaking);
      techTypeSet.insert(Burrowing);
      techTypeSet.insert(Infestation);
      techTypeSet.insert(Spawn_Broodlings);
      techTypeSet.insert(Dark_Swarm);
      techTypeSet.insert(Plague);
      techTypeSet.insert(Consume);
      techTypeSet.insert(Ensnare);
      techTypeSet.insert(Parasite);
      techTypeSet.insert(Psionic_Storm);
      techTypeSet.insert(Hallucination);
      techTypeSet.insert(Recall);
      techTypeSet.insert(Stasis_Field);
      techTypeSet.insert(Archon_Warp);
      techTypeSet.insert(Restoration);
      techTypeSet.insert(Disruption_Web);
      techTypeSet.insert(Mind_Control);
      techTypeSet.insert(Dark_Archon_Meld);
      techTypeSet.insert(Feedback);
      techTypeSet.insert(Optical_Flare);
      techTypeSet.insert(Maelstrom);
      techTypeSet.insert(Lurker_Aspect);
      techTypeSet.insert(Healing);
      techTypeSet.insert(None);
      techTypeSet.insert(Unknown);
      techTypeSet.insert(Nuclear_Strike);

      for(std::set<TechType>::iterator i = techTypeSet.begin(); i != techTypeSet.end(); i++)
      {
        techTypeMap.insert(std::make_pair((*i).getName(), *i));
      }
      initializingTechType = false;
    }
  }
  TechType::TechType()
  {
    this->id = TechTypes::None.id;
  }
  TechType::TechType(int id)
  {
    this->id = id;
    if (!initializingTechType)
    {
      if (id < 0 || id >= 47 || techTypeData[id].name.length() == 0)
      {
        this->id = TechTypes::Unknown.id;
      }
    }
  }
  TechType::TechType(const TechType& other)
  {
    this->id = other.id;
  }
  TechType& TechType::operator=(const TechType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool TechType::operator==(const TechType& other) const
  {
    return this->id == other.id;
  }
  bool TechType::operator!=(const TechType& other) const
  {
    return this->id != other.id;
  }
  bool TechType::operator<(const TechType& other) const
  {
    return this->id < other.id;
  }
  int TechType::getID() const
  {
    return this->id;
  }
  std::string TechType::getName() const
  {
    return techTypeData[this->id].name;
  }
  Race TechType::getRace() const
  {
    return techTypeData[this->id].race;
  }
  int TechType::mineralPrice() const
  {
    return techTypeData[this->id].mineralPrice;
  }
  int TechType::gasPrice() const
  {
    return techTypeData[this->id].gasPrice;
  }
  int TechType::energyUsed() const
  {
    return techTypeData[this->id].energyUsed;
  }
  const UnitType* TechType::whatResearches() const
  {
    return techTypeData[this->id].whatResearches;
  }
  const WeaponType* TechType::getWeapon() const
  {
    return techTypeData[this->id].weapon;
  }
  const std::set<const UnitType*>& TechType::whatUses() const
  {
    return techTypeData[this->id].whatUses;
  }
  TechType TechTypes::getTechType(std::string& name)
  {
    std::map<std::string, TechType>::iterator i = techTypeMap.find(name);
    if (i == techTypeMap.end()) return TechTypes::Unknown;
    return (*i).second;
  }
  std::set<TechType>& TechTypes::allTechTypes()
  {
    return techTypeSet;
  }
}


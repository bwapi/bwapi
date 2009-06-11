#include <string>
#include <map>
#include <set>
#include <BWAPI/TechType.h>
#include <BWAPI/Race.h>
#include <BW/UnitType.h> // Need to include this until a corresponding BWAPI::UnitType class is added to BWAPILIB

namespace BWAPI
{
  bool initializingTechType=true;
  struct TechTypeStruct
  {
    std::string name;
    int mineralPrice;
    int gasPrice;
    int energyPrice;
    BW::UnitType* whatResearches;
    Race race;
  };
  TechTypeStruct techTypeData[46];
  std::map<std::string, TechType> techTypeMap;
  std::set< TechType > techTypeSet;
  namespace TechTypes
  {
    const TechType Stim_Packs(0);
    const TechType Lockdown(1);
    const TechType EMP_Shockwave(2);
    const TechType Spider_Mines(3);
    const TechType Scanner_Sweep(4);
    const TechType Tank_Siege_Mode(5);
    const TechType Defensive_Matrix(6);
    const TechType Irradiate(7);
    const TechType Yamato_Gun(8);
    const TechType Cloaking_Field(9);
    const TechType Personnel_Cloaking(10);
    const TechType Burrowing(11);
    const TechType Infestation(12);
    const TechType Spawn_Broodling(13);
    const TechType Dark_Swarm(14);
    const TechType Plague(15);
    const TechType Consume(16);
    const TechType Ensnare(17);
    const TechType Parasite(18);
    const TechType Psionic_Storm(19);
    const TechType Hallucination(20);
    const TechType Recall(21);
    const TechType Stasis_Field(22);
    const TechType Archon_Warp(23);
    const TechType Restoration(24);
    const TechType Disruption_Web(25);
    const TechType Mind_Control(27);
    const TechType Dark_Archon_Meld(28);
    const TechType Feedback(29);
    const TechType Optical_Flare(30);
    const TechType Maelstrom(31);
    const TechType Lurker_Aspect(32);
    const TechType Healing(34);
    const TechType None(44);
    const TechType Unknown(45);
    void init()
    {
      techTypeData[Stim_Packs.getID()].name="Stim Packs";
      techTypeData[Stim_Packs.getID()].mineralPrice=100;
      techTypeData[Stim_Packs.getID()].gasPrice=100;
      techTypeData[Stim_Packs.getID()].energyPrice=0;
      techTypeData[Stim_Packs.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_Academy);
      techTypeData[Stim_Packs.getID()].race=Races::Terran;
      techTypeData[Lockdown.getID()].name="Lockdown";
      techTypeData[Lockdown.getID()].mineralPrice=200;
      techTypeData[Lockdown.getID()].gasPrice=200;
      techTypeData[Lockdown.getID()].energyPrice=100;
      techTypeData[Lockdown.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_CovertOps);
      techTypeData[Lockdown.getID()].race=Races::Terran;
      techTypeData[EMP_Shockwave.getID()].name="EMP Shockwave";
      techTypeData[EMP_Shockwave.getID()].mineralPrice=200;
      techTypeData[EMP_Shockwave.getID()].gasPrice=200;
      techTypeData[EMP_Shockwave.getID()].energyPrice=100;
      techTypeData[EMP_Shockwave.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_ScienceFacility);
      techTypeData[EMP_Shockwave.getID()].race=Races::Terran;
      techTypeData[Spider_Mines.getID()].name="Spider Mines";
      techTypeData[Spider_Mines.getID()].mineralPrice=100;
      techTypeData[Spider_Mines.getID()].gasPrice=100;
      techTypeData[Spider_Mines.getID()].energyPrice=0;
      techTypeData[Spider_Mines.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_MachineShop);
      techTypeData[Spider_Mines.getID()].race=Races::Terran;
      techTypeData[Scanner_Sweep.getID()].name="Scanner Sweep";
      techTypeData[Scanner_Sweep.getID()].mineralPrice=0;
      techTypeData[Scanner_Sweep.getID()].gasPrice=0;
      techTypeData[Scanner_Sweep.getID()].energyPrice=50;
      techTypeData[Scanner_Sweep.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Scanner_Sweep.getID()].race=Races::Terran;
      techTypeData[Tank_Siege_Mode.getID()].name="Tank Siege Mode";
      techTypeData[Tank_Siege_Mode.getID()].mineralPrice=150;
      techTypeData[Tank_Siege_Mode.getID()].gasPrice=150;
      techTypeData[Tank_Siege_Mode.getID()].energyPrice=0;
      techTypeData[Tank_Siege_Mode.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_MachineShop);
      techTypeData[Tank_Siege_Mode.getID()].race=Races::Terran;
      techTypeData[Defensive_Matrix.getID()].name="Defensive Matrix";
      techTypeData[Defensive_Matrix.getID()].mineralPrice=150;
      techTypeData[Defensive_Matrix.getID()].gasPrice=150;
      techTypeData[Defensive_Matrix.getID()].energyPrice=100;
      techTypeData[Defensive_Matrix.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Defensive_Matrix.getID()].race=Races::Terran;
      techTypeData[Irradiate.getID()].name="Irradiate";
      techTypeData[Irradiate.getID()].mineralPrice=200;
      techTypeData[Irradiate.getID()].gasPrice=200;
      techTypeData[Irradiate.getID()].energyPrice=75;
      techTypeData[Irradiate.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_ScienceFacility);
      techTypeData[Irradiate.getID()].race=Races::Terran;
      techTypeData[Yamato_Gun.getID()].name="Yamato Gun";
      techTypeData[Yamato_Gun.getID()].mineralPrice=100;
      techTypeData[Yamato_Gun.getID()].gasPrice=100;
      techTypeData[Yamato_Gun.getID()].energyPrice=150;
      techTypeData[Yamato_Gun.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_PhysicsLab);
      techTypeData[Yamato_Gun.getID()].race=Races::Terran;
      techTypeData[Cloaking_Field.getID()].name="Cloaking Field";
      techTypeData[Cloaking_Field.getID()].mineralPrice=150;
      techTypeData[Cloaking_Field.getID()].gasPrice=150;
      techTypeData[Cloaking_Field.getID()].energyPrice=25;
      techTypeData[Cloaking_Field.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_ControlTower);
      techTypeData[Cloaking_Field.getID()].race=Races::Terran;
      techTypeData[Personnel_Cloaking.getID()].name="Personnel Cloaking";
      techTypeData[Personnel_Cloaking.getID()].mineralPrice=100;
      techTypeData[Personnel_Cloaking.getID()].gasPrice=100;
      techTypeData[Personnel_Cloaking.getID()].energyPrice=25;
      techTypeData[Personnel_Cloaking.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_CovertOps);
      techTypeData[Personnel_Cloaking.getID()].race=Races::Terran;
      techTypeData[Burrowing.getID()].name="Burrowing";
      techTypeData[Burrowing.getID()].mineralPrice=100;
      techTypeData[Burrowing.getID()].gasPrice=100;
      techTypeData[Burrowing.getID()].energyPrice=0;
      techTypeData[Burrowing.getID()].whatResearches=new BW::UnitType(BW::UnitID::Zerg_Hatchery);
      techTypeData[Burrowing.getID()].race=Races::Zerg;
      techTypeData[Infestation.getID()].name="Infestation";
      techTypeData[Infestation.getID()].mineralPrice=100;
      techTypeData[Infestation.getID()].gasPrice=100;
      techTypeData[Infestation.getID()].energyPrice=0;
      techTypeData[Infestation.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Infestation.getID()].race=Races::Zerg;
      techTypeData[Spawn_Broodling.getID()].name="Spawn Broodling";
      techTypeData[Spawn_Broodling.getID()].mineralPrice=100;
      techTypeData[Spawn_Broodling.getID()].gasPrice=100;
      techTypeData[Spawn_Broodling.getID()].energyPrice=150;
      techTypeData[Spawn_Broodling.getID()].whatResearches=new BW::UnitType(BW::UnitID::Zerg_QueensNest);
      techTypeData[Spawn_Broodling.getID()].race=Races::Zerg;
      techTypeData[Dark_Swarm.getID()].name="Dark Swarm";
      techTypeData[Dark_Swarm.getID()].mineralPrice=100;
      techTypeData[Dark_Swarm.getID()].gasPrice=100;
      techTypeData[Dark_Swarm.getID()].energyPrice=100;
      techTypeData[Dark_Swarm.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Dark_Swarm.getID()].race=Races::Zerg;
      techTypeData[Plague.getID()].name="Plague";
      techTypeData[Plague.getID()].mineralPrice=200;
      techTypeData[Plague.getID()].gasPrice=200;
      techTypeData[Plague.getID()].energyPrice=150;
      techTypeData[Plague.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Plague.getID()].race=Races::Zerg;
      techTypeData[Consume.getID()].name="Consume";
      techTypeData[Consume.getID()].mineralPrice=100;
      techTypeData[Consume.getID()].gasPrice=100;
      techTypeData[Consume.getID()].energyPrice=0;
      techTypeData[Consume.getID()].whatResearches=new BW::UnitType(BW::UnitID::Zerg_DefilerMound);
      techTypeData[Consume.getID()].race=Races::Zerg;
      techTypeData[Ensnare.getID()].name="Ensnare";
      techTypeData[Ensnare.getID()].mineralPrice=100;
      techTypeData[Ensnare.getID()].gasPrice=100;
      techTypeData[Ensnare.getID()].energyPrice=75;
      techTypeData[Ensnare.getID()].whatResearches=new BW::UnitType(BW::UnitID::Zerg_QueensNest);
      techTypeData[Ensnare.getID()].race=Races::Zerg;
      techTypeData[Parasite.getID()].name="Parasite";
      techTypeData[Parasite.getID()].mineralPrice=100;
      techTypeData[Parasite.getID()].gasPrice=100;
      techTypeData[Parasite.getID()].energyPrice=75;
      techTypeData[Parasite.getID()].whatResearches=new BW::UnitType(BW::UnitID::Zerg_QueensNest);
      techTypeData[Parasite.getID()].race=Races::Zerg;
      techTypeData[Psionic_Storm.getID()].name="Psionic Storm";
      techTypeData[Psionic_Storm.getID()].mineralPrice=200;
      techTypeData[Psionic_Storm.getID()].gasPrice=200;
      techTypeData[Psionic_Storm.getID()].energyPrice=75;
      techTypeData[Psionic_Storm.getID()].whatResearches=new BW::UnitType(BW::UnitID::Protoss_TemplarArchives);
      techTypeData[Psionic_Storm.getID()].race=Races::Protoss;
      techTypeData[Hallucination.getID()].name="Hallucination";
      techTypeData[Hallucination.getID()].mineralPrice=150;
      techTypeData[Hallucination.getID()].gasPrice=150;
      techTypeData[Hallucination.getID()].energyPrice=100;
      techTypeData[Hallucination.getID()].whatResearches=new BW::UnitType(BW::UnitID::Protoss_TemplarArchives);
      techTypeData[Hallucination.getID()].race=Races::Protoss;
      techTypeData[Recall.getID()].name="Recall";
      techTypeData[Recall.getID()].mineralPrice=150;
      techTypeData[Recall.getID()].gasPrice=150;
      techTypeData[Recall.getID()].energyPrice=150;
      techTypeData[Recall.getID()].whatResearches=new BW::UnitType(BW::UnitID::Protoss_ArbiterTribunal);
      techTypeData[Recall.getID()].race=Races::Protoss;
      techTypeData[Stasis_Field.getID()].name="Stasis Field";
      techTypeData[Stasis_Field.getID()].mineralPrice=150;
      techTypeData[Stasis_Field.getID()].gasPrice=150;
      techTypeData[Stasis_Field.getID()].energyPrice=100;
      techTypeData[Stasis_Field.getID()].whatResearches=new BW::UnitType(BW::UnitID::Protoss_ArbiterTribunal);
      techTypeData[Stasis_Field.getID()].race=Races::Protoss;
      techTypeData[Archon_Warp.getID()].name="Archon Warp";
      techTypeData[Archon_Warp.getID()].mineralPrice=150;
      techTypeData[Archon_Warp.getID()].gasPrice=150;
      techTypeData[Archon_Warp.getID()].energyPrice=0;
      techTypeData[Archon_Warp.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Archon_Warp.getID()].race=Races::Protoss;
      techTypeData[Restoration.getID()].name="Restoration";
      techTypeData[Restoration.getID()].mineralPrice=100;
      techTypeData[Restoration.getID()].gasPrice=100;
      techTypeData[Restoration.getID()].energyPrice=50;
      techTypeData[Restoration.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Restoration.getID()].race=Races::Protoss;
      techTypeData[Disruption_Web.getID()].name="Disruption Web";
      techTypeData[Disruption_Web.getID()].mineralPrice=200;
      techTypeData[Disruption_Web.getID()].gasPrice=200;
      techTypeData[Disruption_Web.getID()].energyPrice=125;
      techTypeData[Disruption_Web.getID()].whatResearches=new BW::UnitType(BW::UnitID::Protoss_FleetBeacon);
      techTypeData[Disruption_Web.getID()].race=Races::Protoss;
      techTypeData[Mind_Control.getID()].name="Mind Control";
      techTypeData[Mind_Control.getID()].mineralPrice=200;
      techTypeData[Mind_Control.getID()].gasPrice=200;
      techTypeData[Mind_Control.getID()].energyPrice=150;
      techTypeData[Mind_Control.getID()].whatResearches=new BW::UnitType(BW::UnitID::Protoss_TemplarArchives);
      techTypeData[Mind_Control.getID()].race=Races::Protoss;
      techTypeData[Dark_Archon_Meld.getID()].name="Dark Archon Meld";
      techTypeData[Dark_Archon_Meld.getID()].mineralPrice=200;
      techTypeData[Dark_Archon_Meld.getID()].gasPrice=200;
      techTypeData[Dark_Archon_Meld.getID()].energyPrice=100;
      techTypeData[Dark_Archon_Meld.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Dark_Archon_Meld.getID()].race=Races::Protoss;
      techTypeData[Feedback.getID()].name="Feedback";
      techTypeData[Feedback.getID()].mineralPrice=100;
      techTypeData[Feedback.getID()].gasPrice=100;
      techTypeData[Feedback.getID()].energyPrice=50;
      techTypeData[Feedback.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Feedback.getID()].race=Races::Protoss;
      techTypeData[Optical_Flare.getID()].name="Optical Flare";
      techTypeData[Optical_Flare.getID()].mineralPrice=100;
      techTypeData[Optical_Flare.getID()].gasPrice=100;
      techTypeData[Optical_Flare.getID()].energyPrice=75;
      techTypeData[Optical_Flare.getID()].whatResearches=new BW::UnitType(BW::UnitID::Terran_Academy);
      techTypeData[Optical_Flare.getID()].race=Races::Protoss;
      techTypeData[Maelstrom.getID()].name="Maelstrom";
      techTypeData[Maelstrom.getID()].mineralPrice=100;
      techTypeData[Maelstrom.getID()].gasPrice=100;
      techTypeData[Maelstrom.getID()].energyPrice=100;
      techTypeData[Maelstrom.getID()].whatResearches=new BW::UnitType(BW::UnitID::Protoss_TemplarArchives);
      techTypeData[Maelstrom.getID()].race=Races::Protoss;
      techTypeData[Lurker_Aspect.getID()].name="Lurker Aspect";
      techTypeData[Lurker_Aspect.getID()].mineralPrice=200;
      techTypeData[Lurker_Aspect.getID()].gasPrice=200;
      techTypeData[Lurker_Aspect.getID()].energyPrice=100;
      techTypeData[Lurker_Aspect.getID()].whatResearches=new BW::UnitType(BW::UnitID::Zerg_HydraliskDen);
      techTypeData[Lurker_Aspect.getID()].race=Races::Zerg;
      techTypeData[Healing.getID()].name="Healing";
      techTypeData[Healing.getID()].mineralPrice=0;
      techTypeData[Healing.getID()].gasPrice=0;
      techTypeData[Healing.getID()].energyPrice=1;
      techTypeData[Healing.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Healing.getID()].race=Races::Terran;
      techTypeData[None.getID()].name="None";
      techTypeData[None.getID()].mineralPrice=0;
      techTypeData[None.getID()].gasPrice=0;
      techTypeData[None.getID()].energyPrice=0;
      techTypeData[None.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[None.getID()].race=Races::None;
      techTypeData[Unknown.getID()].name="Unknown";
      techTypeData[Unknown.getID()].mineralPrice=0;
      techTypeData[Unknown.getID()].gasPrice=0;
      techTypeData[Unknown.getID()].energyPrice=0;
      techTypeData[Unknown.getID()].whatResearches=new BW::UnitType(BW::UnitID::None);
      techTypeData[Unknown.getID()].race=Races::Unknown;
      techTypeSet.insert(TechTypes::Stim_Packs);
      techTypeSet.insert(TechTypes::Lockdown);
      techTypeSet.insert(TechTypes::EMP_Shockwave);
      techTypeSet.insert(TechTypes::Spider_Mines);
      techTypeSet.insert(TechTypes::Scanner_Sweep);
      techTypeSet.insert(TechTypes::Tank_Siege_Mode);
      techTypeSet.insert(TechTypes::Defensive_Matrix);
      techTypeSet.insert(TechTypes::Irradiate);
      techTypeSet.insert(TechTypes::Yamato_Gun);
      techTypeSet.insert(TechTypes::Cloaking_Field);
      techTypeSet.insert(TechTypes::Personnel_Cloaking);
      techTypeSet.insert(TechTypes::Burrowing);
      techTypeSet.insert(TechTypes::Infestation);
      techTypeSet.insert(TechTypes::Spawn_Broodling);
      techTypeSet.insert(TechTypes::Dark_Swarm);
      techTypeSet.insert(TechTypes::Plague);
      techTypeSet.insert(TechTypes::Consume);
      techTypeSet.insert(TechTypes::Ensnare);
      techTypeSet.insert(TechTypes::Parasite);
      techTypeSet.insert(TechTypes::Psionic_Storm);
      techTypeSet.insert(TechTypes::Hallucination);
      techTypeSet.insert(TechTypes::Recall);
      techTypeSet.insert(TechTypes::Stasis_Field);
      techTypeSet.insert(TechTypes::Archon_Warp);
      techTypeSet.insert(TechTypes::Restoration);
      techTypeSet.insert(TechTypes::Disruption_Web);
      techTypeSet.insert(TechTypes::Mind_Control);
      techTypeSet.insert(TechTypes::Dark_Archon_Meld);
      techTypeSet.insert(TechTypes::Feedback);
      techTypeSet.insert(TechTypes::Optical_Flare);
      techTypeSet.insert(TechTypes::Maelstrom);
      techTypeSet.insert(TechTypes::Lurker_Aspect);
      techTypeSet.insert(TechTypes::Healing);
      techTypeSet.insert(TechTypes::None);
      techTypeSet.insert(TechTypes::Unknown);

      for(std::set<TechType>::iterator i=techTypeSet.begin();i!=techTypeSet.end();i++)
      {
        techTypeMap.insert(std::make_pair((*i).getName(),*i));
      }
      initializingTechType=false;
    }
  }
  TechType::TechType()
  {
    this->id=TechTypes::None.id;
  }
  TechType::TechType(int id)
  {
    this->id=id;
    if (!initializingTechType)
    {
      if (id<0 || id>=46 || techTypeData[id].name.length()==0)
      {
        this->id=TechTypes::Unknown.id;
      }
    }
  }
  TechType::TechType(const TechType &other)
  {
    this->id=other.id;
  }
  TechType& TechType::operator=(const TechType &other)
  {
    this->id=other.id;
    return *this;
  }
  bool TechType::operator==(const TechType &other) const
  {
    return this->id==other.id;
  }
  bool TechType::operator!=(const TechType &other) const
  {
    return this->id!=other.id;
  }
  bool TechType::operator<(const TechType &other) const
  {
    return this->id<other.id;
  }
  int TechType::getID() const
  {
    return this->id;
  }
  std::string TechType::getName() const
  {
    return techTypeData[this->id].name;
  }
  int TechType::mineralPrice() const
  {
    return techTypeData[this->id].mineralPrice;
  }
  int TechType::gasPrice() const
  {
    return techTypeData[this->id].gasPrice;
  }
  int TechType::energyPrice() const
  {
    return techTypeData[this->id].energyPrice;
  }
  BW::UnitType* TechType::whatResearches() const
  {
    return techTypeData[this->id].whatResearches;
  }
  TechType TechTypes::getTechType(std::string &name)
  {
    std::map<std::string, TechType>::iterator i=techTypeMap.find(name);
    if (i==techTypeMap.end()) return TechTypes::Unknown;
    return (*i).second;
  }
  std::set<TechType>& TechTypes::allTechTypes()
  {
    return techTypeSet;
  }
}


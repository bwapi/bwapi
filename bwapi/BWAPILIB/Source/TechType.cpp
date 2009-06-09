#include <string>
#include <map>
#include <set>
#include <BWAPI/TechType.h>

namespace BWAPI
{
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
  }
  struct TechTypeStruct
  {
    std::string name;
    int mineralPrice;
    int gasPrice;
    int energyPrice;
  };
  TechTypeStruct techTypeData[46];
  std::map<std::string, TechType> techTypeMap;
  std::set< TechType > techTypeSet;
  int techTypeInit()
  {
    techTypeData[TechTypes::Stim_Packs.getID()].name="Stim Packs";
    techTypeData[TechTypes::Stim_Packs.getID()].mineralPrice=100;
    techTypeData[TechTypes::Stim_Packs.getID()].gasPrice=100;
    techTypeData[TechTypes::Stim_Packs.getID()].energyPrice=0;
    techTypeData[TechTypes::Lockdown.getID()].name="Lockdown";
    techTypeData[TechTypes::Lockdown.getID()].mineralPrice=200;
    techTypeData[TechTypes::Lockdown.getID()].gasPrice=200;
    techTypeData[TechTypes::Lockdown.getID()].energyPrice=100;
    techTypeData[TechTypes::EMP_Shockwave.getID()].name="EMP Shockwave";
    techTypeData[TechTypes::EMP_Shockwave.getID()].mineralPrice=200;
    techTypeData[TechTypes::EMP_Shockwave.getID()].gasPrice=200;
    techTypeData[TechTypes::EMP_Shockwave.getID()].energyPrice=100;
    techTypeData[TechTypes::Spider_Mines.getID()].name="Spider Mines";
    techTypeData[TechTypes::Spider_Mines.getID()].mineralPrice=100;
    techTypeData[TechTypes::Spider_Mines.getID()].gasPrice=100;
    techTypeData[TechTypes::Spider_Mines.getID()].energyPrice=0;
    techTypeData[TechTypes::Scanner_Sweep.getID()].name="Scanner Sweep";
    techTypeData[TechTypes::Scanner_Sweep.getID()].mineralPrice=0;
    techTypeData[TechTypes::Scanner_Sweep.getID()].gasPrice=0;
    techTypeData[TechTypes::Scanner_Sweep.getID()].energyPrice=50;
    techTypeData[TechTypes::Tank_Siege_Mode.getID()].name="Tank Siege Mode";
    techTypeData[TechTypes::Tank_Siege_Mode.getID()].mineralPrice=150;
    techTypeData[TechTypes::Tank_Siege_Mode.getID()].gasPrice=150;
    techTypeData[TechTypes::Tank_Siege_Mode.getID()].energyPrice=0;
    techTypeData[TechTypes::Defensive_Matrix.getID()].name="Defensive Matrix";
    techTypeData[TechTypes::Defensive_Matrix.getID()].mineralPrice=150;
    techTypeData[TechTypes::Defensive_Matrix.getID()].gasPrice=150;
    techTypeData[TechTypes::Defensive_Matrix.getID()].energyPrice=100;
    techTypeData[TechTypes::Irradiate.getID()].name="Irradiate";
    techTypeData[TechTypes::Irradiate.getID()].mineralPrice=200;
    techTypeData[TechTypes::Irradiate.getID()].gasPrice=200;
    techTypeData[TechTypes::Irradiate.getID()].energyPrice=75;
    techTypeData[TechTypes::Yamato_Gun.getID()].name="Yamato Gun";
    techTypeData[TechTypes::Yamato_Gun.getID()].mineralPrice=100;
    techTypeData[TechTypes::Yamato_Gun.getID()].gasPrice=100;
    techTypeData[TechTypes::Yamato_Gun.getID()].energyPrice=150;
    techTypeData[TechTypes::Cloaking_Field.getID()].name="Cloaking Field";
    techTypeData[TechTypes::Cloaking_Field.getID()].mineralPrice=150;
    techTypeData[TechTypes::Cloaking_Field.getID()].gasPrice=150;
    techTypeData[TechTypes::Cloaking_Field.getID()].energyPrice=25;
    techTypeData[TechTypes::Personnel_Cloaking.getID()].name="Personnel Cloaking";
    techTypeData[TechTypes::Personnel_Cloaking.getID()].mineralPrice=100;
    techTypeData[TechTypes::Personnel_Cloaking.getID()].gasPrice=100;
    techTypeData[TechTypes::Personnel_Cloaking.getID()].energyPrice=25;
    techTypeData[TechTypes::Burrowing.getID()].name="Burrowing";
    techTypeData[TechTypes::Burrowing.getID()].mineralPrice=100;
    techTypeData[TechTypes::Burrowing.getID()].gasPrice=100;
    techTypeData[TechTypes::Burrowing.getID()].energyPrice=0;
    techTypeData[TechTypes::Infestation.getID()].name="Infestation";
    techTypeData[TechTypes::Infestation.getID()].mineralPrice=100;
    techTypeData[TechTypes::Infestation.getID()].gasPrice=100;
    techTypeData[TechTypes::Infestation.getID()].energyPrice=0;
    techTypeData[TechTypes::Spawn_Broodling.getID()].name="Spawn Broodling";
    techTypeData[TechTypes::Spawn_Broodling.getID()].mineralPrice=100;
    techTypeData[TechTypes::Spawn_Broodling.getID()].gasPrice=100;
    techTypeData[TechTypes::Spawn_Broodling.getID()].energyPrice=150;
    techTypeData[TechTypes::Dark_Swarm.getID()].name="Dark Swarm";
    techTypeData[TechTypes::Dark_Swarm.getID()].mineralPrice=100;
    techTypeData[TechTypes::Dark_Swarm.getID()].gasPrice=100;
    techTypeData[TechTypes::Dark_Swarm.getID()].energyPrice=100;
    techTypeData[TechTypes::Plague.getID()].name="Plague";
    techTypeData[TechTypes::Plague.getID()].mineralPrice=200;
    techTypeData[TechTypes::Plague.getID()].gasPrice=200;
    techTypeData[TechTypes::Plague.getID()].energyPrice=150;
    techTypeData[TechTypes::Consume.getID()].name="Consume";
    techTypeData[TechTypes::Consume.getID()].mineralPrice=100;
    techTypeData[TechTypes::Consume.getID()].gasPrice=100;
    techTypeData[TechTypes::Consume.getID()].energyPrice=0;
    techTypeData[TechTypes::Ensnare.getID()].name="Ensnare";
    techTypeData[TechTypes::Ensnare.getID()].mineralPrice=100;
    techTypeData[TechTypes::Ensnare.getID()].gasPrice=100;
    techTypeData[TechTypes::Ensnare.getID()].energyPrice=75;
    techTypeData[TechTypes::Parasite.getID()].name="Parasite";
    techTypeData[TechTypes::Parasite.getID()].mineralPrice=100;
    techTypeData[TechTypes::Parasite.getID()].gasPrice=100;
    techTypeData[TechTypes::Parasite.getID()].energyPrice=75;
    techTypeData[TechTypes::Psionic_Storm.getID()].name="Psionic Storm";
    techTypeData[TechTypes::Psionic_Storm.getID()].mineralPrice=200;
    techTypeData[TechTypes::Psionic_Storm.getID()].gasPrice=200;
    techTypeData[TechTypes::Psionic_Storm.getID()].energyPrice=75;
    techTypeData[TechTypes::Hallucination.getID()].name="Hallucination";
    techTypeData[TechTypes::Hallucination.getID()].mineralPrice=150;
    techTypeData[TechTypes::Hallucination.getID()].gasPrice=150;
    techTypeData[TechTypes::Hallucination.getID()].energyPrice=100;
    techTypeData[TechTypes::Recall.getID()].name="Recall";
    techTypeData[TechTypes::Recall.getID()].mineralPrice=150;
    techTypeData[TechTypes::Recall.getID()].gasPrice=150;
    techTypeData[TechTypes::Recall.getID()].energyPrice=150;
    techTypeData[TechTypes::Stasis_Field.getID()].name="Stasis Field";
    techTypeData[TechTypes::Stasis_Field.getID()].mineralPrice=150;
    techTypeData[TechTypes::Stasis_Field.getID()].gasPrice=150;
    techTypeData[TechTypes::Stasis_Field.getID()].energyPrice=100;
    techTypeData[TechTypes::Archon_Warp.getID()].name="Archon Warp";
    techTypeData[TechTypes::Archon_Warp.getID()].mineralPrice=150;
    techTypeData[TechTypes::Archon_Warp.getID()].gasPrice=150;
    techTypeData[TechTypes::Archon_Warp.getID()].energyPrice=0;
    techTypeData[TechTypes::Restoration.getID()].name="Restoration";
    techTypeData[TechTypes::Restoration.getID()].mineralPrice=100;
    techTypeData[TechTypes::Restoration.getID()].gasPrice=100;
    techTypeData[TechTypes::Restoration.getID()].energyPrice=50;
    techTypeData[TechTypes::Disruption_Web.getID()].name="Disruption Web";
    techTypeData[TechTypes::Disruption_Web.getID()].mineralPrice=200;
    techTypeData[TechTypes::Disruption_Web.getID()].gasPrice=200;
    techTypeData[TechTypes::Disruption_Web.getID()].energyPrice=125;
    techTypeData[TechTypes::Mind_Control.getID()].name="Mind Control";
    techTypeData[TechTypes::Mind_Control.getID()].mineralPrice=200;
    techTypeData[TechTypes::Mind_Control.getID()].gasPrice=200;
    techTypeData[TechTypes::Mind_Control.getID()].energyPrice=150;
    techTypeData[TechTypes::Dark_Archon_Meld.getID()].name="Dark Archon Meld";
    techTypeData[TechTypes::Dark_Archon_Meld.getID()].mineralPrice=200;
    techTypeData[TechTypes::Dark_Archon_Meld.getID()].gasPrice=200;
    techTypeData[TechTypes::Dark_Archon_Meld.getID()].energyPrice=100;
    techTypeData[TechTypes::Feedback.getID()].name="Feedback";
    techTypeData[TechTypes::Feedback.getID()].mineralPrice=100;
    techTypeData[TechTypes::Feedback.getID()].gasPrice=100;
    techTypeData[TechTypes::Feedback.getID()].energyPrice=50;
    techTypeData[TechTypes::Optical_Flare.getID()].name="Optical Flare";
    techTypeData[TechTypes::Optical_Flare.getID()].mineralPrice=100;
    techTypeData[TechTypes::Optical_Flare.getID()].gasPrice=100;
    techTypeData[TechTypes::Optical_Flare.getID()].energyPrice=75;
    techTypeData[TechTypes::Maelstrom.getID()].name="Maelstrom";
    techTypeData[TechTypes::Maelstrom.getID()].mineralPrice=100;
    techTypeData[TechTypes::Maelstrom.getID()].gasPrice=100;
    techTypeData[TechTypes::Maelstrom.getID()].energyPrice=100;
    techTypeData[TechTypes::Lurker_Aspect.getID()].name="Lurker Aspect";
    techTypeData[TechTypes::Lurker_Aspect.getID()].mineralPrice=200;
    techTypeData[TechTypes::Lurker_Aspect.getID()].gasPrice=200;
    techTypeData[TechTypes::Lurker_Aspect.getID()].energyPrice=100;
    techTypeData[TechTypes::Healing.getID()].name="Healing";
    techTypeData[TechTypes::Healing.getID()].mineralPrice=0;
    techTypeData[TechTypes::Healing.getID()].gasPrice=0;
    techTypeData[TechTypes::Healing.getID()].energyPrice=1;
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
  }
  TechType::TechType()
  {
    this->id=TechTypes::None.id;
  }
  TechType::TechType(int id)
  {
    this->id=id;
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
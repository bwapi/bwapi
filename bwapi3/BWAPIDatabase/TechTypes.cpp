#include "TechTypes.h"

#include <BWAPITypes/TechType.h>
#include <BWAPITypes/TechTypeId.h>
#include <BWAPITypes/RaceId.h>
#include <BWAPITypes/UnitTypeId.h>
#include <BWAPITypes/WeaponTypeId.h>

namespace BWAPI
{
  namespace TechTypes
  {

    TechTypeInternal techTypeData[TechTypeIds::count];
    std::map<std::string, TechTypeId> techTypeMap;
    std::set< TechTypeId > techTypeSet;

    void fillTechType(TechTypeId id, const char* name, int mineralPrice, int gasPrice, int energyUsed, int/*UnitTypeId*/ whatResearches, RaceId race, int/*WeaponTypeId*/ weapon, int/*UnitTypeId*/ whatUses1, int /*UnitTypeId*/ whatUses2=NULL, int /*UnitTypeId*/ whatUses3=NULL, int /*UnitType*/ whatUses4=NULL)
    {
      TechTypeInternal &target = techTypeData[id];
      target.name=name;
      target.mineralPrice=mineralPrice;
      target.gasPrice=gasPrice;
      target.energyUsed=energyUsed;
      target.whatResearches=(UnitTypeId)whatResearches;
      target.race=(RaceId)race;
      target.weapon=(WeaponTypeId)weapon;
      if (whatUses1!=NULL && whatUses1 != UnitTypeIds::None)
      {
        target.whatUses.insert((UnitTypeId)whatUses1);
      }
      if (whatUses2!=NULL && whatUses2!= UnitTypeIds::None)
      {
        target.whatUses.insert((UnitTypeId)whatUses2);
      }
      if (whatUses3!=NULL && whatUses3!= UnitTypeIds::None)
      {
        target.whatUses.insert((UnitTypeId)whatUses3);
      }
      if (whatUses4!=NULL && whatUses4!= UnitTypeIds::None)
      {
        target.whatUses.insert((UnitTypeId)whatUses4);
      }
      target.valid = true;
    }

    void init()
    {
      fillTechType(TechTypeIds::Stim_Packs, "Stim Packs",100,100,0,UnitTypeIds::Terran_Academy,RaceIds::Terran,WeaponTypeIds::None,UnitTypeIds::Terran_Marine,UnitTypeIds::Terran_Firebat);
      fillTechType(TechTypeIds::Lockdown, "Lockdown",200,200,100,UnitTypeIds::Terran_Covert_Ops,RaceIds::Terran,WeaponTypeIds::Lockdown,UnitTypeIds::Terran_Ghost);
      fillTechType(TechTypeIds::EMP_Shockwave, "EMP Shockwave",200,200,100,UnitTypeIds::Terran_Science_Facility,RaceIds::Terran,WeaponTypeIds::EMP_Shockwave,UnitTypeIds::Terran_Science_Vessel);
      fillTechType(TechTypeIds::Spider_Mines, "Spider Mines",100,100,0,UnitTypeIds::Terran_Machine_Shop,RaceIds::Terran,WeaponTypeIds::Spider_Mines,UnitTypeIds::Terran_Vulture);
      fillTechType(TechTypeIds::Scanner_Sweep, "Scanner Sweep",0,0,50,UnitTypeIds::None,RaceIds::Terran,WeaponTypeIds::None,UnitTypeIds::Terran_Comsat_Station);
      fillTechType(TechTypeIds::Tank_Siege_Mode, "Tank Siege Mode",150,150,0,UnitTypeIds::Terran_Machine_Shop,RaceIds::Terran,WeaponTypeIds::None,UnitTypeIds::Terran_Siege_Tank_Tank_Mode,UnitTypeIds::Terran_Siege_Tank_Siege_Mode);
      fillTechType(TechTypeIds::Defensive_Matrix, "Defensive Matrix",0,0,100,UnitTypeIds::None,RaceIds::Terran,WeaponTypeIds::None,UnitTypeIds::Terran_Science_Vessel);
      fillTechType(TechTypeIds::Irradiate, "Irradiate",200,200,75,UnitTypeIds::Terran_Science_Facility,RaceIds::Terran,WeaponTypeIds::Irradiate,UnitTypeIds::Terran_Science_Vessel);
      fillTechType(TechTypeIds::Yamato_Gun, "Yamato Gun",100,100,150,UnitTypeIds::Terran_Physics_Lab,RaceIds::Terran,WeaponTypeIds::Yamato_Gun,UnitTypeIds::Terran_Battlecruiser);
      fillTechType(TechTypeIds::Cloaking_Field, "Cloaking Field",150,150,25,UnitTypeIds::Terran_Control_Tower,RaceIds::Terran,WeaponTypeIds::None,UnitTypeIds::Terran_Wraith);
      fillTechType(TechTypeIds::Personnel_Cloaking, "Personnel Cloaking",100,100,25,UnitTypeIds::Terran_Covert_Ops,RaceIds::Terran,WeaponTypeIds::None,UnitTypeIds::Terran_Ghost);
      fillTechType(TechTypeIds::Burrowing, "Burrowing",100,100,0,UnitTypeIds::Zerg_Hatchery,RaceIds::Zerg,WeaponTypeIds::None,UnitTypeIds::Zerg_Drone,UnitTypeIds::Zerg_Zergling,UnitTypeIds::Zerg_Hydralisk,UnitTypeIds::Zerg_Defiler);
      fillTechType(TechTypeIds::Infestation, "Infestation",0,0,0,UnitTypeIds::None,RaceIds::Zerg,WeaponTypeIds::None,UnitTypeIds::Zerg_Queen);
      fillTechType(TechTypeIds::Spawn_Broodlings, "Spawn Broodlings",100,100,150,UnitTypeIds::Zerg_Queens_Nest,RaceIds::Zerg,WeaponTypeIds::Spawn_Broodlings,UnitTypeIds::Zerg_Queen);
      fillTechType(TechTypeIds::Dark_Swarm, "Dark Swarm",0,0,100,UnitTypeIds::None,RaceIds::Zerg,WeaponTypeIds::Dark_Swarm,UnitTypeIds::Zerg_Defiler);
      fillTechType(TechTypeIds::Plague, "Plague",200,200,150,UnitTypeIds::Zerg_Defiler_Mound,RaceIds::Zerg,WeaponTypeIds::Plague,UnitTypeIds::Zerg_Defiler);
      fillTechType(TechTypeIds::Consume, "Consume",100,100,0,UnitTypeIds::Zerg_Defiler_Mound,RaceIds::Zerg,WeaponTypeIds::Consume,UnitTypeIds::Zerg_Defiler);
      fillTechType(TechTypeIds::Ensnare, "Ensnare",100,100,75,UnitTypeIds::Zerg_Queens_Nest,RaceIds::Zerg,WeaponTypeIds::Ensnare,UnitTypeIds::Zerg_Queen);
      fillTechType(TechTypeIds::Parasite, "Parasite",0,0,75,UnitTypeIds::None,RaceIds::Zerg,WeaponTypeIds::Parasite,UnitTypeIds::Zerg_Queen);
      fillTechType(TechTypeIds::Psionic_Storm, "Psionic Storm",200,200,75,UnitTypeIds::Protoss_Templar_Archives,RaceIds::Protoss,WeaponTypeIds::Psionic_Storm,UnitTypeIds::Protoss_High_Templar);
      fillTechType(TechTypeIds::Hallucination, "Hallucination",150,150,100,UnitTypeIds::Protoss_Templar_Archives,RaceIds::Protoss,WeaponTypeIds::None,UnitTypeIds::Protoss_High_Templar);
      fillTechType(TechTypeIds::Recall, "Recall",150,150,150,UnitTypeIds::Protoss_Arbiter_Tribunal,RaceIds::Protoss,WeaponTypeIds::None,UnitTypeIds::Protoss_Arbiter);
      fillTechType(TechTypeIds::Stasis_Field, "Stasis Field",150,150,100,UnitTypeIds::Protoss_Arbiter_Tribunal,RaceIds::Protoss,WeaponTypeIds::Stasis_Field,UnitTypeIds::Protoss_Arbiter);
      fillTechType(TechTypeIds::Archon_Warp, "Archon Warp",0,0,0,UnitTypeIds::None,RaceIds::Protoss,WeaponTypeIds::None,UnitTypeIds::Protoss_High_Templar);
      fillTechType(TechTypeIds::Restoration, "Restoration",100,100,50,UnitTypeIds::Terran_Academy,RaceIds::Terran,WeaponTypeIds::Restoration,UnitTypeIds::Terran_Medic);
      fillTechType(TechTypeIds::Disruption_Web, "Disruption Web",200,200,125,UnitTypeIds::Protoss_Fleet_Beacon,RaceIds::Protoss,WeaponTypeIds::Disruption_Web,UnitTypeIds::Protoss_Corsair);
      fillTechType(TechTypeIds::Mind_Control, "Mind Control",200,200,150,UnitTypeIds::Protoss_Templar_Archives,RaceIds::Protoss,WeaponTypeIds::Mind_Control,UnitTypeIds::Protoss_Dark_Archon);
      fillTechType(TechTypeIds::Dark_Archon_Meld, "Dark Archon Meld",0,0,0,UnitTypeIds::None,RaceIds::Protoss,WeaponTypeIds::None,UnitTypeIds::Protoss_Dark_Templar);
      fillTechType(TechTypeIds::Feedback, "Feedback",0,0,50,UnitTypeIds::None,RaceIds::Protoss,WeaponTypeIds::Feedback,UnitTypeIds::Protoss_Dark_Archon);
      fillTechType(TechTypeIds::Optical_Flare, "Optical Flare",100,100,75,UnitTypeIds::Terran_Academy,RaceIds::Terran,WeaponTypeIds::Optical_Flare,UnitTypeIds::Terran_Medic);
      fillTechType(TechTypeIds::Maelstrom, "Maelstrom",100,100,100,UnitTypeIds::Protoss_Templar_Archives,RaceIds::Protoss,WeaponTypeIds::Maelstrom,UnitTypeIds::Protoss_Dark_Archon);
      fillTechType(TechTypeIds::Lurker_Aspect, "Lurker Aspect",200,200,0,UnitTypeIds::Zerg_Hydralisk_Den,RaceIds::Zerg,WeaponTypeIds::None,UnitTypeIds::Zerg_Lurker);
      fillTechType(TechTypeIds::Healing, "Healing",0,0,1,UnitTypeIds::None,RaceIds::Terran,WeaponTypeIds::None,UnitTypeIds::Terran_Medic);
      fillTechType(TechTypeIds::None, "None",0,0,0,UnitTypeIds::None,RaceIds::None,WeaponTypeIds::None,UnitTypeIds::None);
      fillTechType(TechTypeIds::Nuclear_Strike, "Nuclear Strike",0,0,0,UnitTypeIds::None,RaceIds::Terran,WeaponTypeIds::Nuclear_Strike,UnitTypeIds::Terran_Ghost);
      techTypeSet.insert(TechTypeIds::Stim_Packs);
      techTypeSet.insert(TechTypeIds::Lockdown);
      techTypeSet.insert(TechTypeIds::EMP_Shockwave);
      techTypeSet.insert(TechTypeIds::Spider_Mines);
      techTypeSet.insert(TechTypeIds::Scanner_Sweep);
      techTypeSet.insert(TechTypeIds::Tank_Siege_Mode);
      techTypeSet.insert(TechTypeIds::Defensive_Matrix);
      techTypeSet.insert(TechTypeIds::Irradiate);
      techTypeSet.insert(TechTypeIds::Yamato_Gun);
      techTypeSet.insert(TechTypeIds::Cloaking_Field);
      techTypeSet.insert(TechTypeIds::Personnel_Cloaking);
      techTypeSet.insert(TechTypeIds::Burrowing);
      techTypeSet.insert(TechTypeIds::Infestation);
      techTypeSet.insert(TechTypeIds::Spawn_Broodlings);
      techTypeSet.insert(TechTypeIds::Dark_Swarm);
      techTypeSet.insert(TechTypeIds::Plague);
      techTypeSet.insert(TechTypeIds::Consume);
      techTypeSet.insert(TechTypeIds::Ensnare);
      techTypeSet.insert(TechTypeIds::Parasite);
      techTypeSet.insert(TechTypeIds::Psionic_Storm);
      techTypeSet.insert(TechTypeIds::Hallucination);
      techTypeSet.insert(TechTypeIds::Recall);
      techTypeSet.insert(TechTypeIds::Stasis_Field);
      techTypeSet.insert(TechTypeIds::Archon_Warp);
      techTypeSet.insert(TechTypeIds::Restoration);
      techTypeSet.insert(TechTypeIds::Disruption_Web);
      techTypeSet.insert(TechTypeIds::Mind_Control);
      techTypeSet.insert(TechTypeIds::Dark_Archon_Meld);
      techTypeSet.insert(TechTypeIds::Feedback);
      techTypeSet.insert(TechTypeIds::Optical_Flare);
      techTypeSet.insert(TechTypeIds::Maelstrom);
      techTypeSet.insert(TechTypeIds::Lurker_Aspect);
      techTypeSet.insert(TechTypeIds::Healing);
      techTypeSet.insert(TechTypeIds::None);
      techTypeSet.insert(TechTypeIds::Nuclear_Strike);

      for(std::set<TechTypeId>::iterator i = techTypeSet.begin(); i != techTypeSet.end(); i++)
      {
        techTypeMap.insert(std::make_pair(std::string(techTypeData[*i].name), *i));
      }
    }
  }
  namespace TechTypes
  {
    TechTypeId getIdByName(const std::string& name)
    {
      std::map<std::string, TechTypeId>::iterator i = techTypeMap.find(name);
      if (i == techTypeMap.end()) return TechTypeIds::None;
      return (*i).second;
    }
    std::set<TechTypeId>& allTechTypes()
    {
      return techTypeSet;
    }
  }
}


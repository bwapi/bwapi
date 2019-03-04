#include <string>
#include <BWAPI/TechType.h>
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/WeaponType.h>

namespace BWAPI
{
  // NAMES
  template <>
  const std::string Type<TechType, TechTypes::Enum::Unknown>::typeNames[TechTypes::Enum::MAX] =
  {
    "Stim_Packs",
    "Lockdown",
    "EMP_Shockwave",
    "Spider_Mines",
    "Scanner_Sweep",
    "Tank_Siege_Mode",
    "Defensive_Matrix",
    "Irradiate",
    "Yamato_Gun",
    "Cloaking_Field",
    "Personnel_Cloaking",
    "Burrowing",
    "Infestation",
    "Spawn_Broodlings",
    "Dark_Swarm",
    "Plague",
    "Consume",
    "Ensnare",
    "Parasite",
    "Psionic_Storm",
    "Hallucination",
    "Recall",
    "Stasis_Field",
    "Archon_Warp",
    "Restoration",
    "Disruption_Web",
    "",
    "Mind_Control",
    "Dark_Archon_Meld",
    "Feedback",
    "Optical_Flare",
    "Maelstrom",
    "Lurker_Aspect",
    "",
    "Healing",
    "", "", "", "", "", "", "", "", "",
    "None",
    "Nuclear_Strike",
    "Unknown"
  };

  // LOCALIZATION
  std::string techLocalNames[TechTypes::Enum::MAX];

  // DEFAULTS
  static const int defaultOreCost[TechTypes::Enum::MAX] =         // Same as default gas cost
  { 100, 200, 200, 100, 0, 150, 0, 200, 100, 150, 100, 100, 0, 100, 0, 200, 100, 100, 0, 200, 150, 150, 150, 0, 100, 200, 0, 200, 0, 100, 100, 100, 200 };
  static const int defaultTimeCost[TechTypes::Enum::MAX] =
  { 1200, 1500, 1800, 1200, 0, 1200, 0, 1200, 1800, 1500, 1200, 1200, 0, 1200, 0, 1500, 1500, 1200, 0, 1800, 1200, 1800, 1500, 0, 1200, 1200, 0, 1800, 0, 1800, 1800, 1500, 1800 };
  static const int defaultEnergyCost[TechTypes::Enum::MAX] =
  { 0, 100, 100, 0, 50, 0, 100, 75, 150, 25, 25, 0, 0, 150, 100, 150, 0, 75, 75, 75, 100, 150, 100, 0, 50, 125, 0, 150, 0, 50, 75, 100, 0, 0, 1 };
  
  // ACTUAL
  int oreCost[TechTypes::Enum::MAX], gasCost[TechTypes::Enum::MAX], timeCost[TechTypes::Enum::MAX], energyCost[TechTypes::Enum::MAX];

  namespace techInternalResearches
  {
    using namespace UnitTypes;
    static const UnitType whatResearches[TechTypes::Enum::MAX] =
    { Terran_Academy, Terran_Covert_Ops, Terran_Science_Facility, Terran_Machine_Shop,
    None, Terran_Machine_Shop, None, Terran_Science_Facility, Terran_Physics_Lab, 
    Terran_Control_Tower, Terran_Covert_Ops, Zerg_Hatchery, None, Zerg_Queens_Nest,
    None, Zerg_Defiler_Mound, Zerg_Defiler_Mound, Zerg_Queens_Nest, None,
    Protoss_Templar_Archives, Protoss_Templar_Archives, Protoss_Arbiter_Tribunal,
    Protoss_Arbiter_Tribunal, None, Terran_Academy, Protoss_Fleet_Beacon, None,
    Protoss_Templar_Archives, None, None, Terran_Academy, Protoss_Templar_Archives,
    Zerg_Hydralisk_Den, None, None, None, None, None, None, None, None, None, None, None,
    None, None, Unknown
    };
  }
  namespace techInternalRaces
  {
    using namespace Races;
    static const Race techRaces[TechTypes::Enum::MAX] =
    { Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran,
    Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg,
    Protoss, Protoss, Protoss, Protoss, Protoss, 
    Terran, Protoss, None, Protoss, Protoss, Protoss, Terran, Protoss, Zerg, None, Terran,
    None, None, None, None, None, None, None, None, None, None, Terran, Unknown
    };
  }
  namespace techInternalWeapons
  {
    using namespace WeaponTypes;
    static const WeaponType techWeapons[TechTypes::Enum::MAX] =
    {
      None, Lockdown, EMP_Shockwave, Spider_Mines, None, None, None, Irradiate, Yamato_Gun,
      None, None, None, None, Spawn_Broodlings, Dark_Swarm, Plague, Consume, Ensnare, Parasite,
      Psionic_Storm, None, None, Stasis_Field, None, Restoration, Disruption_Web, None, Mind_Control,
      None, Feedback, Optical_Flare, Maelstrom, None, None, None, None, None, None, None, None, None, 
      None, None, None, None, Nuclear_Strike, Unknown
    };
  }

#define TARG_UNIT  1
#define TARG_POS  2
#define TARG_BOTH  3
  static const int techTypeFlags[TechTypes::Enum::MAX] =
  { 0, TARG_UNIT, TARG_BOTH, TARG_POS, TARG_BOTH, 0, TARG_UNIT, TARG_UNIT, TARG_UNIT, 0, 0, 0, 
  TARG_UNIT, TARG_UNIT, TARG_BOTH, TARG_BOTH, TARG_UNIT, TARG_BOTH, TARG_UNIT, TARG_BOTH, TARG_UNIT,
  TARG_BOTH, TARG_BOTH, TARG_UNIT, TARG_UNIT, TARG_BOTH, 0, TARG_UNIT, TARG_UNIT, TARG_UNIT, TARG_UNIT,
  TARG_BOTH, 0, 0, TARG_BOTH, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TARG_BOTH };

  namespace techInternalOrders
  {
    using namespace Orders;
    static const Order techOrders[TechTypes::Enum::MAX] =
    {  None, CastLockdown, CastEMPShockwave, PlaceMine, CastScannerSweep, None, CastDefensiveMatrix,
      CastIrradiate, FireYamatoGun, None, None, None, CastInfestation, CastSpawnBroodlings,
      CastDarkSwarm, CastPlague, CastConsume, CastEnsnare, CastParasite, CastPsionicStorm,
      CastHallucination, CastRecall, CastStasisField, None, CastRestoration, CastDisruptionWeb,
      None, CastMindControl, None, CastFeedback, CastOpticalFlare, CastMaelstrom, None, None, MedicHeal,
      None, None, None, None, None, None, None, None, None, None, NukePaint, Unknown
    };
  }
  namespace techInternalUsage
  {
    using namespace UnitTypes;

    static const UnitType::set techWhatUses[TechTypes::Enum::MAX] =
    {
      // Stimpacks
      { Terran_Marine, Terran_Firebat, Hero_Jim_Raynor_Marine, Hero_Gui_Montag },
      // Lockdown
      { Terran_Ghost, Hero_Alexei_Stukov, Hero_Infested_Duran, Hero_Samir_Duran, Hero_Sarah_Kerrigan },
      // EMP
      { Terran_Science_Vessel, Hero_Magellan },
      // Spider Mine
      { Terran_Vulture, Hero_Jim_Raynor_Vulture },
      // Scanner Sweep
      { Terran_Comsat_Station },
      // Siege Mode
      { Terran_Siege_Tank_Tank_Mode, Terran_Siege_Tank_Siege_Mode, Hero_Edmund_Duke_Tank_Mode, Hero_Edmund_Duke_Siege_Mode },
      // Defensive Matrix
      { Terran_Science_Vessel, Hero_Magellan },
      // Irradiate
      { Terran_Science_Vessel, Hero_Magellan },
      // Yamato Cannon
      { Terran_Battlecruiser, Hero_Gerard_DuGalle, Hero_Hyperion, Hero_Norad_II },
      // Cloaking Field
      { Terran_Wraith, Hero_Tom_Kazansky },
      // Personnel Cloaking
      { Terran_Ghost, Hero_Alexei_Stukov, Hero_Infested_Duran, Hero_Samir_Duran, Hero_Sarah_Kerrigan, Hero_Infested_Kerrigan },
      // Burrow
      { Zerg_Zergling, Zerg_Hydralisk, Zerg_Drone, Zerg_Defiler, Zerg_Infested_Terran, Hero_Unclean_One, Hero_Hunter_Killer, Hero_Devouring_One, Zerg_Lurker },
      // Infestation
      { Zerg_Queen, Hero_Matriarch },
      // Spawn Broodlings
      { Zerg_Queen, Hero_Matriarch },
      // Dark Swarm
      { Zerg_Defiler, Hero_Unclean_One },
      // Plague
      { Zerg_Defiler, Hero_Unclean_One },
      // Consume
      { Zerg_Defiler, Hero_Unclean_One, Hero_Infested_Kerrigan, Hero_Infested_Duran },
      // Ensnare
      { Zerg_Queen, Hero_Matriarch, Hero_Infested_Kerrigan },
      // Parasite
      { Zerg_Queen, Hero_Matriarch },
      // Psi Storm
      { Protoss_High_Templar, Hero_Tassadar, Hero_Infested_Kerrigan },
      // Hallucination
      { Protoss_High_Templar, Hero_Tassadar },
      // Recall
      { Protoss_Arbiter, Hero_Danimoth },
      // Stasis Field
      { Protoss_Arbiter, Hero_Danimoth },
      // Archon Warp
      { Protoss_High_Templar },
      // Restoration
      { Terran_Medic },
      // Disruption Web
      { Protoss_Corsair, Hero_Raszagal },
      // Unused
      {},
      // Mind Control
      { Protoss_Dark_Archon },
      // Dark Archon Meld
      { Protoss_Dark_Templar },
      // Feedback
      { Protoss_Dark_Archon },
      // Optical Flare
      { Terran_Medic },
      // Maelstrom
      { Protoss_Dark_Archon },
      // Lurker Aspect
      { Zerg_Hydralisk },
      // Unused
      {},
      // Healing
      { Terran_Medic },
      // Unused
      {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
      // Extra (Nuke)
      { Terran_Ghost },
      {}
    };
  }

  namespace TechTypeSet
  {
    using namespace TechTypes;
    const TechType::set techTypeSet = { Stim_Packs, Lockdown, EMP_Shockwave, Spider_Mines,
      Scanner_Sweep, Tank_Siege_Mode, Defensive_Matrix, Irradiate,
      Yamato_Gun, Cloaking_Field, Personnel_Cloaking, Burrowing,
      Infestation, Spawn_Broodlings, Dark_Swarm, Plague, Consume,
      Ensnare, Parasite, Psionic_Storm, Hallucination, Recall,
      Stasis_Field, Archon_Warp, Restoration, Disruption_Web,
      Mind_Control, Dark_Archon_Meld, Feedback, Optical_Flare,
      Maelstrom, Lurker_Aspect, Healing, None, Nuclear_Strike, Unknown };
  }
  Race TechType::getRace() const
  {
    return techInternalRaces::techRaces[this->getID()];
  }
  int TechType::mineralPrice() const
  {
    return defaultOreCost[this->getID()];
  }
  int TechType::gasPrice() const
  {
    // Gas price is exactly the same as mineral price
    return mineralPrice();
  }
  int TechType::researchTime() const
  {
    return defaultTimeCost[this->getID()];
  }
  int TechType::energyCost() const
  {
    return defaultEnergyCost[this->getID()];
  }
  UnitType TechType::whatResearches() const
  {
    return techInternalResearches::whatResearches[this->getID()];
  }
  WeaponType TechType::getWeapon() const
  {
    return techInternalWeapons::techWeapons[this->getID()];
  }
  bool TechType::targetsUnit() const
  {
    return !!(techTypeFlags[this->getID()] & TARG_UNIT);
  }
  bool TechType::targetsPosition() const
  {
    return !!(techTypeFlags[this->getID()] & TARG_POS);
  }
  const UnitType::set& TechType::whatUses() const
  {
    return techInternalUsage::techWhatUses[this->getID()];
  }
  Order TechType::getOrder() const
  {
    return techInternalOrders::techOrders[this->getID()];
  }
  const TechType::set& TechTypes::allTechTypes()
  {
    return TechTypeSet::techTypeSet;
  }
  UnitType TechType::requiredUnit() const
  {
    if (*this == TechTypes::Lurker_Aspect)
      return UnitTypes::Zerg_Lair;
    return UnitTypes::None;
  }
}


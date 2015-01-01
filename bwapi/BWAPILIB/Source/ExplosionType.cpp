#include <string>
#include <BWAPI/ExplosionType.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  template <>
  const std::string Type<ExplosionType, ExplosionTypes::Enum::Unknown>::typeNames[ExplosionTypes::Enum::MAX] =
  {
    "None",
    "Normal",
    "Radial_Splash",
    "Enemy_Splash",
    "Lockdown",
    "Nuclear_Missile",
    "Parasite",
    "Broodlings",
    "EMP_Shockwave",
    "Irradiate",
    "Ensnare",
    "Plague",
    "Stasis_Field",
    "Dark_Swarm",
    "Consume",
    "Yamato_Gun",
    "Restoration",
    "Disruption_Web",
    "Corrosive_Acid",
    "Mind_Control",
    "Feedback",
    "Optical_Flare",
    "Maelstrom",
    "Unused",
    "Air_Splash",
    "Unknown"
  };

  namespace ExplosionTypeSet
  {
    using namespace ExplosionTypes::Enum;
    const ExplosionType::set explosionTypeSet = { None, Normal, Radial_Splash, Enemy_Splash, Lockdown, Nuclear_Missile,
      Parasite, Broodlings, EMP_Shockwave, Irradiate, Ensnare, Plague,
      Stasis_Field, Dark_Swarm, Consume, Yamato_Gun, Restoration, Disruption_Web,
      Corrosive_Acid, Mind_Control, Feedback, Optical_Flare, Maelstrom,
      Air_Splash, Unknown };
  }
  namespace ExplosionTypes
  {
    BWAPI_TYPEDEF(ExplosionType,None);
    BWAPI_TYPEDEF(ExplosionType,Normal);
    BWAPI_TYPEDEF(ExplosionType,Radial_Splash);
    BWAPI_TYPEDEF(ExplosionType,Enemy_Splash);
    BWAPI_TYPEDEF(ExplosionType,Lockdown);
    BWAPI_TYPEDEF(ExplosionType,Nuclear_Missile);
    BWAPI_TYPEDEF(ExplosionType,Parasite);
    BWAPI_TYPEDEF(ExplosionType,Broodlings);
    BWAPI_TYPEDEF(ExplosionType,EMP_Shockwave);
    BWAPI_TYPEDEF(ExplosionType,Irradiate);
    BWAPI_TYPEDEF(ExplosionType,Ensnare);
    BWAPI_TYPEDEF(ExplosionType,Plague);
    BWAPI_TYPEDEF(ExplosionType,Stasis_Field);
    BWAPI_TYPEDEF(ExplosionType,Dark_Swarm);
    BWAPI_TYPEDEF(ExplosionType,Consume);
    BWAPI_TYPEDEF(ExplosionType,Yamato_Gun);
    BWAPI_TYPEDEF(ExplosionType,Restoration);
    BWAPI_TYPEDEF(ExplosionType,Disruption_Web);
    BWAPI_TYPEDEF(ExplosionType,Corrosive_Acid);
    BWAPI_TYPEDEF(ExplosionType,Mind_Control);
    BWAPI_TYPEDEF(ExplosionType,Feedback);
    BWAPI_TYPEDEF(ExplosionType,Optical_Flare);
    BWAPI_TYPEDEF(ExplosionType,Maelstrom);
    BWAPI_TYPEDEF(ExplosionType,Air_Splash);
    BWAPI_TYPEDEF(ExplosionType,Unknown);
  }
  ExplosionType::ExplosionType(int id) : Type( id )
  {}
  const ExplosionType::set& ExplosionTypes::allExplosionTypes()
  {
    return ExplosionTypeSet::explosionTypeSet;
  }
}

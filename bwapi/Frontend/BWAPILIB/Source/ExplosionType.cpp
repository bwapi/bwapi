#include <string>
#include <BWAPI/ExplosionType.h>

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
    using namespace ExplosionTypes;
    const ExplosionType::set explosionTypeSet = { None, Normal, Radial_Splash, Enemy_Splash, Lockdown, Nuclear_Missile,
      Parasite, Broodlings, EMP_Shockwave, Irradiate, Ensnare, Plague,
      Stasis_Field, Dark_Swarm, Consume, Yamato_Gun, Restoration, Disruption_Web,
      Corrosive_Acid, Mind_Control, Feedback, Optical_Flare, Maelstrom,
      Air_Splash, Unknown };
  }
  const ExplosionType::set& ExplosionTypes::allExplosionTypes()
  {
    return ExplosionTypeSet::explosionTypeSet;
  }
}

#include "helpers.h"

#include <string>

#define NAMESPACE_T(x) std::string const & namespaceOf< x >::value = #x "s";
NAMESPACE_T(BulletType)
NAMESPACE_T(Color)
NAMESPACE_T(DamageType)
NAMESPACE_T(Error)
NAMESPACE_T(ExplosionType)
NAMESPACE_T(GameType)
NAMESPACE_T(Order)
NAMESPACE_T(PlayerType)
NAMESPACE_T(Race)
NAMESPACE_T(TechType)
NAMESPACE_T(UnitCommandType)
NAMESPACE_T(UnitSizeType)
NAMESPACE_T(UnitType)
NAMESPACE_T(UpgradeType)
NAMESPACE_T(WeaponType)
#undef NAMESPACE_T

template <>
std::string tref(std::string const &t)
{
  return t;
}

std::string img(std::string const & src)
{
  return "@htmlonly\n<img src=\"" + src + "\">\n@endhtmlonly ";
}

std::string ore()
{
  return img("ore.png");
}

std::string energy()
{
  return img("energy.png");
}

std::string gas(Race race)
{
  return img("gas" + std::to_string(race.getID() % 3) + ".png");
}

std::string supply(Race race)
{
  return img("supply" + std::to_string(race.getID() % 3) + ".png");
}

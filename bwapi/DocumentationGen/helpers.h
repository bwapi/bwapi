#pragma once
#include <BWAPI.h>

#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace BWAPI;

template <class TL, class TR>
std::string row(TL const & left, TR const & right)
{
  std::ostringstream ss;
  ss << "<tr><td><strong>"
    << left
    << "</strong></td><td>"
    << right
    << "</td></tr>\n";
  return ss.str();
}

template <class T> struct namespaceOf { static std::string const & value; };

#define NAMESPACE_T(x) template <> struct namespaceOf < x > { static std::string const & value; };
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

template <class T>
std::string tref(T const & t)
{
  std::string friendlyName = t.toString();
  std::replace(friendlyName.begin(), friendlyName.end(), '_', ' ');
  return "@ref " + namespaceOf<T>::value + "::" + t.toString() + " \"" + friendlyName + "\"";
}

template <>
std::string tref(std::string const &t);

template <class T>
std::string separate(T const & list, char delim = ',')
{
  std::ostringstream ss;
  for (auto a = list.begin(); a != list.end(); ++a)
  {
    if (a != list.begin()) ss << delim << ' ';
    ss << tref(*a);
  }
  return ss.str();
}

std::string img(std::string const & src);
std::string ore();
std::string energy();
std::string gas(Race race);
std::string supply(Race race);

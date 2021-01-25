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

std::string img(std::string const & src);
std::string imgOre();
std::string imgEnergy();
std::string imgGas(Race race);
std::string imgSupply(Race race);

std::string icon(WeaponType const & t);
std::string icon(UpgradeType const & t);
std::string icon(TechType const & t);
std::string icon(Order const & t);
std::string icon(UnitType const & t);


template <class T>
std::string friendlyName(T const & t)
{
  std::string name = t.toString();
  std::replace(name.begin(), name.end(), '_', ' ');
  return name;
}

template <class T>
std::string tref(T const & t)
{
  return "@ref " + namespaceOf<T>::value + "::" + t.toString() + " \"" + friendlyName(t) + "\"";
}

template <class T>
std::string iconref(T const & t)
{
  return icon(t) + tref(t);
}

template <>
std::string tref(std::string const &t);

template <class T>
std::string makelist(T const & list, char delim = ',')
{
  std::ostringstream ss;
  for (auto a = list.begin(); a != list.end(); ++a)
  {
    if (a != list.begin()) ss << delim << ' ';
    ss << tref(*a);
  }
  return ss.str();
}

template <class T>
std::string makeiconlist(T const & list)
{
  std::ostringstream ss;
  for (auto a = list.begin(); a != list.end(); ++a)
  {
    if (a != list.begin()) ss << ' ';
    ss << iconref(*a);
  }
  return ss.str();
}

template <class T>
std::string makekeywords(T const& t)
{
  std::ostringstream ss;
  ss << "@m_keywords{";
  std::string name = t.getName();
  for (char& c : name) {
    if (c == '_') c = ' ';
  }
  ss << name << "}\n";
  return ss.str();
}

template <class T>
std::string docEnum(T const & t)
{
  std::ostringstream ss;
  ss << "/*!\n"
    << "@var BWAPI::" << namespaceOf<T>::value << "::Enum::" << t.getName() << "\n"
    "Value of " << t.getID() << "  [0x" << std::hex << t.getID() << "]. " << std::dec
    << "See BWAPI::" << namespaceOf<T>::value << "::" << t.getName() << ".\n"
    << "*/\n";
  return ss.str();
}

template <class T>
std::string docBegin(T const & t)
{
  std::ostringstream ss;
  ss << "/*!\n";
  ss << "@var BWAPI::" << namespaceOf<T>::value << "::" << t.getName() << "\n";
  return ss.str();
}

inline std::string docEnd()
{
  return "\n*/\n";
}

template <class T>
std::string docIntro(T const & t)
{
  std::ostringstream ss;
  ss << friendlyName(t) << ". Corresponds with " << namespaceOf<T>::value << "::Enum::" << t.getName() << ".\n";
  return ss.str();
}

inline std::string tab(size_t level)
{
  return std::string(level * 2, ' ');
}

template <class T>
inline std::string setToString(T set, const std::string& prefix = "")
{
  std::ostringstream ss;
  ss << "{ ";
  bool first = true;
  for (auto &v : set)
  {
    if (!first)
      ss << ", ";
    first = false;

    ss << prefix << v;
  }
  ss << " }";
  return ss.str();
}

template <class T>
inline std::string mapToString(T map, const std::string& prefix1 = "", const std::string& prefix2 = "")
{
  std::ostringstream ss;
  ss << "{ ";
  bool first = true;
  for (auto &v : map)
  {
    if (!first)
      ss << ", ";
    first = false;

    ss << "{" << prefix1 << v.first << ", " << prefix2 << v.second << "}";
  }
  ss << " }";
  return ss.str();
}

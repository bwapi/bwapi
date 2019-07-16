#include <BWAPI/Player.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/Color.h>

using namespace BWAPI;

//-------------------------------------------- TEXT COLOR --------------------------------------------------
char PlayerInterface::getTextColor() const
{
  switch ( this->getColor() )
  {
  case 111: // red
    return Text::BrightRed;
  case 165: // blue
    return Text::Blue;
  case 159: // teal
    return Text::Teal;
  case 164: // purp
    return Text::Purple;
  case 179: // oj
    return Text::Orange;
  case 19:  // brown
    return Text::Brown;
  case 84:  // white
    return Text::PlayerWhite;
  case 135: // yellow
    return Text::PlayerYellow;
  case 185: // green p9
    return Text::DarkGreen;
  case 136: // p10
    return Text::LightYellow;
  case 134: // p11
    return Text::Tan;
  case 51:  // p12
    return Text::GreyBlue;
  default:
    return Text::Default;
  }
}
//-------------------------------------- UNIT TYPE REQUIREMENT ---------------------------------------------
bool PlayerInterface::hasUnitTypeRequirement(UnitType unit, int amount) const
{
  if (unit == UnitTypes::None)
    return true;

  switch (unit)
  {
  case UnitTypes::Enum::Zerg_Hatchery:
    return completedUnitCount(UnitTypes::Zerg_Hatchery) + allUnitCount(UnitTypes::Zerg_Lair) + allUnitCount(UnitTypes::Zerg_Hive) >= amount;
  case UnitTypes::Enum::Zerg_Lair:
    return completedUnitCount(UnitTypes::Zerg_Lair) + allUnitCount(UnitTypes::Zerg_Hive) >= amount;
  case UnitTypes::Enum::Zerg_Spire:
    return completedUnitCount(UnitTypes::Zerg_Spire) + allUnitCount(UnitTypes::Zerg_Greater_Spire) >= amount;
  default:
    return completedUnitCount(unit) >= amount;
  }
}

int PlayerInterface::weaponMaxRange(WeaponType weapon) const
{
  int range = weapon.maxRange();
  if ((weapon == WeaponTypes::Gauss_Rifle   && getUpgradeLevel(UpgradeTypes::U_238_Shells) > 0) ||
    (weapon == WeaponTypes::Needle_Spines && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0))
    range += 1 * 32;
  else if (weapon == WeaponTypes::Phase_Disruptor       && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0)
    range += 2 * 32;
  else if (weapon == WeaponTypes::Hellfire_Missile_Pack && getUpgradeLevel(UpgradeTypes::Charon_Boosters) > 0)
    range += 3 * 32;
  return range;
}

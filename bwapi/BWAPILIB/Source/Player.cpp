#include <BWAPI/Player.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/Color.h>

using namespace BWAPI;

//--------------------------------------------- INCOMPLETE UNIT COUNT --------------------------------------
int PlayerInterface::incompleteUnitCount(UnitType unit) const
{
  return this->allUnitCount(unit) - this->completedUnitCount(unit);
}
//--------------------------------------------- MAX ENERGY -------------------------------------------------
int PlayerInterface::maxEnergy(UnitType unit) const
{
  int energy = unit.maxEnergy();
  if ((unit == UnitTypes::Protoss_Arbiter       && getUpgradeLevel(UpgradeTypes::Khaydarin_Core)    > 0) ||
      (unit == UnitTypes::Protoss_Corsair       && getUpgradeLevel(UpgradeTypes::Argus_Jewel)       > 0) ||
      (unit == UnitTypes::Protoss_Dark_Archon   && getUpgradeLevel(UpgradeTypes::Argus_Talisman)    > 0) ||
      (unit == UnitTypes::Protoss_High_Templar  && getUpgradeLevel(UpgradeTypes::Khaydarin_Amulet)  > 0) ||
      (unit == UnitTypes::Terran_Ghost          && getUpgradeLevel(UpgradeTypes::Moebius_Reactor)   > 0) ||
      (unit == UnitTypes::Terran_Battlecruiser  && getUpgradeLevel(UpgradeTypes::Colossus_Reactor)  > 0) ||
      (unit == UnitTypes::Terran_Science_Vessel && getUpgradeLevel(UpgradeTypes::Titan_Reactor)     > 0) ||
      (unit == UnitTypes::Terran_Wraith         && getUpgradeLevel(UpgradeTypes::Apollo_Reactor)    > 0) ||
      (unit == UnitTypes::Terran_Medic          && getUpgradeLevel(UpgradeTypes::Caduceus_Reactor)  > 0) ||
      (unit == UnitTypes::Zerg_Defiler          && getUpgradeLevel(UpgradeTypes::Metasynaptic_Node) > 0) ||
      (unit == UnitTypes::Zerg_Queen            && getUpgradeLevel(UpgradeTypes::Gamete_Meiosis)    > 0) )
    energy += 50;
  return energy;
}
//--------------------------------------------- TOP SPEED --------------------------------------------------
double PlayerInterface::topSpeed(UnitType unit) const
{
  double speed = unit.topSpeed();
  if ((unit == UnitTypes::Terran_Vulture   && getUpgradeLevel(UpgradeTypes::Ion_Thrusters)        > 0) ||
      (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Pneumatized_Carapace) > 0) ||
      (unit == UnitTypes::Zerg_Zergling    && getUpgradeLevel(UpgradeTypes::Metabolic_Boost)      > 0) ||
      (unit == UnitTypes::Zerg_Hydralisk   && getUpgradeLevel(UpgradeTypes::Muscular_Augments)    > 0) ||
      (unit == UnitTypes::Protoss_Zealot   && getUpgradeLevel(UpgradeTypes::Leg_Enhancements)     > 0) ||
      (unit == UnitTypes::Protoss_Shuttle  && getUpgradeLevel(UpgradeTypes::Gravitic_Drive)       > 0) ||
      (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Gravitic_Boosters)    > 0) ||
      (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Gravitic_Thrusters)   > 0) ||
      (unit == UnitTypes::Zerg_Ultralisk   && getUpgradeLevel(UpgradeTypes::Anabolic_Synthesis)   > 0))
  {
    if ( unit == UnitTypes::Protoss_Scout )
      speed += 427/256.0;
    else
      speed = speed * 1.5;
    if ( speed < 853/256.0 )
      speed = 853/256.0;
    //acceleration *= 2;
    //turnRadius *= 2;
  }
  return speed;
}
//----------------------------------------------- WEAPON MAX RANGE -----------------------------------------
int PlayerInterface::weaponMaxRange(WeaponType weapon) const
{
  int range = weapon.maxRange();
  if ( (weapon == WeaponTypes::Gauss_Rifle   && getUpgradeLevel(UpgradeTypes::U_238_Shells)   > 0) ||
        (weapon == WeaponTypes::Needle_Spines && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0) )
    range += 1*32;
  else if ( weapon == WeaponTypes::Phase_Disruptor       && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0 )
    range += 2*32;
  else if ( weapon == WeaponTypes::Hellfire_Missile_Pack && getUpgradeLevel(UpgradeTypes::Charon_Boosters)    > 0 )
    range += 3*32;
  return range;
}
//--------------------------------------------- SIGHT RANGE ------------------------------------------------
int PlayerInterface::sightRange(UnitType unit) const
{
  int range = unit.sightRange();
  if ((unit == UnitTypes::Terran_Ghost     && getUpgradeLevel(UpgradeTypes::Ocular_Implants) > 0) ||
      (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Antennae)        > 0) ||
      (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Sensor_Array)    > 0) ||
      (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Apial_Sensors)   > 0))
    range = 11*32;
  return range;
}
//--------------------------------------------- GROUND WEAPON DAMAGE COOLDOWN ------------------------------
int PlayerInterface::weaponDamageCooldown(UnitType unit) const
{
  int cooldown = unit.groundWeapon().damageCooldown();
  if (unit == UnitTypes::Zerg_Zergling && getUpgradeLevel(UpgradeTypes::Adrenal_Glands) > 0)
  {
    // Divide cooldown by 2
    cooldown /= 2;
    // Prevent cooldown from going out of bounds
    cooldown = std::min(std::max(cooldown,5), 250);
  }
  return cooldown;
}
//--------------------------------------------- ARMOR ------------------------------------------------------
int PlayerInterface::armor(UnitType unit) const
{
  int armor = unit.armor();
  armor += getUpgradeLevel(unit.armorUpgrade());
  if ( unit == UnitTypes::Zerg_Ultralisk && getUpgradeLevel(UpgradeTypes::Chitinous_Plating) > 0 )
    armor += 2;
  else if ( unit == UnitTypes::Hero_Torrasque )
    armor += 2;
  return armor;
}
//-------------------------------------------- DAMAGE ------------------------------------------------------
int PlayerInterface::damage(WeaponType wpn) const
{
  int dmg = wpn.damageAmount();
  dmg += getUpgradeLevel(wpn.upgradeType()) * wpn.damageBonus();
  dmg *= wpn.damageFactor();
  return dmg;
}
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
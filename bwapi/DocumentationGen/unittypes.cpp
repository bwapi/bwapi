#include "helpers.h"


void writeUnitInfo()
{
  std::ofstream of("unittypes.dox");
  for (auto t : UnitTypes::allUnitTypes())
  {
    if (t == UnitTypes::Unknown || t == UnitTypes::None) continue;
    of << "/*!\n"
      << "@var BWAPI::UnitTypes::Enum::" << t.getName() << "\n"
      "Value of " << t.getID() << "  [0x" << std::hex << t.getID() << "]. " << std::dec
      << "See BWAPI::UnitTypes::" << t.getName() << ".\n"
      << "*/\n";

    of << "/*!\n";
    of << "@var BWAPI::UnitTypes::" << t.getName() << "\n";

    //int iconId = t.getID();
    //of << "<img src='icon" << iconId << ".png' style='float:right;'>";

    of << "<table border='0'>";
    //of << row("Identifier", t.getID());
    of << row("Race", tref(t.getRace()));
    of << row("Hit Points", t.maxHitPoints());
    if (t.maxShields() > 0)
    {
      of << row("Shields", t.maxShields());
    }
    if (t.maxEnergy() > 0)
    {
      of << row("Energy", t.maxEnergy());
    }
    of << row("Armor", t.armor());
    of << row("Armor Upgrade", tref(t.armorUpgrade()));

    std::string oreCost = ore() + std::to_string(t.mineralPrice());
    std::string gasCost = gas(t.getRace()) + std::to_string(t.gasPrice());
    std::string supplyCost = supply(t.getRace()) + std::to_string(t.supplyRequired());
    of << row("Cost", oreCost + " " + gasCost + " " + supplyCost);
    of << row("Build Time", std::to_string(t.buildTime()) + " frames");

    if (t.supplyProvided() > 0)
    {
      of << row("Supply Provided", t.supplyProvided());
    }

    of << row("Build Score", t.buildScore());
    of << row("Destroy Score", t.destroyScore());

    of << row("Top Speed", t.topSpeed());
    of << row("Acceleration", t.acceleration());
    of << row("Halt Distance", t.haltDistance());
    of << row("Turn Radius", t.turnRadius());

    // Weapons
    if (t.groundWeapon() != WeaponTypes::None)
    {
      if (t.maxGroundHits() == 1)
      {
        of << row("Ground Weapon", tref(t.groundWeapon()));
      }
      else
      {
        of << row("Ground Weapon", tref(t.groundWeapon()) + " x " + std::to_string(t.maxGroundHits()));
      }
    }
    if (t.airWeapon() != WeaponTypes::None)
    {
      if (t.maxAirHits() == 1)
      {
        of << row("Air Weapon", tref(t.airWeapon()));
      }
      else
      {
        of << row("Air Weapon", tref(t.airWeapon()) + " x " + std::to_string(t.maxAirHits()));
      }
    }

    std::ostringstream ss;
    ss << t.width() << "x" << t.height() << " px; " << t.tileWidth() << "x" << t.tileHeight() << " tiles";
    of << row("Size", ss.str());
    of << row("SizeType", tref(t.size()));

    if (t.spaceRequired() != 255) of << row("Space Required", t.spaceRequired());
    if (t.spaceProvided() > 0) of << row("Transport Space Provided", t.spaceProvided());

    of << row("Seek Range", t.seekRange());
    of << row("Sight Range", t.sightRange());

    if (!t.abilities().empty()) of << row("Abilities", separate(t.abilities()));
    if (!t.upgrades().empty()) of << row("Upgrades", separate(t.upgrades()));
    if (t.requiredTech() != TechTypes::None) of << row("Requires Technology", tref(t.requiredTech()));

    if (!t.requiredUnits().empty())
    {
      UnitType::set reqUnits;
      for (auto i : t.requiredUnits()) reqUnits.insert(i.first);
      of << row("Required Units", separate(reqUnits));
    }

    if (t.whatBuilds().second == 1)
    {
      of << row("Created By", tref(t.whatBuilds().first));
    }
    else if (t.whatBuilds().second > 1)
    {
      of << row("Created By", std::to_string(t.whatBuilds().second) + " " + tref(t.whatBuilds().first));
    }

    of << "<tr><td><strong>Attributes</strong></td><td>";
    std::vector<std::string> attributes;
#define ATTRIB_TEST(x) if (t.is ## x()) attributes.push_back(#x);
    ATTRIB_TEST(Flyer);
    ATTRIB_TEST(Invincible);
    ATTRIB_TEST(Organic);
    ATTRIB_TEST(Mechanical);
    ATTRIB_TEST(Robotic);
    ATTRIB_TEST(Detector);
    ATTRIB_TEST(ResourceContainer);
    ATTRIB_TEST(ResourceDepot);
    ATTRIB_TEST(Refinery);
    ATTRIB_TEST(Worker);
    ATTRIB_TEST(TwoUnitsInOneEgg);
    ATTRIB_TEST(FlyingBuilding);
    ATTRIB_TEST(Addon);
    ATTRIB_TEST(Building);
    ATTRIB_TEST(Neutral);
    ATTRIB_TEST(Hero);
    ATTRIB_TEST(Powerup);
    ATTRIB_TEST(Beacon);
    ATTRIB_TEST(FlagBeacon);
    ATTRIB_TEST(SpecialBuilding);
    ATTRIB_TEST(Spell);
    ATTRIB_TEST(MineralField);
    ATTRIB_TEST(Critter);
#undef ATTRIB_TEST
    if (t.requiresCreep()) attributes.push_back("RequiresCreep");
    if (t.requiresPsi()) attributes.push_back("RequiresPsi");
    if (t.producesLarva()) attributes.push_back("ProducesLarva");
    if (t.hasPermanentCloak()) attributes.push_back("PermanentCloak");
    if (t.regeneratesHP()) attributes.push_back("RegeneratesHP");
    of << separate(attributes);
    of << "</td></tr>\n";

    of << "</table>\n";

    // References
    std::string const & name = t.getName();
    of << "@tl" << name << " @scc" << name << " @sen" << name << " @wik" << name;
    of << "\n*/\n";
  }
}

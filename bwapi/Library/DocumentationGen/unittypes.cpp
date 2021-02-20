#include "helpers.h"


void writeUnitInfo()
{
  std::ofstream of("unittypes.dox");
  for (auto t : UnitTypes::allUnitTypes())
  {
    if (t == UnitTypes::Unknown || t == UnitTypes::None)
    {
      //of << docBegin(t) << docIntro(t) << docEnd();
      continue;
    }
    of << docEnum(t);
    of << docBegin(t);

    //int iconId = t.getID();
    //of << "<img src='icon" << iconId << ".png' style='float:right;'>";
    of << icon(t) << " " << docIntro(t) << "\n";

    of << "<table>";
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
    of << row("Armor Upgrade", iconref(t.armorUpgrade()));

    std::string oreCost = imgOre() + std::to_string(t.mineralPrice());
    std::string gasCost = imgGas(t.getRace()) + std::to_string(t.gasPrice());
    std::string supplyCost = imgSupply(t.getRace()) + std::to_string(t.supplyRequired());
    of << row("Cost", oreCost + " " + gasCost + " " + supplyCost);
    of << row("Build Time", std::to_string(t.buildTime()) + " frames");

    if (t.supplyProvided() > 0)
    {
      of << row("Supply Provided", t.supplyProvided());
    }

    if (t.buildScore() != 0) of << row("Build Score", t.buildScore());
    if (t.destroyScore() != 0) of << row("Destroy Score", t.destroyScore());

    if (t.topSpeed() != 0 || t.acceleration() != 0 || t.haltDistance() != 0)
    {
      of << row("Top Speed", t.topSpeed());
      of << row("Acceleration", t.acceleration());
      of << row("Halt Distance", t.haltDistance());
    }
    if (t.turnRadius() != 0) of << row("Turn Radius", t.turnRadius());

    // Weapons
    if (t.groundWeapon() != WeaponTypes::None)
    {
      if (t.maxGroundHits() == 1)
      {
        of << row("Ground Weapon", iconref(t.groundWeapon()));
      }
      else
      {
        of << row("Ground Weapon", iconref(t.groundWeapon()) + " x " + std::to_string(t.maxGroundHits()));
      }
    }
    if (t.airWeapon() != WeaponTypes::None)
    {
      if (t.maxAirHits() == 1)
      {
        of << row("Air Weapon", iconref(t.airWeapon()));
      }
      else
      {
        of << row("Air Weapon", iconref(t.airWeapon()) + " x " + std::to_string(t.maxAirHits()));
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

    if (!t.abilities().empty()) of << row("Abilities", makeiconlist(t.abilities()));
    if (!t.upgrades().empty()) of << row("Upgrades", makeiconlist(t.upgrades()));
    if (t.requiredTech() != TechTypes::None) of << row("Requires Technology", iconref(t.requiredTech()));

    if (!t.requiredUnits().empty())
    {
      UnitType::set reqUnits;
      for (auto i : t.requiredUnits()) reqUnits.insert(i.first);
      of << row("Required Units", makeiconlist(reqUnits));
    }

    if (t.whatBuilds().second == 1)
    {
      of << row("Created By", iconref(t.whatBuilds().first));
    }
    else if (t.whatBuilds().second > 1)
    {
      of << row("Created By", std::to_string(t.whatBuilds().second) + " x " + iconref(t.whatBuilds().first));
    }

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
    of << row("Attributes", makelist(attributes));

    of << "</table>\n";

    // References
    std::string const & name = t.getName();
    of << "@tl" << name << " @scc" << name << " @wik" << name << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}

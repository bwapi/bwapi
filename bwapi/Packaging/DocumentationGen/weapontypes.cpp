#include "helpers.h"

void writeWeaponInfo()
{
  std::ofstream of("weapontypes.dox");
  for (auto t : WeaponTypes::allWeaponTypes())
  {
    if (t == WeaponTypes::Unknown || t == WeaponTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << icon(t) << " " << docIntro(t) << "\n";

    of << "<table>";
    if (t.getTech() != TechTypes::None) of << row("Technology", iconref(t.getTech()));
    of << row("What uses this?", iconref(t.whatUses()));
    of << row("Damage", t.damageAmount());
    of << row("Damage Bonus", t.damageBonus());
    of << row("Base Cooldown", t.damageCooldown());
    of << row("Damage Factor", t.damageFactor());
    of << row("Upgrade", iconref(t.upgradeType()));
    of << row("DamageType", tref(t.damageType()));
    of << row("ExplosionType", tref(t.explosionType()));
    if (t.minRange() != 0) of << row("Minimum Range", t.minRange());
    of << row("Maximum Range", t.maxRange());

    if (t.innerSplashRadius() != 0 || t.medianSplashRadius() != 0 || t.outerSplashRadius() != 0)
    {
      std::ostringstream ssSplash;
      ssSplash << t.innerSplashRadius() << ", " << t.medianSplashRadius() << ", " << t.outerSplashRadius();
      of << row("Splash Radii", ssSplash.str());
    }

    std::vector<std::string> attributes;
#define ATTRIB_TEST(x) if (t.targets ## x()) attributes.push_back(#x);
    ATTRIB_TEST(Air);
    ATTRIB_TEST(Ground);
    ATTRIB_TEST(Mechanical);
    ATTRIB_TEST(Organic);
    ATTRIB_TEST(NonBuilding);
    ATTRIB_TEST(NonRobotic);
    ATTRIB_TEST(Terrain);
    ATTRIB_TEST(OrgOrMech);
    ATTRIB_TEST(Own);
#undef ATTRIB_TEST
    of << row("Target Attributes", makelist(attributes));
    of << "</table>\n";

    of << makekeywords(t);

    of << docEnd();
  }
}

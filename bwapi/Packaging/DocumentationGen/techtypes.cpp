#include "helpers.h"

#include <set>
#include <string>

void writeTechInfo()
{
  std::ofstream of("techtypes.dox");
  for (auto t : TechTypes::allTechTypes())
  {
    if (t == TechTypes::Unknown || t == TechTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << icon(t) << " " << docIntro(t) << "\n";

    of << "<table>";
    of << row("Race", tref(t.getRace()));

    if (t.mineralPrice() != 0 || t.gasPrice() != 0)
    {
      std::string oreCost = imgOre() + std::to_string(t.mineralPrice());
      std::string gasCost = imgGas(t.getRace()) + std::to_string(t.gasPrice());
      of << row("Cost", oreCost + " " + gasCost);
    }

    if (t.researchTime() != 0) of << row("Research Time", std::to_string(t.researchTime()) + " frames");
    if (t.energyCost() != 0) of << row("Energy Cost", imgEnergy() + std::to_string(t.energyCost()));

    if (t.whatResearches() != UnitTypes::None) of << row("Researched at", iconref(t.whatResearches()));
    if (t.requiredUnit() != UnitTypes::None) of << row("Requires", iconref(t.requiredUnit()));

    std::set<std::string> targets;
    if (t.targetsPosition()) targets.insert("Positions");
    if (t.targetsUnit()) targets.insert("Units");
    if (!targets.empty()) of << row("Targets", makelist(targets));

    if (t.getWeapon() != WeaponTypes::None) of << row("Weapon", iconref(t.getWeapon()));
    if (t.getOrder() != Orders::None) of << row("Order", iconref(t.getOrder()));

    if (!t.whatUses().empty()) of << row("Used by", makeiconlist(t.whatUses()));
    
    of << "</table>\n";

    // References
    std::string const & name = t.getName();
    of << "@tl" << name << " @scc" << name << " @wik" << name << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}

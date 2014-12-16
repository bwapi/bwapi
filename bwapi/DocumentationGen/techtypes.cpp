#include "helpers.h"

void writeTechInfo()
{
  std::ofstream of("techtypes.dox");
  for (auto t : TechTypes::allTechTypes())
  {
    if (t == TechTypes::Unknown || t == TechTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << icon(t) << " " << docIntro(t) << "\n";

    of << "<table border='0'>";
    of << row("Race", tref(t.getRace()));

    std::string oreCost = imgOre() + std::to_string(t.mineralPrice());
    std::string gasCost = imgGas(t.getRace()) + std::to_string(t.gasPrice());
    of << row("Cost", oreCost + " " + gasCost);

    of << row("Research Time", std::to_string(t.researchTime()) + " frames");
    if (t.energyCost() != 0) of << row("Energy Cost", imgEnergy() + std::to_string(t.energyCost()));

    if (t.whatResearches() != UnitTypes::None) of << row("Researched at", iconref(t.whatResearches()));

    std::set<std::string> targets;
    if (t.targetsPosition()) targets.insert("Positions");
    if (t.targetsUnit()) targets.insert("Units");
    if (!targets.empty()) of << row("Targets", makelist(targets));

    if (t.getWeapon() != WeaponTypes::None) of << row("Weapon", iconref(t.getWeapon()));
    if (t.getOrder() != Orders::None) of << row("Order", iconref(t.getOrder()));

    if (!t.whatUses().empty()) of << row("Used by", makeiconlist(t.whatUses()));
    
    of << "</table>\n";

    of << docEnd();
  }
}

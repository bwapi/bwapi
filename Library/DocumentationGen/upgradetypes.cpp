#include "helpers.h"

void writeUpgradeInfo()
{
  std::ofstream of("upgradetypes.dox");
  for (auto t : UpgradeTypes::allUpgradeTypes())
  {
    if (t == UpgradeTypes::Unknown || t == UpgradeTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << icon(t) << " " << docIntro(t) << "\n";

    of << "<table>";
    of << row("Race", tref(t.getRace()));

    std::string oreCost = imgOre() + std::to_string(t.mineralPrice());
    if (t.maxRepeats() > 1) oreCost += " + lvl*" + std::to_string(t.mineralPriceFactor());
    std::string gasCost = imgGas(t.getRace()) + std::to_string(t.gasPrice());
    if (t.maxRepeats() > 1) gasCost += " + lvl*" + std::to_string(t.gasPriceFactor());
    of << row("Cost", oreCost + " " + gasCost);

    std::string timeCost = std::to_string(t.upgradeTime());
    if (t.maxRepeats() > 1) timeCost += " + lvl*" + std::to_string(t.upgradeTimeFactor());
    timeCost += " frames";
    of << row("Upgrade Time", timeCost);

    if (t.maxRepeats() != 1) of << row("Maximum Level", t.maxRepeats());
    if (t.whatUpgrades() != UnitTypes::None) of << row("Upgraded at", iconref(t.whatUpgrades()));
    if (!t.whatUses().empty()) of << row("Used by", makeiconlist(t.whatUses()));
    if (t.whatsRequired(1) != UnitTypes::None) of << row("Level 1 Requires", iconref(t.whatsRequired(1)));
    if (t.whatsRequired(2) != UnitTypes::None) of << row("Level 2 Requires", iconref(t.whatsRequired(2)));
    if (t.whatsRequired(3) != UnitTypes::None) of << row("Level 3 Requires", iconref(t.whatsRequired(3)));

    of << "</table>\n";

    of << makekeywords(t);

    of << docEnd();
  }
}

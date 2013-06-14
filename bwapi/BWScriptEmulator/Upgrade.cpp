#include "Upgrade.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Upgrade);

bool Upgrade::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bUpgradeLevel, bPriority;
  WORD wUpgradeType;
  thread.readTuple( std::tie(bUpgradeLevel, wUpgradeType, bPriority) );

        //this->pTown->MacroManagerSet(MACRO_UPGRADE, bUpgLevel, wUpgType, bPriority, false);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%3u %s %3u", bUpgradeLevel, UpgradeType(wUpgradeType).c_str(), bPriority);
  return true;
}

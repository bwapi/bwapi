#include "UpgradeAction.h"
#include "DefaultActions.h"
#include "AbstractReplayReader.h"
#include <sstream>

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

UpgradeAction::UpgradeAction(PlayerID player)
  : GameAction(player, ReplayTool::Upgrade)
  , upgradeType(BWAPI::UpgradeTypes::Carrier_Capacity)

{
}

UpgradeAction::UpgradeAction(PlayerID player, BWAPI::UpgradeType myTech)
  : GameAction(player, ReplayTool::Upgrade)
  , upgradeType(myTech)

{
}

void UpgradeAction::read(AbstractReplayReader &reader)
{
  upgradeType = reader.readUpgradeType();
}

string UpgradeAction::toString() const
{
  ostringstream ss(GameAction::toString());
  ss << upgradeType;
  return ss.str();
}
#include "TankSiegeAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

TankSiegeAction::TankSiegeAction(PlayerID player, bool mySiege)
  : GameAction(player, (mySiege ? ReplayTool::Tank_Siege : ReplayTool::Tank_Unsiege))
  ,how(0)
  ,siege(mySiege)
{
}

TankSiegeAction::TankSiegeAction(PlayerID player, BYTE myHow, bool mySiege)
  : GameAction(player, (mySiege ? ReplayTool::Tank_Siege : ReplayTool::Tank_Unsiege))
  ,how(myHow)
  ,siege(mySiege)
{

}

void TankSiegeAction::read(AbstractReplayReader &reader)
{
  how = reader.readBYTE();
}

string TankSiegeAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", how ? "Queued" : "");

  return str;
}
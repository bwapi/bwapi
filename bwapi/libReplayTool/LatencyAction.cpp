#include "LatencyAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

LatencyAction::LatencyAction(PlayerID player)
  : GameAction(player, ReplayTool::Set_Latency)
  ,latency(0)
{
}

LatencyAction::LatencyAction(PlayerID player, BYTE myLatency)
  : GameAction(player, ReplayTool::Set_Latency)
  ,latency(myLatency)
{

}

void LatencyAction::read(AbstractReplayReader &reader)
{
  latency = reader.readBYTE();
}

string LatencyAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", latency < 3 ? ReplayTool::pszLatency[latency] : "INVALID");

  return str;
}
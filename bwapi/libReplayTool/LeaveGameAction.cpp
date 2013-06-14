#include "LeaveGameAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

LeaveGameAction::LeaveGameAction(PlayerID player)
  : GameAction(player, ReplayTool::Leave_Game)
  ,leaveType(0)
{
}

LeaveGameAction::LeaveGameAction(PlayerID player, BYTE myLeaveType)
  : GameAction(player, ReplayTool::Leave_Game)
  ,leaveType(myLeaveType)
{

}

void LeaveGameAction::read(AbstractReplayReader &reader)
{
  leaveType = reader.readBYTE();
}

string LeaveGameAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", leaveType < 6 ? ReplayTool::pszLeaveType[leaveType] : "UNKNOWN");

  return str;
}
#include "SetReplaySpeedAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

SetReplaySpeedAction::SetReplaySpeedAction(PlayerID player)
  : GameAction(player, ReplayTool::Set_Replay_Speed)
  ,paused(false)
  ,speed(0)
  ,multiplier(0)
{
}

SetReplaySpeedAction::SetReplaySpeedAction(PlayerID player, bool myPaused, DWORD mySpeed, DWORD myMultiplier)
  : GameAction(player, ReplayTool::Set_Replay_Speed)
  ,paused(myPaused)
  ,speed(mySpeed)
  ,multiplier(myMultiplier)
{

}

void SetReplaySpeedAction::read(AbstractReplayReader &reader)
{
  paused = (reader.readBYTE() > 0 ? true : false);
  speed = reader.readDWORD();
  multiplier = reader.readDWORD();
}

string SetReplaySpeedAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s%s, ×%u", paused ? "PAUSED, " : "", speed < 7 ? ReplayTool::pszGameSpeed[speed] : "INVALID", multiplier);

  return str;
}
#include "SetSpeedAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

SetSpeedAction::SetSpeedAction(PlayerID player)
  : GameAction(player, ReplayTool::Set_Seed)
  ,speed(0)
{
}

SetSpeedAction::SetSpeedAction(PlayerID player, BYTE mySpeed)
  : GameAction(player, ReplayTool::Set_Seed)
  ,speed(mySpeed)
{

}

void SetSpeedAction::read(AbstractReplayReader &reader)
{
  speed = reader.readBYTE();
}

string SetSpeedAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", speed < 7 ? ReplayTool::pszGameSpeed[speed] : "INVALID");

  return str;
}
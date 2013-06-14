#include "StopAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

StopAction::StopAction(PlayerID player, bool myStop)
  : GameAction(player, (myStop ? ReplayTool::Stop : ReplayTool::Return))
  ,how(0)
  ,stop(myStop)
{
}

StopAction::StopAction(PlayerID player, BYTE myHow, bool myStop)
  : GameAction(player, (myStop ? ReplayTool::Stop : ReplayTool::Return))
  ,how(myHow)
  ,stop(myStop)
{

}

void StopAction::read(AbstractReplayReader &reader)
{
  how = reader.readBYTE();
}

string StopAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", how ? "Queued" : "");

  return str;
}
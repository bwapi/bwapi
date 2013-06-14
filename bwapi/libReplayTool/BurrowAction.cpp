#include "BurrowAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

BurrowAction::BurrowAction(PlayerID player, bool myUp)
  : GameAction(player, (myUp ? ReplayTool::Burrow_Up : ReplayTool::Burrow_Down))
  ,how(0)
  ,up(myUp)
{
}

BurrowAction::BurrowAction(PlayerID player, BYTE myHow, bool myUp)
  : GameAction(player, (myUp ? ReplayTool::Burrow_Up : ReplayTool::Burrow_Down))
  ,how(myHow)
  ,up(myUp)
{

}

void BurrowAction::read(AbstractReplayReader &reader)
{
  how = reader.readBYTE();
}

string BurrowAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", how ? "Queued" : "");

  return str;
}
#include "UnloadAllAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

UnloadAllAction::UnloadAllAction(PlayerID player, bool myUnloadAll)
  : GameAction(player, (myUnloadAll ? ReplayTool::Unload_All : ReplayTool::Hold_Position))
  ,how(0)
  ,unloadAll(myUnloadAll)
{
}

UnloadAllAction::UnloadAllAction(PlayerID player, BYTE myHow, bool myUnloadAll)
  : GameAction(player, (myUnloadAll ? ReplayTool::Unload_All : ReplayTool::Hold_Position))
  ,how(myHow)
  ,unloadAll(myUnloadAll)
{

}

void UnloadAllAction::read(AbstractReplayReader &reader)
{
  how = reader.readBYTE();
}

string UnloadAllAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", how ? "Queued" : "");

  return str;
}
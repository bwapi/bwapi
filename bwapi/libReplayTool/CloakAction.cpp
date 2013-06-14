#include "CloakAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

CloakAction::CloakAction(PlayerID player, bool myOn)
  : GameAction(player, (myOn ? ReplayTool::Cloak_On : ReplayTool::Cloak_Off))
  ,how(0)
  ,on(myOn)
{
}

CloakAction::CloakAction(PlayerID player, BYTE myHow, bool myOn)
  : GameAction(player, (myOn ? ReplayTool::Cloak_On : ReplayTool::Cloak_Off))
  ,how(myHow)
  ,on(myOn)
{

}

void CloakAction::read(AbstractReplayReader &reader)
{
  how = reader.readBYTE();
}

string CloakAction::toString() const
{
  string str = GameAction::toString();
  str += StrUtil::format("%s", how ? "Queued" : "");

  return str;
}
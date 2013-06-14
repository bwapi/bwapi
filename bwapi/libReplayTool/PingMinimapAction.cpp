#include "PingMinimapAction.h"
#include "DefaultActions.h"
#include "AbstractReplayReader.h"
#include "StrUtil.h"
#include <sstream>

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

PingMinimapAction::PingMinimapAction(PlayerID player)
  : GameAction(player, ReplayTool::Ping_Minimap)
  ,x(0)
  ,y(0)
{
}

PingMinimapAction::PingMinimapAction(PlayerID player, short myX, short myY)
  : GameAction(player, ReplayTool::Ping_Minimap)
  ,x(myX)
  ,y(myY)
{

}

void PingMinimapAction::read(AbstractReplayReader &reader)
{
  x = reader.readWORD();
  y = reader.readWORD();
}

string PingMinimapAction::toString() const
{
  ostringstream ss(GameAction::toString());
  ss << Position(x,y);
  return ss.str();
}
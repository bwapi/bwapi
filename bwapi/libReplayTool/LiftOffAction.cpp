#include "LiftOffAction.h"
#include "DefaultActions.h"
#include "StrUtil.h"
#include "AbstractReplayReader.h"
#include <sstream>

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

LiftOffAction::LiftOffAction(PlayerID player)
  : GameAction(player, ReplayTool::Lift_Off)
  ,x(0)
  ,y(0)
{
}

LiftOffAction::LiftOffAction(PlayerID player, short myX, short myY)
  : GameAction(player, ReplayTool::Lift_Off)
  ,x(myX)
  ,y(myY)
{

}

void LiftOffAction::read(AbstractReplayReader &reader)
{
  x = reader.readWORD();
  y = reader.readWORD();
}

string LiftOffAction::toString() const
{
  ostringstream ss(GameAction::toString());
  ss << Position(x,y);
  return ss.str();
}
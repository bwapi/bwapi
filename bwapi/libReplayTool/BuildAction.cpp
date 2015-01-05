#include "BuildAction.h"
#include "DefaultActions.h"
#include "AbstractReplayReader.h"
#include "StrUtil.h"
#include <sstream>

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

BuildAction::BuildAction(PlayerID player)
  : GameAction(player, ReplayTool::Placebox)
  ,x(0)
  ,y(0)
{
}

BuildAction::BuildAction(PlayerID player, BWAPI::Order myOrderType, short myX, short myY, BWAPI::UnitType myUnitType)
  : GameAction(player, ReplayTool::Placebox)
  ,orderType(myOrderType)
  ,x(myX)
  ,y(myY)
  ,unitType(myUnitType)
{

}

void BuildAction::read(AbstractReplayReader &reader)
{
  orderType = reader.readOrder();
  x = reader.readWORD();
  y = reader.readWORD();
  unitType = reader.readUnitType();
}

string BuildAction::toString() const
{
  ostringstream ss(GameAction::toString());
  ss << orderType << ", " << Position(x,y) << ", " << unitType;

  return ss.str();
}
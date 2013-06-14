#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class BuildAction : public GameAction
{
public:
  BuildAction(PlayerID player);
  BuildAction(PlayerID player, BWAPI::Order orderType, short x, short y, BWAPI::UnitType unitType);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BWAPI::Order& getOrderType() const { return orderType; }
  const short getX() const { return x; }
  const short getY() const { return y; }
  const BWAPI::UnitType& getUnitType() const { return unitType; }

  IMPLEMENT_FROM(BuildAction);

private:
  BWAPI::Order orderType;
  short x;
  short y;
  BWAPI::UnitType unitType;
};

END_REPLAY_TOOL
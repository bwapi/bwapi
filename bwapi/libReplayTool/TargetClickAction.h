#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class TargetClickAction : public GameAction
{
public:
  TargetClickAction(PlayerID player);
  TargetClickAction(PlayerID player, short myX, short myY, WORD mytargetID, BWAPI::UnitType myUnitType, BYTE myHow, BWAPI::Order myOrderType);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const short getX() const { return x; }
  const short getY() const { return y; }
  const WORD getTargetID() const { return targetID; }
  const BWAPI::UnitType getUnitType() const { return unitType; }
  const BYTE getHow() const { return how; }
  const BWAPI::Order getOrderType() const { return orderType; }

  IMPLEMENT_FROM(TargetClickAction);

private:
  short x;
  short y;
  WORD targetID;
  BWAPI::UnitType unitType;
  BYTE how;
  BWAPI::Order orderType;
};
END_REPLAY_TOOL
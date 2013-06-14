#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class RightClickAction : public GameAction
{
public:
  RightClickAction(PlayerID player);
  RightClickAction(PlayerID player, short myX, short myY, WORD mytargetID, BWAPI::UnitType myUnitType, BYTE myHow);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const short getX() const { return x; }
  const short getY() const { return y; }
  const WORD getTargetID() const { return targetID; }
  const BWAPI::UnitType getUnitType() const { return unitType; }
  const BYTE getHow() const { return how; }

  IMPLEMENT_FROM(RightClickAction);

private:
  short x;
  short y;
  WORD targetID;
  BWAPI::UnitType unitType;
  BYTE how;
};
END_REPLAY_TOOL
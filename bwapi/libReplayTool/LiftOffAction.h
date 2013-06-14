#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class LiftOffAction : public GameAction
{
public:
  LiftOffAction(PlayerID player);
  LiftOffAction(PlayerID player, short x, short y);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const short getX() const { return x; }
  const short getY() const { return y; }

  IMPLEMENT_FROM(LiftOffAction);

private:
  short x;
  short y;
};
END_REPLAY_TOOL
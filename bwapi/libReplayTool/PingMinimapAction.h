#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class PingMinimapAction : public GameAction
{
public:
  PingMinimapAction(PlayerID player);
  PingMinimapAction(PlayerID player, short x, short y);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const short getX() const { return x; }
  const short getY() const { return y; }

  IMPLEMENT_FROM(PingMinimapAction);

private:
  short x;
  short y;
};
END_REPLAY_TOOL
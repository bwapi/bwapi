#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class BurrowAction : public GameAction
{
public:
  BurrowAction(PlayerID player, bool myUp);
  BurrowAction(PlayerID player, BYTE myHow, bool myUp);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getHow() const { return how; }

  IMPLEMENT_FROM_WITH_1_PARAM(BurrowAction, up);

private:
  BYTE how;
  bool up;
};
END_REPLAY_TOOL
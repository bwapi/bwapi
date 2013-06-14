#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class StopAction : public GameAction
{
public:
  StopAction(PlayerID player, bool stop);
  StopAction(PlayerID player, BYTE myHow, bool stop);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getHow() const { return how; }

  IMPLEMENT_FROM_WITH_1_PARAM(StopAction, stop);

private:
  BYTE how;
  bool stop;
};
END_REPLAY_TOOL
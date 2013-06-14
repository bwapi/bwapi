#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class TankSiegeAction : public GameAction
{
public:
  TankSiegeAction(PlayerID player, bool siege);
  TankSiegeAction(PlayerID player, BYTE myHow, bool siege);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getHow() const { return how; }

  IMPLEMENT_FROM_WITH_1_PARAM(TankSiegeAction, siege);

private:
  BYTE how;
  bool siege;
};
END_REPLAY_TOOL
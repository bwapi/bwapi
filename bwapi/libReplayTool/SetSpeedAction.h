#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class SetSpeedAction : public GameAction
{
public:
  SetSpeedAction(PlayerID player);
  SetSpeedAction(PlayerID player, BYTE mySpeed);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getSpeed() const { return speed; }

  IMPLEMENT_FROM(SetSpeedAction);

private:
  BYTE speed;
};

END_REPLAY_TOOL
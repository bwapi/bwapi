#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class SetReplaySpeedAction : public GameAction
{
public:
  SetReplaySpeedAction(PlayerID player);
  SetReplaySpeedAction(PlayerID player, bool myPaused, DWORD mySpeed, DWORD myMultiplier);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const bool getPaused() const { return paused; }
  const DWORD getSpeed() const { return speed; }
  const DWORD getMultiplier() const { return multiplier; }

  IMPLEMENT_FROM(SetReplaySpeedAction);

private:
  bool paused;
  DWORD speed;
  DWORD multiplier;
};
END_REPLAY_TOOL
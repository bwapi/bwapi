#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class LatencyAction : public GameAction
{
public:
  LatencyAction(PlayerID player);
  LatencyAction(PlayerID player, BYTE myLatency);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getLatency() const { return latency; }

  IMPLEMENT_FROM(LatencyAction);

private:
  BYTE latency;
};
END_REPLAY_TOOL
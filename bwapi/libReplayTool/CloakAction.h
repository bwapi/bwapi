#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class CloakAction : public GameAction
{
public:
  CloakAction(PlayerID player, bool on);
  CloakAction(PlayerID player, BYTE myHow, bool on);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getHow() const { return how; }

  IMPLEMENT_FROM_WITH_1_PARAM(CloakAction, on);

private:
  BYTE how;
  bool on;
};
END_REPLAY_TOOL
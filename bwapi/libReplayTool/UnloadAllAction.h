#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class UnloadAllAction : public GameAction
{
public:
  UnloadAllAction(PlayerID player, bool myUnloadAll);
  UnloadAllAction(PlayerID player, BYTE myHow, bool myUnloadAll);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getHow() const { return how; }

  IMPLEMENT_FROM_WITH_1_PARAM(UnloadAllAction, unloadAll);

private:
  BYTE how;
  bool unloadAll;
};
END_REPLAY_TOOL
#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class ChatReplayAction : public GameAction
{
public:
  ChatReplayAction(PlayerID player);
  ChatReplayAction(PlayerID player, PlayerID otherPlayer, const std::string& msg);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const PlayerID& getOtherPlayer() const { return otherPlayer; }
  const std::string& getMsg() const { return msg; }

  IMPLEMENT_FROM(ChatReplayAction);

  static const unsigned MAX_MSG_SIZE = 80;

private:
  PlayerID otherPlayer;
  std::string msg;
};

END_REPLAY_TOOL
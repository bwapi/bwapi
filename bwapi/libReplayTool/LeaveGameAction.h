#pragma once

#include "GameAction.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class LeaveGameAction : public GameAction
{
public:
  LeaveGameAction(PlayerID player);
  LeaveGameAction(PlayerID player, BYTE myLeaveType);

  void read(AbstractReplayReader &reader);
  std::string toString() const;
  const BYTE getLeaveType() const { return leaveType; }

  IMPLEMENT_FROM(LeaveGameAction);

private:
  BYTE leaveType;
};
END_REPLAY_TOOL
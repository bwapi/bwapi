#include "ChatReplayAction.h"
#include "DefaultActions.h"
#include "AbstractReplayReader.h"
#include "StrUtil.h"

using namespace std;
using namespace BWAPI;
using namespace ReplayTool;

ChatReplayAction::ChatReplayAction(PlayerID player)
  :GameAction(player, ReplayTool::Chat_Replay)
  ,otherPlayer(0)
{
}

ChatReplayAction::ChatReplayAction(PlayerID player, PlayerID myOtherPlayer, const std::string& myMsg)
  : GameAction(player, ReplayTool::Chat_Replay)
  ,otherPlayer(myOtherPlayer)
  ,msg(myMsg)
{
}

void ChatReplayAction::read(AbstractReplayReader &reader)
{
  otherPlayer = reader.readBYTE();
  char buffer[MAX_MSG_SIZE];

  msg.clear();

  for (unsigned i = 0; i < MAX_MSG_SIZE; ++i)
  {
    buffer[i] = reader.readBYTE();
  }

  msg = buffer;
}

string ChatReplayAction::toString() const
{
  stringstream ss;
  ss << GameAction::toString();
  ss << " " << otherPlayer << ", " << msg;

  return ss.str();
}
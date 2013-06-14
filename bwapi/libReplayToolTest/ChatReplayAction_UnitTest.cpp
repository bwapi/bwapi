#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "ChatReplayAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Chat: %u, %s"
#define DEFAULT_PLAYER  (1)
#define OTHER_PLAYER    (2)
#define CHAT_MSG        "Hello World!"

ReplayTool::GameAction* CreateChatReplayAction()
{ 
  return new ChatReplayAction(DEFAULT_PLAYER, OTHER_PLAYER, CHAT_MSG);
}

class ChatReplayActionTest : public AbstractActionTest<CreateChatReplayAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Chat: 2, Hello World!";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    ChatReplayAction chatAction(DEFAULT_PLAYER);

    // An explicit scope is required to mark the gmock code that runs in sequence
    {
      InSequence seq;

      EXPECT_CALL(reader, readBYTE()).
        WillOnce(testing::Return(OTHER_PLAYER));

      const char buffer[ChatReplayAction::MAX_MSG_SIZE] = CHAT_MSG;

      for (unsigned i = 0; i < ChatReplayAction::MAX_MSG_SIZE; ++i)
      {
        EXPECT_CALL(reader, readBYTE()).
          WillOnce(testing::Return(buffer[i])).
          RetiresOnSaturation();
      }
    }

    chatAction.read(reader);

    {
      InSequence seq;

      EXPECT_EQ(OTHER_PLAYER, chatAction.getOtherPlayer());
      EXPECT_STREQ((const char*)CHAT_MSG, chatAction.getMsg().c_str());
    }
  }
};

TEST_F(ChatReplayActionTest, Ctor)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Chat_Replay, action->getAction());
}

TEST_F(ChatReplayActionTest, read)
{
  testRead();
}
#include "MockReplayReader.h"
#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "DefaultActions.h"
#include "SaveGameAction.h"
#include "AbstractActionTest.h"
#include "StrUtil.h"

using namespace ReplayTool;
using namespace BWAPI;
using namespace std;
using namespace testing;

#define STR_FORMAT      "(P%d) Save Game: 0x%08X, %s"
#define DEFAULT_PLAYER  (1)
#define SAVE_PATH       "C:\\MySave.rep"
#define SAVE_INFO       (0x8)

ReplayTool::GameAction* CreateSaveAction() { return new SaveGameAction(DEFAULT_PLAYER, SAVE_INFO, SAVE_PATH); }

class SaveActionTest : public AbstractActionTest<CreateSaveAction>
{
protected:
  void testToString()
  {
    string expected = "(P1) Save Game: 0x00000008, C:\\MySave.rep";
    string actual = action->toString();

    EXPECT_STREQ(expected.c_str(), actual.c_str());
  }

  void testRead()
  {
    MockReplayReader reader;
    SaveGameAction svAction(DEFAULT_PLAYER);

    InSequence seq;
    {
      EXPECT_CALL(reader, readDWORD()).
        WillOnce(testing::Return(SAVE_INFO));
      EXPECT_CALL(reader, readCString()).
        WillOnce(testing::Return(SAVE_PATH));
    }

    svAction.read(reader);

    EXPECT_EQ(SAVE_PATH, svAction.getSavePath());
    EXPECT_EQ(SAVE_INFO, svAction.getSaveInfo());
  }
};

TEST_F(SaveActionTest, SaveAction)
{
  EXPECT_EQ(DEFAULT_PLAYER, action->getPlayer());
  EXPECT_EQ(ReplayTool::Save_Game, action->getAction());
}

TEST_F(SaveActionTest, read)
{
  testRead();
}

TEST_F(SaveActionTest, toString)
{
  testToString();
}
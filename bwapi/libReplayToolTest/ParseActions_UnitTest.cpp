#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include <Windows.h>
#include <string>
#include <fstream>
#include "StrUtil.h"

using namespace std;
using namespace testing;
using namespace ReplayTool;

#define TEST_DATA_DIR                     "TestData"
#define ACTUAL_REPLAY_TRACES_DIR_SEARCH   TEST_DATA_DIR##"\\ActualReplayTraces\\*"
#define ACTUAL_REPLAY_TRACES_DIR          TEST_DATA_DIR##"\\ActualReplayTraces"
#define EXPECTED_REPLAY_TRACES_DIR        TEST_DATA_DIR##"\\ExpectedReplayTraces"
#define REPLAY_TRACES_GENERATOR           TEST_DATA_DIR##"\\replay_trace_generator.bat "
#define REPLAYS_SOURCE_DIR                TEST_DATA_DIR##"\\Replays"
#define ACTUAL_REPLAY_TOOL_EXE            TEST_DATA_DIR##"\\Replay_Tool_Actual.exe"
#define EXPECTED_REPLAY_TOOL_EXE          TEST_DATA_DIR##"\\Replay_Tool_Expected.exe"
#define REPLAY_TRACES_CLEANUP_CMD         "del /s %s\\*.* /q"
#define REPLAY_TRACES_GENERATOR_CMD       "%s %s %s %s"

class ParseActionsTest : public Test
{
public:
  void SetUp()
  {
    string delCmd = StrUtil::format(REPLAY_TRACES_CLEANUP_CMD, ACTUAL_REPLAY_TRACES_DIR);
    string genCmd = StrUtil::format(REPLAY_TRACES_GENERATOR_CMD,
      REPLAY_TRACES_GENERATOR,
      ACTUAL_REPLAY_TOOL_EXE,
      REPLAYS_SOURCE_DIR,
      ACTUAL_REPLAY_TRACES_DIR);

    system(delCmd.c_str());
    system(genCmd.c_str());
  }

  void TearDown()
  {
    string delCmd = StrUtil::format(REPLAY_TRACES_CLEANUP_CMD, ACTUAL_REPLAY_TRACES_DIR);
    system(delCmd.c_str());
  }
};

TEST_F(ParseActionsTest, WholeTest)
{
  string actualReplayFilepath;
  string expectedReplayFilepath;
  string actualReplayLine;
  string expectedReplayLine;
  fstream expectedReplayFile;
  fstream actualReplayFile;
  WIN32_FIND_DATA findData;
  unsigned replayLineNum;

  HANDLE hFind = FindFirstFile(ACTUAL_REPLAY_TRACES_DIR_SEARCH, &findData);

  ASSERT_NE(hFind, INVALID_HANDLE_VALUE);

  do 
  {
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      continue;

    actualReplayFilepath = ACTUAL_REPLAY_TRACES_DIR;
    actualReplayFilepath += '\\';
    actualReplayFilepath += findData.cFileName;

    expectedReplayFilepath = EXPECTED_REPLAY_TRACES_DIR;
    expectedReplayFilepath += '\\';
    expectedReplayFilepath += findData.cFileName;

    expectedReplayFile.open(expectedReplayFilepath.c_str(), ios::in);
    actualReplayFile.open(actualReplayFilepath.c_str(), ios::in);

    if (!expectedReplayFile.is_open())
    {
      RecordProperty("Inconclusive", expectedReplayFilepath.c_str());
      actualReplayFile.close();
      continue;
    }

    if (!actualReplayFile.is_open())
    {
      RecordProperty("Inconclusive", actualReplayFilepath.c_str());
      expectedReplayFile.close();
      continue;
    }
    
    replayLineNum = 0;

    while (getline(expectedReplayFile, expectedReplayLine) && 
      getline(actualReplayFile, actualReplayLine))
    {
      ++replayLineNum;
      EXPECT_STREQ(expectedReplayLine.c_str(), actualReplayLine.c_str()) <<
        "Failed in replay:" << findData.cFileName << " @" << replayLineNum;
    }

    expectedReplayFile.close();
    actualReplayFile.close();

  } while (FindNextFile(hFind, &findData));

  FindClose(hFind);
}
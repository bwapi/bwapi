#pragma 

#include "ReplayToolDefs.h"

START_REPLAY_TOOL

class ParseReplayParams
{
public:
  ParseReplayParams()
    :option('\0')
  {
    replayPath[0] = outRepoPath[0] = '\0';
  }

  const char* getReplayPath() const { return replayPath; }
  void setReplayPath(const char* val) { strcpy_s(replayPath, MAX_PATH, val); }

  const char* getOutRepoPath() const { return outRepoPath; }
  void setOutRepoPath(const char* val) { strcpy_s(outRepoPath, MAX_PATH, val); }

  char getOption() const { return option; }
  void setOption(char val) { option = val; }

private:
  char  replayPath[MAX_PATH];
  char  outRepoPath[MAX_PATH];
  char  option;
};

END_REPLAY_TOOL

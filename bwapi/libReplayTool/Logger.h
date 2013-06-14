#pragma once

#include <cstdio>
#include "ReplayToolDefs.h"

START_REPLAY_TOOL

class Logger
{
public:
  Logger(const char *filename);
  ~Logger();
  void writeLine(const char *pszFormat, ...);

private:
  FILE* file;
};

END_REPLAY_TOOL
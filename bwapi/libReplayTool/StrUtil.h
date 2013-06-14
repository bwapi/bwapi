#pragma once

#include "ReplayToolDefs.h"
#include <string>

START_REPLAY_TOOL

class StrUtil
{
public:
  static std::string format(const char* format, ...);
};

END_REPLAY_TOOL
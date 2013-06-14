#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

struct CheatActionParamFormatter
{
  static std::string toString(const DWORD& param) { return StrUtil::format("0x%08X", param); }
};

class CheatAction : public GenericAction1P<DWORD, ReplayTool::Cheat, CheatActionParamFormatter>
{
public:
  IMPLEMENT_CTOR1(CheatAction, DWORD);
  IMPLEMENT_FROM(CheatAction);

  DWORD getFlags() const { return getParam(); }
};

END_REPLAY_TOOL
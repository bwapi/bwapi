#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

struct SetAlliesActionParamFormatter
{
  static std::string toString(const DWORD& param) { return StrUtil::format("0x%08X", param); }
};

class SetAlliesAction : public GenericAction1P<DWORD, ReplayTool::Set_Allies, SetAlliesActionParamFormatter>
{
public:
  IMPLEMENT_CTOR1(SetAlliesAction, DWORD);
  IMPLEMENT_FROM(SetAlliesAction);

  DWORD getData() const { return getParam(); }
};

END_REPLAY_TOOL
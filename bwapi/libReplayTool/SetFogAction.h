#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

struct SetFogActionParamFormatter
{
  static std::string toString(const WORD& param) { return StrUtil::format("%04X", param); }
};

class SetFogAction : public GenericAction1P<WORD, ReplayTool::Set_Fog, SetFogActionParamFormatter>
{
public:
  IMPLEMENT_CTOR1(SetFogAction, WORD);
  IMPLEMENT_FROM(SetFogAction);

  const WORD& getFlags() const { return getParam(); }
};

END_REPLAY_TOOL
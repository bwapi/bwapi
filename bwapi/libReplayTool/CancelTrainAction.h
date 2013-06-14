#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

struct CancelTrainActionParamFormatter
{
  static std::string toString(const WORD& param) { return StrUtil::format("(%u, 0x%02X)", param & 0x7FF, param >> 12); }
};

class CancelTrainAction : public GenericAction1P<WORD, ReplayTool::Cancel_Train, CancelTrainActionParamFormatter>
{
public:
  IMPLEMENT_CTOR1(CancelTrainAction, WORD);
  IMPLEMENT_FROM(CancelTrainAction);

  UnitID getUnitID() const { return (UnitID)(getParam() & 0x7FF); }
  BYTE getData() const { return (BYTE)(getParam() >> 12); }
};

END_REPLAY_TOOL
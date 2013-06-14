#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

typedef BYTE GroupType;
typedef BYTE TGroupNum;

struct GroupUnitsActionParamFormatter
{
  static std::string toString(GroupType groupType, TGroupNum groupNum) 
  {
    return StrUtil::format("%s, %u", groupType < 3 ? ReplayTool::pszGroupType[groupType] : "INVALID", groupNum); 
  }
};

class GroupUnitsAction : public GenericAction2P<GroupType, TGroupNum, ReplayTool::Group_Units, GroupUnitsActionParamFormatter>
{
public:
  IMPLEMENT_CTOR2(GroupUnitsAction, GroupType, TGroupNum);
  IMPLEMENT_FROM(GroupUnitsAction);
  GroupType getGroupType() const { return getParam1(); }
  TGroupNum getGroupNum() const { return getParam2(); }
};

END_REPLAY_TOOL
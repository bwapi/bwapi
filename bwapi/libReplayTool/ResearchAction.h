#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class ResearchAction : public GenericAction1P<BWAPI::TechType, ReplayTool::Research>
{
public:
  IMPLEMENT_CTOR1(ResearchAction, BWAPI::TechType);
  IMPLEMENT_FROM(ResearchAction);
  const BWAPI::TechType& getResearch() const { return getParam(); }
};

END_REPLAY_TOOL
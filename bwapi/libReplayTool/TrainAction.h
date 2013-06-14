#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class TrainAction : public GenericAction1P<BWAPI::UnitType, ReplayTool::Train>
{
public:
  IMPLEMENT_CTOR1(TrainAction, BWAPI::UnitType);
  IMPLEMENT_FROM(TrainAction);

  const BWAPI::UnitType& getTraineeType() const { return getParam(); }
};

END_REPLAY_TOOL
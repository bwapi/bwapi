#pragma once

#include "GenericAction1P.h"
#include "DefaultActions.h"

START_REPLAY_TOOL

class AbstractReplayReader;

class BuildingMorphAction : public GenericAction1P<BWAPI::UnitType, ReplayTool::Building_Morph>
{
public:
  IMPLEMENT_CTOR1(BuildingMorphAction, BWAPI::UnitType);
  IMPLEMENT_FROM(BuildingMorphAction);

  const BWAPI::UnitType& getTarget() const { return getParam(); }
};

class UnitMorphAction : public GenericAction1P<BWAPI::UnitType, ReplayTool::Unit_Morph>
{
public:
  IMPLEMENT_CTOR1(UnitMorphAction, BWAPI::UnitType);
  IMPLEMENT_FROM(UnitMorphAction);

  const BWAPI::UnitType& getTarget() const { return getParam(); }
};

END_REPLAY_TOOL
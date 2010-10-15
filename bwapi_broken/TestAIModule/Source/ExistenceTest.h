#pragma once
#include <BWAPI.h>
#include "TestModule.h"
class ExistenceTest : public TestModule
{
  public:
  virtual void onStart();
  virtual void onFrame();
  virtual void onUnitCreate(BWAPI::Unit* unit);
  virtual void onUnitDestroy(BWAPI::Unit* unit);
};
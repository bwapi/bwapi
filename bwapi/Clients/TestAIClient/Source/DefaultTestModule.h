#pragma once
#include <BWAPI.h>
#include "TestModule.h"
class DefaultTestModule : public TestModule
{
  public:
  virtual void onStart();
  virtual void onFrame();
};
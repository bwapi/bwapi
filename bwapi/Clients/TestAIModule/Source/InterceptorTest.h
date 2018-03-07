#pragma once
#include <BWAPI.h>
#include "TestModule.h"
class InterceptorTest : public TestModule
{
  public:
  virtual void onStart();
  virtual void onFrame();
};
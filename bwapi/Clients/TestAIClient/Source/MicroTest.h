#pragma once
#include <BWAPI.h>
class MicroTest : public BWAPI::AIModule
{
  public:
  virtual void onStart();
  virtual void onFrame();
};
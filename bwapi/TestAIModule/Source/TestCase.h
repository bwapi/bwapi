#pragma once
#include "BWAssert.h"
class TestCase
{
  public:
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() = 0; 
};
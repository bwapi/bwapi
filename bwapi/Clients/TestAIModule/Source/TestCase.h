#pragma once
#include "BWAssert.h"
class TestCase
{
  public:
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void stop() = 0;
    bool isRunning() const {return running;}
    bool isFailed() const {return fail;}
    bool running;
    bool fail;

};
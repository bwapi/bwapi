#include "FrameTimer.h"

// constructors

FrameTimer::FrameTimer(int newperiod)
{
	period = newperiod;
	framesLeft = period;
	running = true;
	gate = false;
}

FrameTimer::~FrameTimer()
{
}

void FrameTimer::setPeriod(int newperiod)
{
	period = newperiod;
	if(framesLeft > period)
		framesLeft = period;
}

bool FrameTimer::tick()
{
	framesLeft --;
	if(framesLeft <= 0)
	{
		framesLeft = period;
		return gate = true;
	}
	return gate = false;
}

void FrameTimer::stop()
{
	running = false;
}

void FrameTimer::start()
{
	running = true;
}

void FrameTimer::restart()
{
	running = true;
	framesLeft = period;
}

bool FrameTimer::isRunning()
{
	return running;
}

bool FrameTimer::isGate()
{
	return gate;
}
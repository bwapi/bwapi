#pragma once
/**
 *  returns true every n-th frame
 */

class FrameTimer
{

	// constructors
public:
	FrameTimer(int period = 1);
	~FrameTimer();

	// state
private:
	int framesLeft;
	int period;
	bool running;
	bool gate;

	// methods
public:
	void setPeriod(int period);
	bool tick();
	void stop();
	void start();
	void restart();
	bool isRunning();
	bool isGate();
};

#pragma once

class Settings
{
public:
	Settings() {};
	virtual ~Settings() {};

	virtual void init() = 0;
	virtual void release() = 0;

	virtual const char* getHostIPString() = 0;
	virtual const char* getHostPortString() = 0;
	virtual const char* getLocalPortString() = 0;
	virtual void setStatusString(const char* statusText) = 0;

	static Settings* getSettings();
};


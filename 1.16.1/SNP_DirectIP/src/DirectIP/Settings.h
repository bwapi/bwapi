#pragma once

#include <memory>

class Settings
{
public:
	Settings() {};
	virtual ~Settings() {};

	virtual void init() = 0;
	virtual void release() = 0;

	virtual const char* getHostIPString() const = 0;
	virtual const uint16_t getHostPort() const = 0;
	virtual const uint16_t getLocalPort() const = 0;
	virtual void setStatusString(const char* statusText) = 0;

	static std::unique_ptr<Settings> getSettings();
};


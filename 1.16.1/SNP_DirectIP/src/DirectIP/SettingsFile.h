#pragma once

#include "Settings.h"

#include <string>
#include <fstream>

class SettingsFile : public Settings
{
private:
	std::string hostIp;
	std::string hostPort;
	std::string localPort;

public:
	SettingsFile(std::ifstream& settingsFile);
	virtual ~SettingsFile();

	void init() override;
	void release() override;

	const char* getHostIPString() const override;
	const u_short getHostPort() const override;
	const u_short getLocalPort() const override;
	void setStatusString(const char* statusText) override;
};

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

	const char* getHostIPString() override;
	const char* getHostPortString() override;
	const char* getLocalPortString() override;
	void setStatusString(const char* statusText) override;
};
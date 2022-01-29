#pragma once
#include "Settings.h"

#include <string>
#include <windows.h>

class SettingsEnvironment :
    public Settings
{

private:
	bool ok;
	std::string hostIp;
	std::string hostPort;
	std::string localPort;

	void FetchEnvironmentVariable(LPCTSTR name, std::string& res);

public:
	SettingsEnvironment();
	virtual ~SettingsEnvironment();

	void init() override;
	void release() override;

	const char* getHostIPString() const override;
	const u_short getHostPort() const override;
	const u_short getLocalPort() const override;
	void setStatusString(const char* statusText) override;

	bool isOk() const;
};

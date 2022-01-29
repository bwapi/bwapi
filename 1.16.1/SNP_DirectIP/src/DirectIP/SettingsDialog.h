#pragma once

#include "Settings.h"

#include <windows.h>
#include "../../resource.h"


class SettingsDialog : public Settings
{
private:
	HANDLE hdlgThread;


	void showSettingsDialog();
	void hideSettingsDialog();

public:
	SettingsDialog();
	virtual ~SettingsDialog();

	void init() override;
	void release() override;

	const char* getHostIPString() const override;
	const u_short getHostPort() const override;
	const u_short getLocalPort() const override;
	void setStatusString(const char* statusText) override;
};

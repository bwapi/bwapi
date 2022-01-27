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

	const char* getHostIPString() override;
	const char* getHostPortString() override;
	const char* getLocalPortString() override;
	void setStatusString(const char* statusText) override;
};

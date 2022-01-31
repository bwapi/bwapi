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
	const uint16_t getHostPort() const override;
	const uint16_t getLocalPort() const override;
	void setStatusString(const char* statusText) override;
};

#include "Settings.h"

#include <iostream>
#include <fstream>

#include "SettingsFile.h"
#include "SettingsDialog.h"
#include "SettingsEnvironment.h"

Settings* Settings::getSettings() {
	SettingsEnvironment* envSettings = new SettingsEnvironment();
	if (envSettings->isOk()) {
		return envSettings;
	}
	else {
		delete envSettings;
	}

	std::ifstream settingsFile("directip.conf", std::ifstream::in);
	if (settingsFile.good()) {
		return new SettingsFile(settingsFile);
	}
	else {
		return new SettingsDialog();
	}
}

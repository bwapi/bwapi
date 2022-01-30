#include "Settings.h"

#include <iostream>
#include <fstream>

#include "SettingsFile.h"
#include "SettingsDialog.h"
#include "SettingsEnvironment.h"

std::unique_ptr<Settings> Settings::getSettings() {
	std::unique_ptr<SettingsEnvironment> envSettings = std::make_unique<SettingsEnvironment>();
	if (envSettings->isOk()) {
		return envSettings;
	}

	std::ifstream settingsFile("directip.conf", std::ifstream::in);
	if (settingsFile.good()) {
		return std::make_unique<SettingsFile>(settingsFile);
	}
	else {
		return std::make_unique<SettingsDialog>();
	}
}

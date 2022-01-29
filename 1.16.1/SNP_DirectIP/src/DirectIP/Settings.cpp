#include "Settings.h"

#include <iostream>
#include <fstream>

#include "SettingsFile.h"
#include "SettingsDialog.h"
#include "SettingsEnvironment.h"

std::unique_ptr<Settings> Settings::getSettings() {
	unique_ptr<SettingsEnvironment> envSettings = make_unique<SettingsEnvironment>();
	if (envSettings->isOk()) {
		return envSettings;
	}
	else {
		delete envSettings;
	}

	std::ifstream settingsFile("directip.conf", std::ifstream::in);
	if (settingsFile.good()) {
		return make_unique<SettingsFile>(settingsFile);
	}
	else {
		return make_unique<SettingsDialog>();
	}
}

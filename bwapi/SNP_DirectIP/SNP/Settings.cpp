#include "Settings.h"

#include <iostream>
#include <fstream>

#include "SettingsFile.h"
#include "SettingsDialog.h"

Settings* Settings::getSettings() {
	std::ifstream settingsFile("directip.conf", std::ifstream::in);
	if (settingsFile.good()) {
		return new SettingsFile(settingsFile);
	}
	else {
		return new SettingsDialog();
	}
}
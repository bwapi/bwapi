#include "SettingsFile.h"

SettingsFile::SettingsFile(std::ifstream &settingsFile) {
	settingsFile >> hostIp >> hostPort >> localPort;
}
SettingsFile::~SettingsFile() {}

void SettingsFile::init() {}
void SettingsFile::release() {}

const char* SettingsFile::getHostIPString() {
	return hostIp.c_str();
}
const char* SettingsFile::getHostPortString() {
	return hostPort.c_str();
}
const char* SettingsFile::getLocalPortString() {
	return localPort.c_str();
}
void SettingsFile::setStatusString(const char* statusText) {

}

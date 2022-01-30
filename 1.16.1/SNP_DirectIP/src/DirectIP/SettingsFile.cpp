#include "SettingsFile.h"

SettingsFile::SettingsFile(std::ifstream &settingsFile) {
	settingsFile >> hostIp >> hostPort >> localPort;
}
SettingsFile::~SettingsFile() {}

void SettingsFile::init() {}
void SettingsFile::release() {}

const char* SettingsFile::getHostIPString() const {
	return hostIp.c_str();
}
const uint16_t SettingsFile::getHostPort() const {
	return std::stoul(hostPort);
}
const uint16_t SettingsFile::getLocalPort() const {
	return std::stoul(localPort);
}
void SettingsFile::setStatusString(const char* statusText) {

}

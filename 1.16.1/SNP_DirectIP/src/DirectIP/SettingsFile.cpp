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
const u_short SettingsFile::getHostPort() const {
	return std::stoul(hostPort);
}
const u_short SettingsFile::getLocalPort() const {
	return std::stoul(localPort);
}
void SettingsFile::setStatusString(const char* statusText) {

}

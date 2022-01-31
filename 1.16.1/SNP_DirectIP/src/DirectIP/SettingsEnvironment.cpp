#include "SettingsEnvironment.h"

void SettingsEnvironment::FetchEnvironmentVariable(LPCTSTR name, std::string& res) {
	DWORD bufferSize = GetEnvironmentVariable(name, nullptr, 0);
	if (bufferSize > 0) {
		res.resize(bufferSize + 1);
		bufferSize = GetEnvironmentVariable(name, res.data(), res.size());
		res.pop_back(); // Get rid of null terminator
	} else {
		ok = false;
		res = "";
	}
}

SettingsEnvironment::SettingsEnvironment() : ok(true) {
	FetchEnvironmentVariable("BW_HOST_IP", hostIp);
	FetchEnvironmentVariable("BW_HOST_PORT", hostPort);
	FetchEnvironmentVariable("BW_LOCAL_PORT", localPort);
}
SettingsEnvironment::~SettingsEnvironment() {}


void SettingsEnvironment::init() {}
void SettingsEnvironment::release() {}

const char* SettingsEnvironment::getHostIPString() const {
	return hostIp.c_str();
}
const uint16_t SettingsEnvironment::getHostPort() const {
	return std::stoul(hostPort);
}
const uint16_t SettingsEnvironment::getLocalPort() const {
	return std::stoul(localPort);
}
void SettingsEnvironment::setStatusString(const char* statusText) {

}

bool SettingsEnvironment::isOk() const {
	return ok;
}

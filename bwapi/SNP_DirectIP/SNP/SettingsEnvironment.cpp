#include "SettingsEnvironment.h"

void SettingsEnvironment::FetchEnvironmentVariable(LPCTSTR name, std::string& res) {
	DWORD bufferSize = 0xFFFF; //Limit according to http://msdn.microsoft.com/en-us/library/ms683188.aspx
	res.resize(bufferSize);
	bufferSize = GetEnvironmentVariableA(name, &res[0], bufferSize);
	if (!bufferSize) {
		ok = false;
		res = "";
		return;
	}
	res.resize(bufferSize);
}

SettingsEnvironment::SettingsEnvironment() : ok(true) {
	FetchEnvironmentVariable("BW_HOST_IP", hostIp);
	FetchEnvironmentVariable("BW_HOST_PORT", hostPort);
	FetchEnvironmentVariable("BW_LOCAL_PORT", localPort);
}
SettingsEnvironment::~SettingsEnvironment() {}


void SettingsEnvironment::init() {}
void SettingsEnvironment::release() {}

const char* SettingsEnvironment::getHostIPString() {
	return hostIp.c_str();
}
const char* SettingsEnvironment::getHostPortString() {
	return hostPort.c_str();
}
const char* SettingsEnvironment::getLocalPortString() {
	return localPort.c_str();
}
void SettingsEnvironment::setStatusString(const char* statusText) {

}

bool SettingsEnvironment::isOk() const {
	return ok;
}

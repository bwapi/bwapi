#pragma once

#include <string>

#ifdef _DEBUG
#define MODULE "BWAPId.dll"
#else
#define MODULE "BWAPI.dll"
#endif

// Common functions
std::string GetPluginName();
std::string GetBWAPIDataDirectory();
std::string GetBWAPITarget();

std::string GetRegString(const char *pszSubKey, const char *pszValueName);
bool BWAPIError(const char *format, ...);



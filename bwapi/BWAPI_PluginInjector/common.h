#include <string>

#include "../svnrev.h"
#include "../starcraftver.h"

#ifdef _DEBUG
#define MODULE "BWAPId.dll"
#else
#define MODULE "BWAPI.dll"
#endif

extern const char *pszPluginName;

// Common functions
std::string GetBWAPIDataDirectory();
std::string GetBWAPITarget();

std::string GetRegString(const char *pszSubKey, const char *pszValueName);
bool BWAPIError(const char *format, ...);



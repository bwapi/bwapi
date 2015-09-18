#pragma once
#include <string>
#include <storm.h>
#include "DLLMain.h"

// Functions
std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
std::string LoadConfigStringUCase (const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault = 0);

void InitPrimaryConfig();

// The starcraft instance's root directory
inline const std::string& installPath()
{
  static std::string path;
  if (path.empty())
  {
    char buffer[MAX_PATH];
    if (GetModuleFileNameA(NULL, buffer, MAX_PATH))
    {
      path = std::string(buffer);
      path = path.substr(0, path.find_last_of("\\/")+1);
    }
    else
      BWAPIError("Error getting starcraft's root directory via GetModuleFileNameA()");
  }
  return path;
}
// The config directory
inline const std::string& configDir()
{
  static const std::string path = installPath() + "bwapi-data\\";
  return path;
}
// The bwapi.ini file in the config directory or environment variable BWAPI_CONFIG_INI
inline const std::string& configPath()
{
  char* v;
  static const std::string path = (v=std::getenv("BWAPI_CONFIG_INI")) ? v : configDir()+"bwapi.ini";
  return path;
}
extern std::string screenshotFmt;

extern bool isCorrectVersion;
extern bool showWarn;
extern bool serverEnabled;
extern unsigned gdwProcNum;

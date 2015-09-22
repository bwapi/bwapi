#pragma once
#include <string>
#include <storm.h>
#include "DLLMain.h"

// Functions
std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
std::string LoadConfigStringUCase (const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault = 0);

void InitPrimaryConfig();

// Data
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
inline const std::string& configPath()
{
  static const std::string path = installPath() + "bwapi-data\\bwapi.ini";
  return path;
}
extern std::string screenshotFmt;

extern bool isCorrectVersion;
extern bool showWarn;
extern bool serverEnabled;
extern unsigned gdwProcNum;


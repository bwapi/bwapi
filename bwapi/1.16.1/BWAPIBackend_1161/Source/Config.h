#pragma once
#include <string>
#include <filesystem>
#include <storm.h>
#include "DLLMain.h"

// Functions
std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
std::string LoadConfigStringUCase (const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault = 0);
void WriteConfig(const char *pszKey, const char *pszItem, const std::string& value);
void WriteConfig(const char *pszKey, const char *pszItem, int value);

void InitPrimaryConfig();

// The starcraft instance's root directory
inline const std::string& installPath()
{
  static std::string path;
  if (path.empty())
  {
    char buffer[MAX_PATH];
    if (GetModuleFileNameA(NULL, buffer, MAX_PATH)) //get .exe path
      path = std::filesystem::path(buffer).parent_path().string() + "\\";
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
// The bwapi.ini file in the config directory. Can be overridden by environment variable BWAPI_CONFIG_INI
inline const std::string& configPath()
{
  static std::string path;
  if (path.empty())
  {
    char* env = std::getenv("BWAPI_CONFIG_INI");
    path = env ? env : configDir() + "bwapi.ini";
  }
  return path;
}
extern std::string screenshotFmt;

extern bool isCorrectVersion;
extern bool showWarn;
extern unsigned gdwProcNum;

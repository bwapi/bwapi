#pragma once
#include <windows.h>
#include <string>
#include <storm.h>

// Functions
std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault = 0);
std::string LoadRegString(const char *pszKeyName, const char *pszValueName);

void InitPrimaryConfig();

// Data
static const std::string installPath( LoadRegString("starcraft", "InstallPath")+"\\" );
static const std::string configPath( (installPath + "bwapi-data\\bwapi.ini") );
extern std::string screenshotFmt;

extern bool isCorrectVersion;
extern bool showWarn;
extern bool serverEnabled;
extern DWORD gdwProcNum;


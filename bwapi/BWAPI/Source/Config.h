#pragma once
#include <windows.h>
#include <string>
#include <storm.h>

// Functions
std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault = 0);
std::string LoadRegString(const char *pszKeyName, const char *pszValueName, BYTE bSRegFlags = SREG_NONE);

void InitPrimaryConfig();

// Data
extern std::string sConfigPath;
extern std::string sInstallPath;
extern std::string sLogPath;
extern std::string sScreenshotFormat;

extern bool isCorrectVersion;
extern bool showWarn;
extern bool serverEnabled;
extern DWORD gdwProcNum;


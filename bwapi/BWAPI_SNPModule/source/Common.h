#pragma once
#include <windows.h>
#include "../../Storm/storm.h"

extern char gszInstallPath[MAX_PATH];
extern char gszConfigPath[MAX_PATH];
extern char gszLogPath[MAX_PATH];

extern CRITICAL_SECTION gCrit;
extern DWORD gdwProcId;

#define i(x) MessageBox(NULL, x, "*", MB_OK | MB_ICONASTERISK)
#define w(x) MessageBox(NULL, x, "!", MB_OK | MB_ICONWARNING)
#define e(x) MessageBox(NULL, x, "Error!", MB_OK | MB_ICONERROR)

#define ip(x) ( inet_ntoa(*(in_addr*)&x[2]) )

void Error(DWORD dwErrCode, const char *format = "", ...);
void Log(const char *format = "", ...);
void LogBytes(char *pBuffer, DWORD dwSize, const char *format, ...);

#pragma once
#include <windows.h>
#include <string>
#include "BW/Offsets.h"

std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault = NULL);
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault = 0);

void __fastcall QueueGameCommand(void *pBuffer, DWORD dwLength);

#define QUEUE_COMMAND(x,...) QueueGameCommand(&x(__VA_ARGS__), sizeof(x))

void BWAPIError(const char *format, ...);
void BWAPIError(DWORD dwErrCode, const char *format, ...);
int getFileType(const char *szFileName);

extern char logPath[MAX_PATH];

extern DWORD gdwProcNum;
extern char szInstallPath[MAX_PATH];
extern char szConfigPath[MAX_PATH];

#pragma once
#include <windows.h>
#include "BW/Offsets.h"

void __fastcall QueueGameCommand(void *pBuffer, DWORD dwLength);

void BWAPIError(const char *format, ...);
void BWAPIError(DWORD dwErrCode, const char *format, ...);
int getFileType(const char *szFileName);

extern char logPath[MAX_PATH];

extern DWORD gdwProcNum;
extern char szInstallPath[MAX_PATH];
extern char szConfigPath[MAX_PATH];

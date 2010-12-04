#pragma once
#include <windows.h>
#include "../../Storm/storm.h"

#define PKT_SIZE 512

extern char gszInstallPath[MAX_PATH];
extern char gszConfigPath[MAX_PATH];
extern char gszLogPath[MAX_PATH];

extern CRITICAL_SECTION gCrit;
extern DWORD  gdwProcId;
extern HANDLE ghRecvEvent;
extern HANDLE hRecvThread;

extern volatile gameStruc *gpMGameList;

extern volatile DWORD gdwSendCalls;
extern volatile DWORD gdwSendBytes;
extern volatile DWORD gdwRecvCalls;
extern volatile DWORD gdwRecvBytes;

extern DWORD gdwProduct;
extern DWORD gdwVerbyte;
extern DWORD gdwMaxPlayers;
extern DWORD gdwLangId;

#define i(x) MessageBox(NULL, x, "*", MB_OK | MB_ICONASTERISK)
#define w(x) MessageBox(NULL, x, "!", MB_OK | MB_ICONWARNING)
#define e(x) MessageBox(NULL, x, "Error!", MB_OK | MB_ICONERROR)

void Error(DWORD dwErrCode, const char *format = "", ...);
void Log(const char *format = "", ...);
void LogBytes(char *pBuffer, DWORD dwSize, const char *format, ...);

void CleanGameList(DWORD dwTimeout);
bool isThreadAlive();
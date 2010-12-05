#include "Common.h"

char gszInstallPath[MAX_PATH];
char gszConfigPath[MAX_PATH];
char gszLogPath[MAX_PATH];

CRITICAL_SECTION gCrit;
DWORD  gdwProcId;
HANDLE ghRecvEvent;

volatile gameStruc *gpMGameList;

volatile DWORD gdwSendCalls;
volatile DWORD gdwSendBytes;
volatile DWORD gdwRecvCalls;
volatile DWORD gdwRecvBytes;

DWORD gdwProduct;
DWORD gdwVerbyte;
DWORD gdwMaxPlayers;
DWORD gdwLangId;
HANDLE hRecvThread;

bool isThreadAlive()
{
  DWORD dwExitCode = 0;
  BOOL s = GetExitCodeThread(hRecvThread, &dwExitCode);
  if (!s) return false;
  return dwExitCode == STILL_ACTIVE;
}
void Error(DWORD dwErrCode, const char *format, ...)
{
  char szBuffer[256];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(szBuffer, 256, 256, format, ap);
  va_end(ap);

  char szErrStr[256];
  SErrGetErrorStr(dwErrCode, szErrStr, 256);

  char szFinalStr[512];
  sprintf_s(szFinalStr, 512, "Error: 0x%p\n%s%s", dwErrCode, szErrStr, szBuffer);

  FILE *hLog = fopen(gszLogPath, "a+");
  if ( hLog )
  {
    fprintf(hLog, "%s\n------------\n", szFinalStr);
    fclose(hLog);
  }
  e(szFinalStr);
}

void Log(const char *format, ...)
{
  char szBuffer[256];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(szBuffer, 256, 256, format, ap);
  va_end(ap);

  FILE *hLog = fopen(gszLogPath, "a+");
  if ( hLog )
  {
    fprintf(hLog, "%s\n------------\n", szBuffer);
    fclose(hLog);
  }
  //i(szBuffer);
}

void LogBytes(char *pBuffer, DWORD dwSize, const char *format, ...)
{
  char szBuffer[256];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(szBuffer, 256, 256, format, ap);
  va_end(ap);

  FILE *hLog = fopen(gszLogPath, "a+");
  if ( hLog )
  {
    SYSTEMTIME _time;
    GetLocalTime(&_time);
    fprintf(hLog, "[%02u:%02u:%03u] %s", _time.wMinute, _time.wSecond, _time.wMilliseconds, szBuffer);
    for ( unsigned int i = 0; i < dwSize; ++i )
    {
      if ( i % 16 == 0 )
        fprintf(hLog, "\n    ");
      fprintf(hLog, "%02X ", ((unsigned char)pBuffer[i]));
    }
    fprintf(hLog, "\n------------ ------------\n");
    fclose(hLog);
  }
  //i(szBuffer);
}

void CleanGameList(DWORD dwTimeout)
{
  EnterCriticalSection(&gCrit);
  if ( gpMGameList )
  {
    DWORD dwThisTickCount = GetTickCount();

    volatile gameStruc **g = &gpMGameList;
    while ( *g )
    {
      volatile gameStruc *t = *g;
      if ( dwThisTickCount - (*g)->dwTimer <= dwTimeout )
      {
        g = (volatile gameStruc**)&t->pNext;
      }
      else
      {
        *g = t->pNext;
        if ( t->pExtra )
          SMFree(t->pExtra);
        SMFree((void*)t);
      }
    }
  }
  LeaveCriticalSection(&gCrit);
}

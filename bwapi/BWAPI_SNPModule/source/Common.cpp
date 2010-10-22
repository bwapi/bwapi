#include "Common.h"

char gszInstallPath[MAX_PATH];
char gszConfigPath[MAX_PATH];
char gszLogPath[MAX_PATH];

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
  i(szBuffer);
}

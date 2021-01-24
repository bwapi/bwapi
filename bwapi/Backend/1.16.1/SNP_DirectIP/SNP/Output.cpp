#include "Output.h"
#include "SNPNetwork.h"
#include <windows.h>
#include <stdio.h>

#include <storm.h>

int messageOffset = 0;

void DropMessage(int errorlevel, const char *format, ...)
{
#ifdef _DEBUG
  char szBuffer[512];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(szBuffer, 512, 512, format, ap);
  va_end(ap);

  HDC screen = GetDC(NULL);
  int dropcolor[] = {0x000000, 0x008888, 0x0000FF};
  SetTextColor(screen, dropcolor[errorlevel]);
  TextOutA(screen, 0, messageOffset*16, szBuffer, strlen(szBuffer));
  messageOffset = (messageOffset+1)%40;
  const char *szSpaces = "                                                                                       ";
  TextOutA(screen, 0, messageOffset*16, szSpaces, strlen(szSpaces));
  ReleaseDC(NULL, screen);
#endif
}

void DropLastError(const char *format, ...)
{
  char szBuffer[256];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(szBuffer, sizeof(szBuffer), sizeof(szBuffer), format, ap);
  va_end(ap);

  DWORD dwErrCode = GetLastError();

  char szErrStr[256];
  SErrGetErrorStr(dwErrCode, szErrStr, sizeof(szErrStr));

  char szFinalStr[512];
  sprintf_s(szFinalStr, sizeof(szFinalStr), "Error: 0x%lx;%s;%s", dwErrCode, szBuffer, szErrStr);

  DropMessage(2, szFinalStr);
}

char* sprintfBytes(void* bytes, int byteCount)
{
  char byteBuffer[16];
  static char endBuffer[256];
  char *p = endBuffer;

  for(int i = 0; i < byteCount; i++)
  {
    sprintf(byteBuffer, " %02X", ((unsigned char*)bytes)[i]);
    strcpy(p, byteBuffer);
    p += strlen(byteBuffer);
    if(p > endBuffer+250)
      break;
  }

  return endBuffer+1; // remove leading space
}

void OutputStatus(const char *format, ...)
{
#ifdef _DEBUG
  char szBuffer[512];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(szBuffer, 512, 512, format, ap);
  va_end(ap);

  HDC screen = GetDC(NULL);
  TextOutA(screen, 0, 40*16, szBuffer, strlen(szBuffer));
  ReleaseDC(NULL, screen);
#endif
}
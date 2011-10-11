#pragma once
#include <windows.h>

LONG WINAPI BWAPIExceptionFilter(EXCEPTION_POINTERS *ep);
const char *GetExceptionName(DWORD dwExceptionCode);

class TopLevelExceptionFilter
{
public:
  TopLevelExceptionFilter();
  TopLevelExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpNewExceptionFilter);
  ~TopLevelExceptionFilter();
  
  LONG DefFilterProc(EXCEPTION_POINTERS *ep);
private:
  LPTOP_LEVEL_EXCEPTION_FILTER pOldExceptionFilter;
};

#pragma once
#include <windows.h>

LONG WINAPI BWAPIExceptionFilter(EXCEPTION_POINTERS *ep);
const char * const GetExceptionName(DWORD dwExceptionCode);
void GetCurrentProductVersion(WORD &w1, WORD &w2, WORD &w3, WORD &w4);

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

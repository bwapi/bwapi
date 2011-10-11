#include "ExceptionFilter.h"

#include <string>
#include <vector>
#include <time.h>

#include <Dbghelp.h>
#include <tlhelp32.h>

#include "DLLMain.h"

// Declare the exception filter, which will be registered before DLLMain is called
TopLevelExceptionFilter TopExceptionFilter(&BWAPIExceptionFilter);

struct _customSymbolStore
{
  std::string name;
  DWORD dwStartAddress;
  DWORD dwEndAddress;
};

// The primary exception filter
LONG WINAPI BWAPIExceptionFilter(EXCEPTION_POINTERS *ep)
{
  ShowCursor(TRUE);

  SYSTEMTIME st;
  char szFilename[MAX_PATH];

  // Create the file
  GetSystemTime(&st);
  sprintf(szFilename, "\\bwapi-data\\logs\\Exceptions\\%u_%02u_%02u.txt", st.wYear, st.wMonth, st.wDay);

  FILE *hFile = fopen( (std::string(szInstallPath) + szFilename).c_str(), "a+");
  if ( hFile )
  {
    fprintf(hFile, "\n//////////////////////////////////////////////////\n");

    // Print the time
    time_t _t = time(NULL);
    fprintf(hFile, "TIME: %s\n", ctime(&_t));

    // Get path to Starcraft.exe
    char szExecutableName[MAX_PATH];
    if ( GetModuleFileName(NULL, szExecutableName, MAX_PATH) )
    {
      // Get the File Version information
      DWORD dwUnused, dwVersionSize;
      dwVersionSize = GetFileVersionInfoSize(szExecutableName, &dwUnused);
      if ( dwVersionSize )
      {
        // Version Variables
        VS_FIXEDFILEINFO *pFileInfo;
        UINT dwFileInfoSize;
        void *pVersionData;
        
        pVersionData = malloc(dwVersionSize);
        if ( GetFileVersionInfo(szExecutableName, NULL, dwVersionSize, pVersionData) &&
             VerQueryValue(pVersionData, "\\", (LPVOID*)&pFileInfo, &dwFileInfoSize) )
        {
          // Print version data
          fprintf(hFile, "VERSION: %d.%d.%d.%d\n", HIWORD(pFileInfo->dwProductVersionMS), 
                                                   LOWORD(pFileInfo->dwProductVersionMS),
                                                   HIWORD(pFileInfo->dwProductVersionLS), 
                                                   LOWORD(pFileInfo->dwProductVersionLS) );
        }
        free(pVersionData);
      } // ^if dwVerSize
    } // ^if GetModuleFileName

    // Print the exception info
    DWORD dwExceptionCode = ep->ExceptionRecord->ExceptionCode;
    fprintf(hFile, "EXCEPTION: 0x%p    %s\n", dwExceptionCode, GetExceptionName(dwExceptionCode));

    // Store exception address
    PVOID pExceptionAddr = ep->ExceptionRecord->ExceptionAddress;

    // Print offending module info
    MEMORY_BASIC_INFORMATION memInfo;
    char szOffender[MAX_PATH] = { "_unknown_" };
    if ( VirtualQuery(pExceptionAddr, &memInfo, sizeof(memInfo)) )
      GetModuleFileName(memInfo.AllocationBase ? (HMODULE)memInfo.AllocationBase : GetModuleHandle(NULL), szOffender, MAX_PATH);    
    fprintf(hFile, "FAULT: 0x%p    %s\n", pExceptionAddr, szOffender);

    // Print register information
    fprintf(hFile, "REGISTERS:\n");
    DWORD dwCntxtFlags = ep->ContextRecord->ContextFlags;
    if ( dwCntxtFlags & CONTEXT_INTEGER )
      fprintf(hFile, "  EDI: %08X\n"
                     "  ESI: %08X\n"
                     "  EBX: %08X\n"
                     "  EDX: %08X\n"
                     "  ECX: %08X\n"
                     "  EAX: %08X\n",
                     ep->ContextRecord->Edi,
                     ep->ContextRecord->Esi,
                     ep->ContextRecord->Ebx,
                     ep->ContextRecord->Edx,
                     ep->ContextRecord->Ecx,
                     ep->ContextRecord->Eax);
    if ( dwCntxtFlags & CONTEXT_CONTROL )
      fprintf(hFile, "  EBP: %08X\n"
                     "  EIP: %08X\n"
                     "  ESP: %08X\n",
                     ep->ContextRecord->Ebp,
                     ep->ContextRecord->Eip,
                     ep->ContextRecord->Esp);

    // Get the stack frame
    STACKFRAME sf = { 0 };
    sf.AddrPC.Mode      = AddrModeFlat;
    sf.AddrPC.Offset    = ep->ContextRecord->Eip;
    sf.AddrFrame.Mode   = AddrModeFlat;
    sf.AddrFrame.Offset = ep->ContextRecord->Ebp;
    sf.AddrStack.Mode   = AddrModeFlat;
    sf.AddrStack.Offset = ep->ContextRecord->Esp;

    // Create a context record copy
    CONTEXT c = *ep->ContextRecord;

    // Do the stack trace
    fprintf(hFile, "STACK:\n");

    // Get frequently used handles
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread  = GetCurrentThread();

    // Initialize symbols and stuff
    SymInitialize(hProcess, NULL, FALSE);
    SymSetOptions(SYMOPT_ALLOW_ABSOLUTE_SYMBOLS | SYMOPT_AUTO_PUBLICS |
                  SYMOPT_DEFERRED_LOADS | SYMOPT_FAVOR_COMPRESSED |
                  SYMOPT_INCLUDE_32BIT_MODULES | SYMOPT_LOAD_ANYTHING |
                  SYMOPT_LOAD_LINES);

    // Load all module symbols
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if ( Module32First(hSnapshot, &me32) )
    {
      do
      {
        SymLoadModule(hProcess, NULL, me32.szExePath, me32.szModule, (DWORD)me32.modBaseAddr, me32.modBaseSize);
      } while( Module32Next(hSnapshot, &me32) );
    }
    CloseHandle(hSnapshot);

    // Load custom symbols for Broodwar, etc
    std::vector<_customSymbolStore> customSymbols;
    FILE *hBWSymbols = fopen((std::string(szInstallPath) + "\\bwapi-data\\data\\Broodwar.map").c_str(), "r");
    if ( hBWSymbols )
    {
      char szSymbolName[512];
      DWORD dwAddress = 0;
      DWORD dwSize = 0;
      for (;;)
      {
        int iResult = fscanf(hBWSymbols, "%512s %x %x", szSymbolName, &dwAddress, &dwSize);
        if ( iResult == EOF || iResult == 0 )
          break;
        _customSymbolStore sym = { szSymbolName, dwAddress, dwAddress + dwSize };
        customSymbols.push_back(sym);
      }
      fclose(hBWSymbols);
    }

    // Walk, don't run
    while ( StackWalk(IMAGE_FILE_MACHINE_I386, 
                        hProcess, 
                        hThread, 
                        &sf, 
                        &c, 
                        NULL, 
                        &SymFunctionTableAccess, 
                        &SymGetModuleBase, 
                        NULL) )
    {
      DWORD dwOffset = sf.AddrPC.Offset;
      fprintf(hFile, "  0x%p    ", dwOffset);
      bool foundSomething = false;
      if ( dwOffset )
      {
        // Get the symbol name
        IMAGEHLP_SYMBOL_PACKAGE sip = { 0 };
        sip.sym.SizeOfStruct  = sizeof(IMAGEHLP_SYMBOL);
        sip.sym.MaxNameLength = MAX_SYM_NAME;
        
        DWORD dwJunk = 0;
        if ( SymGetSymFromAddr(hProcess, dwOffset, &dwJunk, &sip.sym) )
        {
          fprintf(hFile, "%s", sip.sym.Name);
          foundSomething = true;
        }

        // Get the file name + line
        IMAGEHLP_LINE il = { 0 };
        il.SizeOfStruct = sizeof(IMAGEHLP_LINE);
        dwJunk = 0;
        if ( SymGetLineFromAddr(hProcess, dwOffset, &dwJunk, &il) )
        {
          fprintf(hFile, "\n                  %s:%u", il.FileName, il.LineNumber);
          foundSomething = true;
        }

        if ( !foundSomething )
        {
          for ( std::vector<_customSymbolStore>::const_iterator i = customSymbols.begin();
                i != customSymbols.end();
                ++i )
          {
            if ( dwOffset >= i->dwStartAddress && dwOffset < i->dwEndAddress )
            {
              fprintf(hFile, "%s", i->name.c_str());
              foundSomething = true;
              break;
            }
          }
        }
      }
      if ( !foundSomething )
      {
        fprintf(hFile, "  ----");
      }
      fprintf(hFile, "\n");
    }
    // Clean up
    SymCleanup(hProcess);
    fclose(hFile);
  } // ^if hFile

  ShowCursor(FALSE);
  /* // Uncomment this block to "fix" integer divisions by 0
  if ( ep->ExceptionRecord->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO )
  {
    ep->ContextRecord->Ecx = 1;
    return EXCEPTION_CONTINUE_EXECUTION;
  }*/

  // Call the previous exception filter
  return TopExceptionFilter.DefFilterProc(ep);
}

const char *GetExceptionName(DWORD dwExceptionCode)
{
  switch ( dwExceptionCode )
  {
  case EXCEPTION_ACCESS_VIOLATION:
    return "EXCEPTION_ACCESS_VIOLATION";
  case EXCEPTION_DATATYPE_MISALIGNMENT:
    return "EXCEPTION_DATATYPE_MISALIGNMENT";
  case EXCEPTION_BREAKPOINT:
    return "EXCEPTION_BREAKPOINT";
  case EXCEPTION_SINGLE_STEP:
    return "EXCEPTION_SINGLE_STEP";
  case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
  case EXCEPTION_FLT_DENORMAL_OPERAND:
    return "EXCEPTION_FLT_DENORMAL_OPERAND";
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:
    return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
  case EXCEPTION_FLT_INEXACT_RESULT:
    return "EXCEPTION_FLT_INEXACT_RESULT";
  case EXCEPTION_FLT_INVALID_OPERATION:
    return "EXCEPTION_FLT_INVALID_OPERATION";
  case EXCEPTION_FLT_OVERFLOW:
    return "EXCEPTION_FLT_OVERFLOW";
  case EXCEPTION_FLT_STACK_CHECK:
    return "EXCEPTION_FLT_STACK_CHECK";
  case EXCEPTION_FLT_UNDERFLOW:
    return "EXCEPTION_FLT_UNDERFLOW";
  case EXCEPTION_INT_DIVIDE_BY_ZERO:
    return "EXCEPTION_INT_DIVIDE_BY_ZERO";
  case EXCEPTION_INT_OVERFLOW:
    return "EXCEPTION_INT_OVERFLOW";
  case EXCEPTION_PRIV_INSTRUCTION:
    return "EXCEPTION_PRIV_INSTRUCTION";
  case EXCEPTION_IN_PAGE_ERROR:
    return "EXCEPTION_IN_PAGE_ERROR";
  case EXCEPTION_ILLEGAL_INSTRUCTION:
    return "EXCEPTION_ILLEGAL_INSTRUCTION";
  case EXCEPTION_NONCONTINUABLE_EXCEPTION:
    return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
  case EXCEPTION_STACK_OVERFLOW:
    return "EXCEPTION_STACK_OVERFLOW";
  case EXCEPTION_INVALID_DISPOSITION:
    return "EXCEPTION_INVALID_DISPOSITION";
  case EXCEPTION_GUARD_PAGE:
    return "EXCEPTION_GUARD_PAGE";
  case EXCEPTION_INVALID_HANDLE:
    return "EXCEPTION_INVALID_HANDLE";
  default:
    break;
  }
  return "UNKNOWN";
}

TopLevelExceptionFilter::TopLevelExceptionFilter()
: pOldExceptionFilter(NULL)
{}

TopLevelExceptionFilter::TopLevelExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpNewExceptionFilter)
: pOldExceptionFilter(NULL)
{
  if ( lpNewExceptionFilter )
    pOldExceptionFilter = SetUnhandledExceptionFilter(lpNewExceptionFilter);
}

TopLevelExceptionFilter::~TopLevelExceptionFilter()
{
  if ( pOldExceptionFilter )
    SetUnhandledExceptionFilter(pOldExceptionFilter);
}

LONG TopLevelExceptionFilter::DefFilterProc(EXCEPTION_POINTERS *ep)
{
  if ( pOldExceptionFilter )
    return pOldExceptionFilter(ep);
  return EXCEPTION_CONTINUE_SEARCH;
}

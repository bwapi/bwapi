#include "ExceptionFilter.h"

#include <string>
#include <vector>
#include <time.h>

#include <Dbghelp.h>
#include <tlhelp32.h>

#include <BWAPI/GameImpl.h>

#include "Config.h"
#include "NewHackUtil.h"
#include "Resolution.h"

BOOL  (WINAPI *_SymInitialize)(HANDLE hProcess,PCSTR UserSearchPath,BOOL fInvadeProcess);
DWORD (WINAPI *_SymSetOptions)(DWORD SymOptions);
DWORD (WINAPI *_SymLoadModule)(HANDLE hProcess,HANDLE hFile,PCSTR ImageName,PCSTR ModuleName,DWORD BaseOfDll,DWORD SizeOfDll);
BOOL  (WINAPI *_StackWalk)(DWORD MachineType,HANDLE hProcess,HANDLE hThread,LPSTACKFRAME StackFrame,PVOID ContextRecord,PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine,PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine,PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine,PTRANSLATE_ADDRESS_ROUTINE TranslateAddress);
PVOID (WINAPI *_SymFunctionTableAccess)(HANDLE hProcess,DWORD AddrBase);
DWORD (WINAPI *_SymGetModuleBase)(HANDLE hProcess,DWORD dwAddr);
BOOL  (WINAPI *_SymGetSymFromAddr)(HANDLE hProcess,DWORD dwAddr,PDWORD pdwDisplacement,PIMAGEHLP_SYMBOL Symbol);
BOOL  (WINAPI *_SymGetLineFromAddr)(HANDLE hProcess,DWORD dwAddr,PDWORD pdwDisplacement,PIMAGEHLP_LINE Line);
BOOL  (WINAPI *_SymCleanup)(HANDLE hProcess);

// Declare the exception filter, which will be registered before DLLMain is called
TopLevelExceptionFilter TopExceptionFilter(&BWAPIExceptionFilter);

struct _customSymbolStore
{
  std::string name;
  DWORD dwStartAddress;
  DWORD dwEndAddress;
};

std::string getModuleNameFrom(LPCVOID pExcptAddr)
{
  MEMORY_BASIC_INFORMATION memInfo;
  char szOffender[MAX_PATH] = { "_unknown_" };
  if ( VirtualQuery(pExcptAddr, &memInfo, sizeof(memInfo)) )
    GetModuleFileName(memInfo.AllocationBase ? (HMODULE)memInfo.AllocationBase : GetModuleHandle(NULL), szOffender, MAX_PATH);
  char *pszLast = strrchr(szOffender, '\\');
  return std::string(pszLast ? pszLast + 1 : szOffender);
}

void GetCurrentProductVersion(WORD &w1, WORD &w2, WORD &w3, WORD &w4)
{
  // Initialize values
  w1 = 0;
  w2 = 0;
  w3 = 0;
  w4 = 0;

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
      
      // allocate version info
      pVersionData = malloc(dwVersionSize);

      // get version data
      if ( GetFileVersionInfo(szExecutableName, NULL, dwVersionSize, pVersionData) &&
           VerQueryValue(pVersionData, "\\", (LPVOID*)&pFileInfo, &dwFileInfoSize) )
      {
        w1 = HIWORD(pFileInfo->dwProductVersionMS);
        w2 = LOWORD(pFileInfo->dwProductVersionMS);
        w3 = HIWORD(pFileInfo->dwProductVersionLS);
        w4 = LOWORD(pFileInfo->dwProductVersionLS);

      }
      free(pVersionData);
    } // ^if dwVerSize
  } // ^if GetModuleFileName
}

#define NULLCHECK(x) if ( !(x) ) fprintf(hFile, "%s is NULL.\n", #x);

// The primary exception filter
LONG WINAPI BWAPIExceptionFilter(EXCEPTION_POINTERS *ep)
{
  DDrawDestroy();
  ShowCursor(TRUE);

  SYSTEMTIME st;
  char szFilename[MAX_PATH];

  // Create the file
  GetSystemTime(&st);
  sprintf_s(szFilename, MAX_PATH, "%sbwapi-data\\logs\\Exceptions\\%u_%02u_%02u.txt", szInstallPath, st.wYear, st.wMonth, st.wDay);

  FILE *hFile = fopen( szFilename, "a+");
  if ( hFile )
  {
    fprintf(hFile, "\n//////////////////////////////////////////////////\n");

    // Print the time
    time_t _t = time(NULL);
    fprintf(hFile, "TIME: %s\n", ctime(&_t));

    // Print version data
    WORD w1,w2,w3,w4;
    GetCurrentProductVersion(w1, w2, w3, w4);
    fprintf(hFile, "VERSION: %hu.%hu.%hu.%hu\n", w1, w2, w3, w4);

    // BWAPI/Broodwar specific
    fprintf(hFile, "BWAPI:\n");
    fprintf(hFile, "  REVISION: %u\n", BWAPI::BroodwarImpl.getRevision());
    fprintf(hFile, "  BUILD: %s\n", BWAPI::BroodwarImpl.isDebug() ? "DEBUG" : "RELEASE");
    fprintf(hFile, "  ERROR: %s\n", BWAPI::BroodwarImpl.getLastError().c_str());
    fprintf(hFile, "  LOCATION: %s %s\n", BWAPI::BroodwarImpl.isMultiplayer() ? (BWAPI::BroodwarImpl.isBattleNet() ? "Battle.net" : "Multiplayer") : "Single Player", BWAPI::BroodwarImpl.isReplay() ? "Replay" : "");
    
    if ( BWAPI::BroodwarImpl.isInGame() )
    {
      fprintf(hFile, "MAP: %s\n     %s\n", BWAPI::BroodwarImpl.mapName().c_str(), BWAPI::BroodwarImpl.mapFileName().c_str());
      NULLCHECK(BWAPI::BroodwarImpl.self());
      NULLCHECK(BWAPI::BroodwarImpl.enemy());
      NULLCHECK(BWAPI::BroodwarImpl.neutral());
      if ( BWAPI::BroodwarImpl.hAIModule && !BWAPI::BroodwarImpl.client )
        fprintf(hFile, "\"Broodwar\" pointer was not initialized for AI module.\n");
      if ( BWAPI::BroodwarImpl.hTournamentModule && !BWAPI::BroodwarImpl.tournamentAI )
        fprintf(hFile, "\"Broodwar\" pointer was not initialized for Tournament module.\n");
    }

    // Print the exception info
    DWORD dwExceptionCode = ep->ExceptionRecord->ExceptionCode;
    fprintf(hFile, "\nEXCEPTION: 0x%p    %s\n", dwExceptionCode, GetExceptionName(dwExceptionCode));

    // Store exception address
    PVOID pExceptionAddr = ep->ExceptionRecord->ExceptionAddress;

    // Print offending module info
    fprintf(hFile, "FAULT:     0x%p    %s\n", pExceptionAddr, getModuleNameFrom(pExceptionAddr).c_str());

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
    if ( _SymInitialize )
    {
      _SymInitialize(hProcess, NULL, FALSE);
      if ( _SymSetOptions )
        _SymSetOptions(SYMOPT_ALLOW_ABSOLUTE_SYMBOLS | SYMOPT_AUTO_PUBLICS |
                        SYMOPT_DEFERRED_LOADS | SYMOPT_FAVOR_COMPRESSED |
                        SYMOPT_INCLUDE_32BIT_MODULES | SYMOPT_LOAD_ANYTHING |
                        SYMOPT_LOAD_LINES);

      // Load all module symbols
      if ( _SymLoadModule )
      {
        MODULEENTRY32 me32;
        me32.dwSize = sizeof(MODULEENTRY32);

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
        if ( Module32First(hSnapshot, &me32) )
        {
          do
          {
            _SymLoadModule(hProcess, NULL, me32.szExePath, me32.szModule, (DWORD)me32.modBaseAddr, me32.modBaseSize);
          } while( Module32Next(hSnapshot, &me32) );
        }
        CloseHandle(hSnapshot);
      } // if _SymLoadModule is valid
    }

    // Load custom symbols for Broodwar, etc
    std::vector<_customSymbolStore> customSymbols;
    char szSymbolMap[MAX_PATH];
    sprintf_s(szSymbolMap, MAX_PATH, "%sbwapi-data\\data\\Broodwar.map", szInstallPath);
    FILE *hBWSymbols = fopen(szSymbolMap, "r");
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
    if ( _StackWalk && _SymFunctionTableAccess && _SymGetModuleBase )
    {
      while ( _StackWalk(IMAGE_FILE_MACHINE_I386, 
                          hProcess, 
                          hThread, 
                          &sf, 
                          &c, 
                          NULL, 
                          _SymFunctionTableAccess,
                          _SymGetModuleBase,
                          NULL) )
      {
        DWORD dwOffset = sf.AddrPC.Offset;
        fprintf(hFile, "  %-16s  0x%p    ", getModuleNameFrom((LPCVOID)dwOffset).c_str(), dwOffset);
        bool foundSomething = false;
        if ( dwOffset )
        {
          // Get the symbol name
          IMAGEHLP_SYMBOL_PACKAGE sip = { 0 };
          sip.sym.SizeOfStruct  = sizeof(IMAGEHLP_SYMBOL);
          sip.sym.MaxNameLength = MAX_SYM_NAME;

          DWORD dwJunk = 0;
          if ( _SymInitialize && _SymGetSymFromAddr && _SymGetSymFromAddr(hProcess, dwOffset, &dwJunk, &sip.sym) )
          {
            fprintf(hFile, "%s", sip.sym.Name);
            foundSomething = true;
          }

          // Get the file name + line
          IMAGEHLP_LINE il = { 0 };
          il.SizeOfStruct = sizeof(IMAGEHLP_LINE);
          dwJunk = 0;
          if ( _SymInitialize && _SymGetLineFromAddr && _SymGetLineFromAddr(hProcess, dwOffset, &dwJunk, &il) )
          {
            fprintf(hFile, "\n                                     %s:%u", il.FileName, il.LineNumber);
            foundSomething = true;
          }

          if ( !foundSomething )
          {
            for ( std::vector<_customSymbolStore>::const_iterator i = customSymbols.begin(),
                  iend = customSymbols.end();
                  i != iend;
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
          fprintf(hFile, "  ----");

        fprintf(hFile, "\n");
      }
    }
    // Clean up
    if ( _SymInitialize && _SymCleanup )
      _SymCleanup(hProcess);
    fclose(hFile);
  } // ^if hFile

  ShowCursor(FALSE);

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

void InitializeSymFunctions()
{
  HMODULE hDbgHlp = LoadLibrary("DbgHelp");
  if ( !hDbgHlp )
    return;

  *(FARPROC*)&_SymInitialize          = GetProcAddress(hDbgHlp, "SymInitialize");
  *(FARPROC*)&_SymSetOptions          = GetProcAddress(hDbgHlp, "SymSetOptions");
  *(FARPROC*)&_SymLoadModule          = GetProcAddress(hDbgHlp, "SymLoadModule");
  *(FARPROC*)&_StackWalk              = GetProcAddress(hDbgHlp, "StackWalk");
  *(FARPROC*)&_SymFunctionTableAccess = GetProcAddress(hDbgHlp, "SymFunctionTableAccess");
  *(FARPROC*)&_SymGetModuleBase       = GetProcAddress(hDbgHlp, "SymGetModuleBase");
  *(FARPROC*)&_SymGetSymFromAddr      = GetProcAddress(hDbgHlp, "SymGetSymFromAddr");
  *(FARPROC*)&_SymGetLineFromAddr     = GetProcAddress(hDbgHlp, "SymGetLineFromAddr");
  *(FARPROC*)&_SymCleanup             = GetProcAddress(hDbgHlp, "SymCleanup");
}

TopLevelExceptionFilter::TopLevelExceptionFilter()
: pOldExceptionFilter(NULL)
{
  InitPrimaryConfig();
  InitializeSymFunctions();
}

TopLevelExceptionFilter::TopLevelExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpNewExceptionFilter)
: pOldExceptionFilter(NULL)
{
  InitPrimaryConfig();

  if ( lpNewExceptionFilter )
    pOldExceptionFilter = SetUnhandledExceptionFilter(lpNewExceptionFilter);
  InitializeSymFunctions();
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

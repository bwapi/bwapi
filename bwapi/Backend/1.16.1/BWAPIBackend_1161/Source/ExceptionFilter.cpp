#include "ExceptionFilter.h"

#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include <fstream>
#include <cassert>

#include <Dbghelp.h>
#include <tlhelp32.h>

#include "BWAPI/GameImpl.h"

#include "Config.h"
#include "NewHackUtil.h"

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
    GetModuleFileNameA(memInfo.AllocationBase ? (HMODULE)memInfo.AllocationBase : GetModuleHandle(NULL), szOffender, MAX_PATH);
  char *pszLast = strrchr(szOffender, '\\');
  return std::string(pszLast ? pszLast + 1 : szOffender);
}

void GetCurrentProductVersion(WORD &w1, WORD &w2, WORD &w3, WORD &w4)
{
  // Initialize values
  w1 = w2 = w3 = w4 = 0;

  // Get path to Starcraft.exe
  char szExecutableName[MAX_PATH];
  if ( GetModuleFileNameA(nullptr, szExecutableName, MAX_PATH) )
  {
    // Get the File Version information
    DWORD dwUnused, dwVersionSize;
    dwVersionSize = GetFileVersionInfoSizeA(szExecutableName, &dwUnused);
    if (dwVersionSize > 0)
    {
      // Version Variables
      VS_FIXEDFILEINFO *pFileInfo;
      UINT dwFileInfoSize;
      std::vector<char> versionData(dwVersionSize);
      
      // get version data
      if (GetFileVersionInfoA(szExecutableName, NULL, dwVersionSize, versionData.data()) &&
        VerQueryValueA(versionData.data(), "\\", (LPVOID*)&pFileInfo, &dwFileInfoSize))
      {
        w1 = HIWORD(pFileInfo->dwProductVersionMS);
        w2 = LOWORD(pFileInfo->dwProductVersionMS);
        w3 = HIWORD(pFileInfo->dwProductVersionLS);
        w4 = LOWORD(pFileInfo->dwProductVersionLS);
      }
      
    } // ^if dwVerSize
  } // ^if GetModuleFileName
}

#define NULLCHECK(x) if ( !(x) ) fprintf(hFile, "%s is NULL.\n", #x);

// The primary exception filter
LONG WINAPI BWAPIExceptionFilter(EXCEPTION_POINTERS *ep)
{
  // Destroy fullscreen mode and show the cursor (something the original doesn't do!)
  ShowCursor(TRUE);

  // Create the log file path
  char szLogFilename[MAX_PATH];
  time_t myTime = time(nullptr);
  strftime(szLogFilename, sizeof(szLogFilename), "Errors\\%Y %b %d.txt", localtime(&myTime));
  
  // Create the file
  FILE *hFile = fopen( (installPath() + szLogFilename).c_str(), "a+");
  if ( hFile )
  {
    fprintf(hFile, "\n//////////////////////////////////////////////////\n");

    // Print the time
    fprintf(hFile, "TIME: %s\n", ctime(&myTime));

    // Print version data
    WORD w1,w2,w3,w4;
    GetCurrentProductVersion(w1, w2, w3, w4);
    fprintf(hFile, "VERSION: %hu.%hu.%hu.%hu\n", w1, w2, w3, w4);

    // BWAPI4/Broodwar specific
    fprintf(hFile, "BWAPI:\n");
    //fprintf(hFile, "  REVISION: %d\n", BWAPI4::BroodwarImpl.getRevision());
    //fprintf(hFile, "  CLIENT VERSION: %d\n", BWAPI4::BroodwarImpl.getClientVersion());
    fprintf(hFile, "  BUILD: %s\n", BWAPI4::BroodwarImpl.isDebug() ? "DEBUG" : "RELEASE");
    fprintf(hFile, "  ERROR: %s\n", BWAPI4::BroodwarImpl.getLastError().c_str());
    fprintf(hFile, "  LOCATION: %s %s\n", BWAPI4::BroodwarImpl.isMultiplayer() ? (BWAPI4::BroodwarImpl.isBattleNet() ? "Battle.net" : "Multiplayer") : "Single Player", BWAPI4::BroodwarImpl.isReplay() ? "Replay" : "");
    
    if ( BWAPI4::BroodwarImpl.isInGame() )
    {
      fprintf(hFile, "MAP: %s\n     %s\n", BWAPI4::BroodwarImpl.mapName().c_str(), BWAPI4::BroodwarImpl.mapFileName().c_str());
      NULLCHECK(BWAPI4::BroodwarImpl.self());
      NULLCHECK(BWAPI4::BroodwarImpl.enemy());
      NULLCHECK(BWAPI4::BroodwarImpl.neutral());
    }

    // Print the exception info
    DWORD dwExceptionCode = ep->ExceptionRecord->ExceptionCode;
    fprintf(hFile, "\nEXCEPTION: 0x%08lX    %s\n", dwExceptionCode, GetExceptionName(dwExceptionCode));

    // Store exception address
    PVOID pExceptionAddr = ep->ExceptionRecord->ExceptionAddress;

    // Print offending module info
    fprintf(hFile, "FAULT:     0x%p    %s\n", pExceptionAddr, getModuleNameFrom(pExceptionAddr).c_str());

    // Print register information
    fprintf(hFile, "REGISTERS:\n");
    DWORD dwCntxtFlags = ep->ContextRecord->ContextFlags;
    if ( dwCntxtFlags & CONTEXT_INTEGER )
      fprintf(hFile,  "  EDI: %08X\n"
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
      fprintf(hFile,  "  EBP: %08X\n"
              "  EIP: %08X\n"
              "  ESP: %08X\n",
              ep->ContextRecord->Ebp,
              ep->ContextRecord->Eip,
              ep->ContextRecord->Esp);

    // Get the stack frame
    STACKFRAME sf = { 0 };
    sf.AddrPC.Mode    = AddrModeFlat;
    sf.AddrPC.Offset  = ep->ContextRecord->Eip;
    sf.AddrFrame.Mode  = AddrModeFlat;
    sf.AddrFrame.Offset = ep->ContextRecord->Ebp;
    sf.AddrStack.Mode  = AddrModeFlat;
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
        me32.dwSize = sizeof(me32);

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
    std::string symbolMapPath = configDir() + "data\\Broodwar.map";
    FILE *hBWSymbols = fopen(symbolMapPath.c_str(), "r");
    if ( hBWSymbols )
    {
      char szSymbolName[512];
      DWORD dwAddress = 0;
      DWORD dwSize = 0;
      for (;;)
      {
        int iResult = fscanf(hBWSymbols, "%511s %8lx %8lx", szSymbolName, &dwAddress, &dwSize);
        if ( iResult == EOF || iResult == 0 )
          break;
        _customSymbolStore sym = { szSymbolName, dwAddress, dwAddress + dwSize };
        customSymbols.push_back(sym);
      }
      fclose(hBWSymbols);
    }
    /*std::ifstream bwSymbols( configDir() + "data\\Broodwar.map");
    if ( bwSymbols )
    {
      DWORD dwAddr = 0, dwSize = 0;
      std::string symName("");
      while ( bwSymbols >> symName >> std::hex >> dwAddr >> dwSize )
      {
        _customSymbolStore sym = { symName, dwAddr, dwAddr + dwSize };
        customSymbols.push_back(sym);
      }
      bwSymbols.close();
    }*/
    
    // Walk, don't run
    if ( _StackWalk && _SymFunctionTableAccess && _SymGetModuleBase )
    {
      while ( _StackWalk(IMAGE_FILE_MACHINE_I386, 
                          hProcess, 
                          hThread, 
                          &sf, 
                          &c, 
                          nullptr, 
                          _SymFunctionTableAccess,
                          _SymGetModuleBase,
                          nullptr) )
      {
        DWORD dwOffset = sf.AddrPC.Offset;
        fprintf(hFile, "  %-16s  0x%08lX    ", getModuleNameFrom((LPCVOID)dwOffset).c_str(), dwOffset);
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
            // Iterate custom symbols, @TODO: make this a map?
            for ( auto &i : customSymbols )
            {
              if ( dwOffset >= i.dwStartAddress && dwOffset < i.dwEndAddress )
              {
                fprintf(hFile, "%s", i.name.c_str());
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
#undef NULLCHECK

#define CASENAME(x) case x: return #x
const char * const GetExceptionName(DWORD dwExceptionCode)
{
  switch ( dwExceptionCode )
  {
  CASENAME(EXCEPTION_ACCESS_VIOLATION);
  CASENAME(EXCEPTION_DATATYPE_MISALIGNMENT);
  CASENAME(EXCEPTION_BREAKPOINT);
  CASENAME(EXCEPTION_SINGLE_STEP);
  CASENAME(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
  CASENAME(EXCEPTION_FLT_DENORMAL_OPERAND);
  CASENAME(EXCEPTION_FLT_DIVIDE_BY_ZERO);
  CASENAME(EXCEPTION_FLT_INEXACT_RESULT);
  CASENAME(EXCEPTION_FLT_INVALID_OPERATION);
  CASENAME(EXCEPTION_FLT_OVERFLOW);
  CASENAME(EXCEPTION_FLT_STACK_CHECK);
  CASENAME(EXCEPTION_FLT_UNDERFLOW);
  CASENAME(EXCEPTION_INT_DIVIDE_BY_ZERO);
  CASENAME(EXCEPTION_INT_OVERFLOW);
  CASENAME(EXCEPTION_PRIV_INSTRUCTION);
  CASENAME(EXCEPTION_IN_PAGE_ERROR);
  CASENAME(EXCEPTION_ILLEGAL_INSTRUCTION);
  CASENAME(EXCEPTION_NONCONTINUABLE_EXCEPTION);
  CASENAME(EXCEPTION_STACK_OVERFLOW);
  CASENAME(EXCEPTION_INVALID_DISPOSITION);
  CASENAME(EXCEPTION_GUARD_PAGE);
  CASENAME(EXCEPTION_INVALID_HANDLE);
  default:
    break;
  }
  return "UNKNOWN";
}
#undef CASENAME

void InitializeSymFunctions()
{
  HMODULE hDbgHlp = LoadLibraryA("DbgHelp");
  if ( !hDbgHlp )
    return;

  (FARPROC&)_SymInitialize          = GetProcAddress(hDbgHlp, "SymInitialize");
  (FARPROC&)_SymSetOptions          = GetProcAddress(hDbgHlp, "SymSetOptions");
  (FARPROC&)_SymLoadModule          = GetProcAddress(hDbgHlp, "SymLoadModule");
  (FARPROC&)_StackWalk              = GetProcAddress(hDbgHlp, "StackWalk");
  (FARPROC&)_SymFunctionTableAccess = GetProcAddress(hDbgHlp, "SymFunctionTableAccess");
  (FARPROC&)_SymGetModuleBase       = GetProcAddress(hDbgHlp, "SymGetModuleBase");
  (FARPROC&)_SymGetSymFromAddr      = GetProcAddress(hDbgHlp, "SymGetSymFromAddr");
  (FARPROC&)_SymGetLineFromAddr     = GetProcAddress(hDbgHlp, "SymGetLineFromAddr");
  (FARPROC&)_SymCleanup             = GetProcAddress(hDbgHlp, "SymCleanup");
}

TopLevelExceptionFilter::TopLevelExceptionFilter()
: pOldExceptionFilter(NULL)
{
  InitializeSymFunctions();
}

TopLevelExceptionFilter::TopLevelExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpNewExceptionFilter)
: pOldExceptionFilter(NULL)
{
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

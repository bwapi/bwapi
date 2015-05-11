#include <windows.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <cassert>
#include "Thread.h"

#include <Util/clamp.h>
#include <Util/Convenience.h>

#include <BWAPI.h>

#include "../../svnrev.h"

#include "BW/Offsets.h"
#include "BW/MenuPosition.h"
#include "BWAPI/GameImpl.h"
#include "DLLMain.h"
#include "ExceptionFilter.h"

#include "Detours.h"
#include "CodePatch.h"
#include "Config.h"
#include "WMode.h"

#include "../../Debug.h"

//---------------------------------------------- QUEUE COMMAND -----------------------------------------------
void __fastcall QueueGameCommand(void *pBuffer, size_t dwLength)
{
  assert(dwLength < BW::BWDATA::TurnBuffer.size());
  if ( !pBuffer || !dwLength || !isCorrectVersion )
    return;

  CAPS caps;
  caps.dwSize = sizeof(CAPS);
  SNetGetProviderCaps(&caps);

  DWORD dwMaxBuffer = clamp<DWORD>(caps.maxmessagesize, 0, BW::BWDATA::TurnBuffer.size());
  if ( dwLength + BW::BWDATA::sgdwBytesInCmdQueue <= dwMaxBuffer )
  {
    // Copy data to primary turn buffer
    memcpy(&BW::BWDATA::TurnBuffer[BW::BWDATA::sgdwBytesInCmdQueue], pBuffer, dwLength);
    BW::BWDATA::sgdwBytesInCmdQueue += dwLength;
    return;
  }
  
  // Verify game mode
  if ( BW::BWDATA::gwGameMode == BW::GAME_GLUES )
    return;

  int turns;
  if ( SNetGetTurnsInTransit(&turns) ) // Buffer is full
  {
    int callDelay = 1;
    if ( BW::BWDATA::NetMode )
      callDelay = clamp<DWORD>(caps.dwCallDelay, 2, 8);

    // This statement will probably never be hit, but just in case
    if ( turns >= 16 - callDelay )
      return;

    // Send the turn and fill the new buffer
    BW::BWDATA::BWFXN_sendTurn();
    memcpy(&BW::BWDATA::TurnBuffer[BW::BWDATA::sgdwBytesInCmdQueue], pBuffer, dwLength);
    BW::BWDATA::sgdwBytesInCmdQueue += dwLength;
  }
  // assume no error, would be fatal in Starcraft anyway
}
int getFileType(const std::string &sFileName)
{
  int rVal = 0;
  Storm::CFile file;

  if ( !sFileName.empty() )
  {
    // Open archive for map checking
    Storm::CArchive archive(sFileName);
    if ( archive )
    {
      // Open scenario.chk file
      if ( archive.openFile("staredit\\scenario.chk") )
        rVal = 1;
    }
    else if ( file.open(sFileName, SFILE_FROM_ABSOLUTE) )
    {
      // Read file data to check if it's a replay
      char tbuff[16];
      if ( file.read(tbuff, sizeof(tbuff)) && *(DWORD*)&tbuff[12] == 'SRer' )
        rVal = 2;
    }
  }
  return rVal;
}

//------------------------------------------------ BWAPI ERROR -----------------------------------------------
void vBWAPIError(const char *format, va_list arg)
{
  // Expand format
  char buffer[256];
  VSNPrintf(buffer, format, arg);

  // Send error message to Broodwar
  BWAPI::Broodwar << BWAPI::Text::Red << "ERROR: " << buffer << std::endl;

  // Retrieve system time
  SYSTEMTIME time;
  GetSystemTime(&time);

  FILE *log = fopen("Errors/bwapi-error.txt", "a+");
  if ( log )
  {
    fprintf(log, "[%u/%02u/%02u - %02u:%02u:%02u] %s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, buffer);
    fclose(log);
  }
}
void BWAPIError(const char *format, ...)
{
  va_list ap;
  va_start(ap,format);
  vBWAPIError(format, ap);
  va_end(ap);
}

void BWAPIError(DWORD dwErrCode, const char *format, ...)
{
  // Expand format
  char buffer[256];
  va_list ap;
  va_start(ap,format);
  VSNPrintf(buffer, format, ap);
  va_end(ap);

  // Obtain last STORM error
  char szErrString[256];
  SErrGetErrorStr(dwErrCode, szErrString, 256);

  BWAPIError("%s    %s", szErrString, buffer);
}

void CheckVersion()
{
  WORD w1, w2, w3, w4;
  GetCurrentProductVersion(w1, w2, w3, w4);
  if (  w1 != SC_VER_1 ||
      w2 != SC_VER_2 ||
      w3 != SC_VER_3 )
  {
    isCorrectVersion = false;
    MessageBoxA(NULL, "The version of Starcraft that you are using is not compatible with BWAPI. BWAPI is intended to run on Starcraft version " STARCRAFT_VER ". However, BWAPI will attempt to continue to run in a reduced functionality mode.", NULL, MB_ICONERROR | MB_OK);
  }
}

DWORD WINAPI PersistentPatch(LPVOID)
{
  RegisterThreadName("BWAPI Persistent Patch");
  for (;;)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds{ 300 });

    // If version is correct
    if ( isCorrectVersion )
    {
      // dialog/menu layer
      if ( BW::BWDATA::ScreenLayers[2].pUpdate != DrawDialogHook && 
           BW::BWDATA::ScreenLayers[2].pUpdate != nullptr )
      {
        BW::pOldDrawDialogProc = BW::BWDATA::ScreenLayers[2].pUpdate;
        BW::BWDATA::ScreenLayers[2].pUpdate = DrawDialogHook;
      }

      // game layer
      if ( BW::BWDATA::ScreenLayers[5].pUpdate != DrawHook && BW::BWDATA::ScreenLayers[5].pUpdate != nullptr )
      {
        BW::pOldDrawGameProc = BW::BWDATA::ScreenLayers[5].pUpdate;
        BW::BWDATA::ScreenLayers[5].pUpdate = DrawHook;
      }
    }

    // Only grab this info if we are not currently detouring the CreateWindowEx procedure
    if ( !detourCreateWindow )
    {
      if ( !ghMainWnd )
        ghMainWnd = SDrawGetFrameWindow();

      if ( ghMainWnd )
      {
        WNDPROC thisProc = (WNDPROC)GetWindowLong(ghMainWnd, GWLP_WNDPROC);
        if ( thisProc != &WindowProc )
        {
          wOriginalProc = thisProc;
          SetWindowLong(ghMainWnd, GWLP_WNDPROC, (LONG)&WindowProc);
        }
      }
    }

    // Checks and names all registered threads, does not need to be called elsewhere
    CheckRegisteredThreads();
  } //loop
}

//------------------------------------------------- DLL MAIN -------------------------------------------------
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
  // Event to prevent the injection of more than one BWAPI DLL
  static HANDLE hEvent = nullptr;
  static HANDLE hPersistThread = nullptr;
  
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_DETACH:
    if ( hEvent != nullptr )
      CloseHandle(hEvent);  // destroy the event

    if (hPersistThread != nullptr)
      CloseHandle(hPersistThread); // destroy the thread
    break;
  case DLL_PROCESS_ATTACH:
  {
    static char szEventName[32];  // The name of the event, unique for this process
    sprintf(szEventName, "BWAPI #%u", GetCurrentProcessId());

    // Create a BWAPI event for this process
    hEvent = CreateEventA(NULL, FALSE, FALSE, szEventName);
    if (GetLastError() == ERROR_ALREADY_EXISTS) // There is a BWAPI module already injected
      return FALSE; // Prevent the injection of this DLL

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    // Do version checking
    CheckVersion();

    // Apply all hacks and patches to the game
    ApplyCodePatches();

    // Create our thread that persistently applies hacks
    hPersistThread = CreateThread(NULL, 0, &PersistentPatch, NULL, 0, NULL);

    break;
  }
  }
  return TRUE;
}

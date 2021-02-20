#include <windows.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <cassert>
#include <algorithm>
#include "Thread.h"

#include "Convenience.h"

#include <BWAPI/Color.h>

#include "../starcraftver.h"

#include "BW/Offsets.h"
#include "BW/MenuPosition.h"
#include "BWAPI/GameImpl.h"
#include "DLLMain.h"
#include "ExceptionFilter.h"

#include "Detours.h"
#include "CodePatch.h"
#include "Config.h"

//---------------------------------------------- QUEUE COMMAND -----------------------------------------------
void __fastcall QueueGameCommand(void *pBuffer, size_t dwLength)
{
  assert(dwLength < BW::BWDATA::TurnBuffer.size());
  if ( !pBuffer || !dwLength || !isCorrectVersion )
    return;

  SNETCAPS caps;
  caps.size = sizeof(SNETCAPS);
  SNetGetProviderCaps(&caps);

  DWORD dwMaxBuffer = std::clamp<size_t>(caps.maxmessagesize, 0, BW::BWDATA::TurnBuffer.size());
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

  DWORD turns;
  if ( SNetGetTurnsInTransit(&turns) ) // Buffer is full
  {
    DWORD callDelay = 1;
    if ( BW::BWDATA::NetMode )
      callDelay = std::clamp<DWORD>(caps.defaultturnsintransit, 2, 8);

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
  BWAPI4::BroodwarImpl.printf("%cERROR: %s", BWAPI4::Text::Red, buffer);

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

    // Checks and names all registered threads, does not need to be called elsewhere
    CheckRegisteredThreads();
  } //loop
}

HANDLE CreateUniqueEvent()
{
  static char szEventName[32];  // The name of the event, unique for this process
  sprintf(szEventName, "BWAPI #%u", GetCurrentProcessId());

  HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, szEventName);
  if (GetLastError() == ERROR_ALREADY_EXISTS) // There is a BWAPI module already injected
    hEvent = nullptr;

  return hEvent;
}

void SetDebug()
{
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
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
    // Create a BWAPI event for this process
    hEvent = CreateUniqueEvent();
    if (!hEvent) // There is a BWAPI module already injected
      return FALSE; // Prevent the injection of this DLL

    // Workaround for injection failures in WINE. The issue is caused by WINE not correctly
    // loading statically linked libraries upon injection, for some reason this fixes it.
    // Note that Storm's SFile module is re-initialized upon loading any file.
    SFileDestroy();

    // Sets debug information
    SetDebug();

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

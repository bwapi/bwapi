#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <cstdio>

#include <BWAPI.h>

#include "BWScriptEmulator.h"

extern "C" __declspec(dllexport) void gameInit(BWAPI::Game* game) { BWAPI::BroodwarPtr = game; }

BOOL APIENTRY DllMain( HANDLE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    break;
  }
  return TRUE;
}

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
  return new BWScriptEmulator();
}

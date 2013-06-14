#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <cstdio>

#include <BWAPI.h>

#include "ExampleTournamentModule.h"

extern "C" __declspec(dllexport) void gameInit(BWAPI::Game* game) { BWAPI::BroodwarPtr = game; }
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
  switch ( ul_reason_for_call )
  {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}

extern "C" __declspec(dllexport) BWAPI::AIModule* newTournamentAI()
{
  return new ExampleTournamentAI();
}

 extern "C" __declspec(dllexport) BWAPI::TournamentModule* newTournamentModule()
{
  return new ExampleTournamentModule();
}

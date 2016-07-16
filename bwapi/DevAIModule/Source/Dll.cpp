#include <BWAPI.h>

#include "DevAIModule.h"


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
  return new DevAIModule();
}

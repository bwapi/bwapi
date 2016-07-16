#include <BWAPI.h>

#include "ExampleTournamentModule.h"

extern "C" __declspec(dllexport) void gameInit(BWAPI::Game* game) { BWAPI::BroodwarPtr = game; }

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
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

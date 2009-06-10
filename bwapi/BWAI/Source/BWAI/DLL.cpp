#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <BWAPI.h>

#include "AI.h"
#include "Globals.h"
///The following commented code will be uncommented when we're finally ready to turn the BWAI project into a dll.

//namespace BWAPI { Game* Broodwar; }

/*

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}


	return TRUE;
}
*/

/* extern "C" __declspec(dllexport) */ BWAPI::AIModule* newAIModule(BWAPI::Game* game)
{
//  BWAPI::Broodwar=game;
  BWAI::ai = new BWAI::AI();
  return BWAI::ai;
}

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <BWAPI.h>

#include "TestMap1.h"
#include "TerranTest.h"
#include "ProtossTest.h"
#include "ZergTest.h"
#include "InterceptorTest.h"
#include "ExistenceTest.h"
#include "MapTest.h"
#include "EventTest.h"
#include "MicroTest.h"
#include "DefaultTestModule.h"
namespace BWAPI { Game* Broodwar; }
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
    BWAPI::BWAPI_init();
		break;
	case DLL_PROCESS_DETACH:
		break;
	}


	return TRUE;
}

 extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule(BWAPI::Game* game)
{
  BWAPI::Broodwar=game;
  if (game->mapFileName()=="testmap1.scm")
    return new TestMap1();
  if (game->mapFileName()=="TerranTest.scm")
    return new TerranTest();
  if (game->mapFileName()=="ProtossTest.scm")
    return new ProtossTest();
  if (game->mapFileName()=="ZergTest.scm")
    return new ZergTest();
  if (game->mapFileName()=="InterceptorTest.scm")
    return new InterceptorTest();
  if (game->mapFileName()=="ExistenceTest.scx")
    return new ExistenceTest();
  if (game->mapFileName()=="MapTest.scx")
    return new MapTest();
  if (game->mapFileName()=="EventTest.scm")
    return new EventTest();
  if (game->mapFileName()=="MicroTest.scm")
    return new MicroTest();
  return new DefaultTestModule();
}

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>

#include "./Types.h"
#include "./BWAPI/Globals.h"
#include "./BWAPI/Game.h"


DWORD WINAPI CTRT_Thread( LPVOID lpThreadParameter )
{
  // IT never gets here
  for(;;)
  {
    BWAPI::Broodwar.update();
    BWAPI::Broodwar.test();
    Sleep(500);
  }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{

   switch (ul_reason_for_call)
   {
     case DLL_PROCESS_ATTACH: 
     {
       CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CTRT_Thread, NULL, 0, NULL);
        return true;
     }
     case DLL_THREAD_ATTACH:
     case DLL_THREAD_DETACH:
     case DLL_PROCESS_DETACH:
      break;
   }

   return true;
}
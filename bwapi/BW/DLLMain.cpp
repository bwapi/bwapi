
#define _CRT_SECURE_NO_DEPRECATE

//#include "BW_UnitTypes.h"
//#include "BW_Unit.h"
//#include "Offsets.h"
//#include "Globals.h"
//#include "Game.h"

#include "windows.h"
#include "assert.h"

#include <vector>
#include <map>
#include <algorithm>
#include "Globals.h"
#include "Game.h"


// OFFSETS


//////////////////////////////////////////////////////////////////////////

void* UnitArray_LastElement = (void*)0x00628280;    // Last Element
void* UnitArray_FirstElement = (void*)0x0059CB40;   // First Element

#include <stdio.h>
DWORD WINAPI CTRT_Thread( LPVOID lpThreadParameter )
{
  // IT never gets here
  FILE *f = fopen("testx.log","wt"); 
  fprintf(f, "asdf");
  fclose(f);
  while(true)
  {
    Broodwar.update();
    Sleep(500);
  }
  return 0;
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
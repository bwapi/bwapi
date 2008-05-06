
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
#include "./BWAPI/CodeHook.h"
 const int back = 0x004D94ED + 5;
void hookTest()
{
  {
     FILE *f = fopen("hooked.log","at"); 
     fprintf(f, "Before the call of target\n");
     fclose(f);
   }
   int target = 0x4D1110;
   __asm
   {
     call [target]
   }
   {
     FILE *f = fopen("hooked.log","at"); 
     fprintf(f, "After\n");
     fclose(f);
   }
   __asm
   {
     jmp [back]
   }
  {
    FILE *f = fopen("hooked.log","at"); 
    fprintf(f, "Should never get here !!!\n");
    fclose(f);
  }
}

void JmpCallPatch(void *pDest, int pSrc, int nNops = 0)
{
  DWORD OldProt = 0;
  VirtualProtect((LPVOID)pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
  unsigned char jmp = 0xE9;
  memcpy((LPVOID)pSrc, &jmp, 1);
  DWORD address = (DWORD)pDest - (DWORD)pSrc - 5;
  memcpy((LPVOID)(pSrc + 1), &address, 4); 
  for (int i = 0; i < nNops; ++i)
    *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
  VirtualProtect((LPVOID)pSrc, 5 + nNops, OldProt, &OldProt);
}

DWORD WINAPI CTRT_Thread( LPVOID lpThreadParameter )
{
  Sleep(2000);
  JmpCallPatch(hookTest, 0x004D94ED, 0);
//  VirtualProtect((LPVOID)0x004D94ED, 6, PAGE_EXECUTE_READWRITE, &notUsed);
//  void** ptr = (void**)(0x004D94ED + 2);
 // memcpy(ptr, &hookFunction, 4);
  //for ever
 // {
    /*
    BWAPI::Broodwar.update();
    BWAPI::Broodwar.test();
    */
   // Sleep(1000);
  //}
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
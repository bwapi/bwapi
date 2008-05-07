
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
#include "./BWAPI/Globals.h"

const int back = 0x004D94ED + 5;
const int target = 0x4D1110;

int frameCount = 0;
int lastEax = 0;
int lastEbx = 0;
int lastEcx = 0;
int lastEdx = 0;
float ticksStart = 0;
float secs = 0;
float perSec = 0;
FILE *f;


//DWORD eaxSave,ebxSave,ecxSave,edxSave
void __declspec(naked)  hookTest()
{
  {
/*   if (BWAPI::count == 0)
      ticksStart = (float)GetTickCount();
   secs = ((float)GetTickCount() - ticksStart)/1000.0;
   perSec = (float)BWAPI::count/secs;
   f = fopen("hooked.log","at"); 
   fprintf(f, "count: %d eax: %d ebx:%d ecx: %d edx %d, secs = %f, perSec = %f\n", BWAPI::count, lastEax, lastEbx, lastEcx, lastEdx, secs, perSec);
   fclose(f);*/
   if (lastEcx >= 0)
   {
     BWAPI::Broodwar.update();
     BWAPI::Broodwar.test();
   }
  }
  __asm
  {
    call [target]
    mov lastEax, eax;
    mov lastEbx, ebx;
    mov lastEcx, ecx;
    mov lastEdx, edx;
    jmp [back]
  }
  {
    FILE *f = fopen("hooked.log","at"); 
    fprintf(f, "Should never get here !!!\n");
    fclose(f);
  }
}
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
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
#pragma (pop)

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